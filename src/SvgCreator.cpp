#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset>       //to use quickly bit representation
#include <vector>
#include <fstream>

#include "../Include/SvgCreator.h"
#include "../Include/ConfigParser.h"
#include "../Include/NodeInfoStructure.h"
#include "../Include/Header.h"
#include "../Include/NodeInfo.h"
#include "../Include/ObjNode.h"
#include "../Include/SvgUtils.h"


using namespace std;

SvgCreator::SvgCreator(char *inputFileName, char *outputFile, map<string, string> *configParameter) {

    this->configParameter = configParameter;
    //READ BINARY FILE
    std::ifstream bin_in(inputFileName, std::ios::binary);
    BitIo<16> bio2;
    openFile(&bin_in, inputFileName, &bio2);

    //PARAMETER CONFIGURATION
    Header header = Header();
    header.readHeader(&bio2);

    //After reading header create the NodeInfoStructure
    NodeInfoStructure nodeStructure = NodeInfoStructure(header.getNodeInfoStructure(), configParameter);

    if (!checkConfigParameter(configParameter, &nodeStructure)) exit(-1);

    bool VERBOSE = (stoi(configParameter->at("VERBOSE"))) == 1;

    //BEGIN SVG CREATOR
    std::ofstream svg_out(outputFile, std::ios::out | std::ios::binary);

    svg_out << SvgUtils::createSvgHeader(stoi(configParameter->at("WINDOW_WIDTH")),
                                         stoi(configParameter->at("WINDOW_HEIGHT")));

    //PARAMETER THAT I NEED
    SVG_FROM_TOP = stoi(configParameter->at("SVG_FROM_TOP")) == 1;
    BASIC_KVALUE_KMER =  stoi(configParameter->at("BASIC_KVALUE_KMER"));
    BASIC_KMER = stoi(configParameter->at("BASIC_KMER")) == 1;

    RgbColor rgbColor, blenchedRgbColor;    //RGB COLOR rgbColor : colore pieno, blenchedRgbColor: colore sfumato settato dal config
    HsvColor hsvColor, blenchedHsvColor;    //HSV COLOR


    //INITIALIZE PARAMETER
    H = stoi(configParameter->at("BASIC_BLOCK_HEIGHT")); //dovrà poi essere messa nel config e decisa dall'utente
    rectWidth = stoi(configParameter->at("WINDOW_WIDTH")) - 20;
    rootNodeWidth = rectWidth;
    //Initialize color
    colorSetter(&rgbColor, &blenchedRgbColor, &hsvColor, &blenchedHsvColor, configParameter);

    //Starting point of tree
    if (stoi(configParameter->at("SVG_FROM_TOP")) == 1) { //the root is on the top
        x0 = 10;
        y0 = 40;
    } else { //the root is in the bottom
        x0 = 10;
        y0 = stoi(configParameter->at("WINDOW_HEIGHT")) - 40;
    }


    NodeInfo nodeInfoObj(&nodeStructure);
    while (!bio2.empty()) {

        //READ AN OTHER NODE AND PUT THE INFOMATION INSIDE THE nodeInfoObj
        nodeInfo = readNextNodeInfo(&bio2);
        nodeInfoObj.setNodeField(&nodeInfo);

        if (VERBOSE) std::cout << nodeInfoObj.print() << std::endl;

        //ACQUIRE THE DEFAULT PARAMETERS
        nodeDepth = nodeInfoObj.getNodeDepth();
        depth = nodeInfoObj.getDepth();
        lb = nodeInfoObj.getLb();
        rb = nodeInfoObj.getRb();
        frequency = rb - lb;
//        if ( frequency < 25){
//            continue;
//        }

        ObjNode objNode = ObjNode(); //lo creo fuori dalle varie opzioni
        if (stoi(configParameter->at("TYPE_NODE_DIMENSION")) == 1) {
            //means each children have the same dimension of their brother
            fatherLabel = nodeInfoObj.getFatherLabel();
            label = nodeInfoObj.getLabel();


            objNode.setObjNodeDepth(nodeDepth);
            if (nodeDepth == 0) {
                count = 1; // c'è solo la root
            } else {
                count = hashmap[fatherLabel].getNumberOfChildren() + 1;// numero di figli del padre del nodo che sto valutando compreso se stesso
            }

            if (nodeDepth == 0) {
                w = rootNodeWidth;
                x = x0;
                y = y0;
                objNode.setObjNodeWid(w);
                objNode.setObjNodeX(x);
                objNode.setObjNodeY(y);
                objNode.setNumberOfChildren(nodeInfoObj.getNumbrOfChildren());
                objNode.setObjNodeDepth(depth);
                pair<int, ObjNode> element = {label, objNode};
                hashmap.insert(element);
            } else { //altrimenti scalo la larghezza per la larghezza del suffix interval

                hashmap[fatherLabel].incCounter();
                int actSons = hashmap[fatherLabel].getSonsCount();
                float fatWid = hashmap[fatherLabel].getObjNodeWid();
                double fatX = hashmap[fatherLabel].getObjNodeX();
                double fatY = hashmap[fatherLabel].getObjNodeY();
                if ((rb == lb)) {
                    w = 0;

                } else {
                    //(rb != lb)
                    w = fatWid / count;
                }

                x = fatX + (actSons * w);
                if (stoi(configParameter->at("SVG_FROM_TOP")) == 1) {
                    y = fatY + H + 1;
                } else {
                    y = fatY - H - 1;
                }

                objNode.setObjNodeWid(w);
                objNode.setObjNodeY(y);
                objNode.setObjNodeX(x);
                objNode.setNumberOfChildren(nodeInfoObj.getNumbrOfChildren());
                objNode.setObjNodeDepth(depth);

                //settati tutti i parametri inserisco l'oggetto nodo nella mappa
                pair<int, ObjNode> element = {label, objNode};
                hashmap.insert(element);

            }

        } else if (stoi(configParameter->at("TYPE_NODE_DIMENSION")) == 2) {
            //means the dimensions of a node is proportional with the depth
            if (nodeDepth == 0) {  //Root is large as the windows minus a border
                w = rootNodeWidth;
                maxSuffixArrayLength = rb;
                x = x0;
                y = y0;
                scaleUnit = rootNodeWidth / rb;
            } else {
                //altrimenti scalo la larghezza per la larghezza del suffix interval
                setPositionTYPE_NODE_DIMENSION2();
            }

        } else {
            printf("Error not valid TYPE_NODE_DIMENSION");
            exit(-1);
        }

        //SETTINGS EDGE INFO
        if (stoi(configParameter->at("SHOW_EDGE_INFO")) == 1) {
            edge = nodeInfoObj.getEdgeDecoded();
        }

        if(BASIC_KMER && stoi(configParameter->at("TYPE_NODE_DIMENSION")) == 1){
            if (nodeInfoObj.getDepth()>= BASIC_KVALUE_KMER && hashmap[nodeInfoObj.getFatherLabel()].getObjNodeDepth()<BASIC_KVALUE_KMER) { //Full color
                SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, rgbColor);
            } else { //Blenched
                SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, blenchedRgbColor);
            }
        }
        //SETTING COLOR
//        if (stoi(configParameter->at("BASIC_FREQUENCY_COLOR_TYPE")) == 1) {
//            //the frequency is representing with a gradient color
//            blenchedHsvColor.v = 100;
//            blenchedHsvColor.s = (100 * frequency) / maxSuffixArrayLength + 40;
//
//            SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, SvgUtils::HsvToRgb(blenchedHsvColor));
//
//        } else if (stoi(configParameter->at("BASIC_FREQUENCY_COLOR_TYPE")) == 2) {
//            //the node with a frequency lower than a setted thresold are bleached.
//            if ((frequency >= stoi(configParameter->at("BASIC_FREQUENCY_THRESHOLD"))) && (depth >= stoi(configParameter->at("BASIC_DEPTH_THRESHOLD")))) { //Full color
//                SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, rgbColor);
//            } else { //Blenched
//                SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, blenchedRgbColor);
//            }
//
//        } else {
//            SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, rgbColor);
//        }

    }


    printStatusBar(&svg_out);
    char svgEnd[] = {"</svg>"};  //Close the SVG File
    svg_out << svgEnd;

    bin_in.close();     //Close the input file
    svg_out.close();    //chiudo il file on output*/
}


void SvgCreator::openFile(std::ifstream *bin_in, char *inputFileName, BitIo<16> *bio) {
    if (!(*bin_in).is_open()) {
        std::cout << "I'm not able to open file: " << inputFileName
                  << " probably you must create the file test.bin inside Output with "
                     " the first program" << std::endl;
        exit(7);
    }
    *bin_in >> *bio;

    //Check that the node property file generate with the first program must contain informations
    if ((*bio).size() == 8) {
        printf("The node property file generated with the first program is empty, probably you have passed a bad string path");
        exit(8);
    }

}

string SvgCreator::readNextNodeInfo(BitIo<16> *bio) {

    string nodeInfo = "";

    int e = stoi((*bio).pop_front().to_string(), nullptr, 2);

    for (int i = 0; i < e; i++) {
        nodeInfo += (*bio).pop_front().to_string();
    }

    return nodeInfo;
}

bool SvgCreator::checkConfigParameter(map<string, string> *configParameter, NodeInfoStructure *nodeInfoStructure) {

    string barInfo = "VISUALIZATION MODALITY:  " + configParameter->at("MODALITY");
    statusBarInfo.push_back(barInfo);

    //If I want to use the same dimension for each brother
    int TYPE_NODE_DIMENSION = stoi(configParameter->at("TYPE_NODE_DIMENSION"));
    if (TYPE_NODE_DIMENSION == 1 || TYPE_NODE_DIMENSION == 2) {
        //ok
    } else {
        TYPE_NODE_DIMENSION = 2;
    }

    if (TYPE_NODE_DIMENSION == 1) {
        //I need the children, lable and father label available
        if (!(nodeInfoStructure->OPT_CHILDREN_INFO && nodeInfoStructure->OPT_FATHERLABLE &&
              nodeInfoStructure->OPT_LABEL)) {

            printf("if you chose TYPE_NODE_DIMENSION=1 you need: \n OPT_LABEL=1\n"
                   " OPT_FATHERLABLE=1\n"
                   " OPT_CHILDREN_INFO=1 \n Or you can chose TYPE_NODE_DIMENSION=2");
            return false;
        }
    }

    //CHECK EDGE AVAILABILITY
    if (stoi(configParameter->at("SHOW_EDGE_INFO")) == 1) {
        //show edge info if available
        if (nodeInfoStructure->OPT_EDGEINFO) {
            //ok
        } else {
            //the edge info is not available
            printf("The edge info isn't available");
            configParameter->at("SHOW_EDGE_INFO") = "0";
        }
    } else {
        //ok
    }

    return true;
}

void SvgCreator::printStatusBar(std::ofstream *svg_out) {

    int font_size = 15;
    int heigth = 40 + (font_size+5) * statusBarInfo.size();
    int x = 20;
    int width = stoi(configParameter->at("WINDOW_WIDTH")) - x*2;

    int y = stoi(configParameter->at("WINDOW_HEIGHT")) - heigth -20;
    string bar = "<rect x=\""+to_string(x)+"\" y=\""+to_string(y)+"\" rx=\"10\" ry=\"10\" width=\""+to_string(width)+"\" height=\""+to_string(heigth)+"\"\n"
                 "  style=\"fill:white;stroke:black;stroke-width:5;opacity:1.0\" />";

    int textX = x + 50;
    int textY = y+40;



    for (int i = 0; i < statusBarInfo.size(); i++) {
        int y1 = textY + i * (font_size +5);
        bar += "<text x=\""+to_string(textX)+"\" y=\""+to_string(y1)+"\" \n";
        bar += "font-family=\"Verdana\" font-size=\""+to_string(font_size)+"\" fill=\"black\" >\n";
        bar += statusBarInfo.at(i);
        bar += "  </text>";
    }



    char str[bar.length()];
    strcpy(str, bar.c_str());

    *svg_out << str;

}



