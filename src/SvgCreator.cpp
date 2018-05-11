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

    //READ BINARY FILE
    std::ifstream bin_in(inputFileName, std::ios::binary);
    BitIo<16> bio2;
    openFile(&bin_in, inputFileName, &bio2);

    //PARAMETER CONFIGURATION
    Header header = Header();
    header.readHeader(&bio2);

    //After reading header create the NodeInfoStructure
    NodeInfoStructure nodeStructure = NodeInfoStructure(header.getNodeInfoStructure(), configParameter);

    if (!checkConfigParameter(configParameter, &nodeStructure)) {
        exit(-1);
    }

    //BEGIN SVG CREATOR
    std::ofstream svg_out(outputFile, std::ios::out | std::ios::binary);

    svg_out << SvgUtils::createSvgHeader(stoi(configParameter->at("WINDOW_WIDTH")),
                                         stoi(configParameter->at("WINDOW_HEIGHT")));

    //PARAMETER THAT I NEED
    map<int, ObjNode> hashmap; //Useful only when we represent the dimension of the child equel to the dim of the brother

    float nodeDepth, lb, rb;
    double H = stoi(configParameter->at("BASIC_BLOCK_HEIGHT")); //dovrà poi essere messa nel config e decisa dall'utente
    float rectWidth = stoi(configParameter->at("WINDOW_WIDTH")) - 20;
    float rootNodeWidth = rectWidth;
    int count = 1;

    int maxSuffixArrayLength;

    int fatherLabel, label;
    int frequency;
    float x0, y0, w;
    float x, y;
    string edge = "";

    if (stoi(configParameter->at("SVG_FROM_TOP")) == 1) {
        //the root is on the top
        x0 = 10;
        y0 = 40;

    } else {
        //the root is in the bottom
        x0 = 10;
        y0 = stoi(configParameter->at("WINDOW_HEIGHT")) - 40;
    }

    string nodeInfo = "";
    NodeInfo nodeInfoObj(&nodeStructure);

    float scaleUnit = 0;

    string defaultColor = "RGB";
    RgbColor rgbColor;
    RgbColor blenchedRgbColor;
    HsvColor hsvColor;
    HsvColor blenchedHsvColor;

    //COLOR PARAMETER
    if( configParameter->at("BASIC_COLOR").compare("RGB") == 0){
        defaultColor = "RGB";
        rgbColor.r = stoi(configParameter->at("BASIC_COLOR_VAL_1"));
        rgbColor.g = stoi(configParameter->at("BASIC_COLOR_VAL_2"));
        rgbColor.b = stoi(configParameter->at("BASIC_COLOR_VAL_3"));

        hsvColor = SvgUtils::RgbToHsv(rgbColor);
    } else if ( configParameter->at("BASIC_COLOR").compare("HSV") == 0){
        string defaultColor = "HSV";

        hsvColor.h = stoi(configParameter->at("BASIC_COLOR_VAL_1"));
        hsvColor.s = 100;//stoi(configParameter->at("BASIC_COLOR_VAL_2"));
        hsvColor.v = stoi(configParameter->at("BASIC_COLOR_VAL_3"));
        rgbColor = SvgUtils::HsvToRgb(hsvColor);

    } else {
        //default
    }

    blenchedHsvColor = hsvColor;
    blenchedHsvColor.s = 50;
    blenchedRgbColor = SvgUtils::HsvToRgb(blenchedHsvColor);


    while (!bio2.empty()) {

        //READ AN OTHER NODE AND PUT THE INFOMATION INSIDE THE nodeInfoObj
        nodeInfo = readNextNodeInfo(&bio2);
        nodeInfoObj.setNodeField(&nodeInfo);

//        std::cout << nodeInfoObj.print() << std::endl;

        //ACQUIRE THE DEFAULT PARAMETERS
        nodeDepth = nodeInfoObj.getNodeDepth();
        lb = nodeInfoObj.getLb();
        rb = nodeInfoObj.getRb();
        frequency = rb -lb;

        if (stoi(configParameter->at("TYPE_NODE_DIMENSION")) == 1) {
            //means each children have the same dimension of their brother
            fatherLabel = nodeInfoObj.getFatherLabel();
            label = nodeInfoObj.getLabel();

            ObjNode objNode = ObjNode();
            objNode.setObjNodeDepth(nodeDepth);
            if (nodeDepth == 0) {
                count = 1; // c'è solo la root
            } else {
                count = hashmap[fatherLabel].getNumberOfChildren() +
                        1;// numero di figli del padre del nodo che sto valutando compreso se stesso
            }


            if (nodeDepth == 0) {
                w = rootNodeWidth;
                x = x0;
                y = y0;
                objNode.setObjNodeWid(w);
                objNode.setObjNodeX(x);
                objNode.setObjNodeY(y);
                objNode.setNumberOfChildren(nodeInfoObj.getNumbrOfChildren());
                pair<int, ObjNode> element = {label, objNode};
                hashmap.insert(element);
            } else { //altrimenti scalo la larghezza per la larghezza del suffix interval

                hashmap[fatherLabel].incCounter();
                int actSons = hashmap[fatherLabel].getSonsCount();
                int fatWid = hashmap[fatherLabel].getObjNodeWid();
                int fatX = hashmap[fatherLabel].getObjNodeX();
                int fatY = hashmap[fatherLabel].getObjNodeY();
                if ((rb == lb)) {
                    w = 0;

                } else if (rb != lb) {
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

                //settati tutti i parametri inserisco l'oggetto nodo nella mappa
                pair<int, ObjNode> element = {label, objNode};
                hashmap.insert(element);

            }

        } else if (stoi(configParameter->at("TYPE_NODE_DIMENSION")) == 2) {
            //means thedimensions of a node is proportional with the depth

            if (nodeDepth == 0) {  //se è la radice la disegno grande come il rettangolo
                w = rootNodeWidth;
                maxSuffixArrayLength = rb;
                x = x0;
                y = y0;
                scaleUnit = rootNodeWidth / rb;
            } else { //altrimenti scalo la larghezza per la larghezza del suffix interval
                if ((frequency) == 0) {
                    w = 0;
                } else {
                    w = scaleUnit * (frequency);
                }
                x = x0 + lb * scaleUnit;
                if (stoi(configParameter->at("SVG_FROM_TOP")) == 1) {
                    y = y0 + (nodeDepth * H) + nodeDepth * 0.7;
                } else {
                    y = y0 - (nodeDepth * H) - nodeDepth * 0.7;
                }

            }

        } else {
            std::cout << "Error not valid TYPE_NODE_DIMENSION" << std::endl;
            exit(-1);
        }


        if (stoi(configParameter->at("SHOW_EDGE_INFO")) == 1) {
            //add edge info
            edge = nodeInfoObj.getEdgeDecoded();
        }

        if (stoi(configParameter->at("BASIC_FREQUENCY_COLOR_TYPE")) == 1 ){
            //the frequency is representing with a gradient color
            blenchedHsvColor.v = 100;
            blenchedHsvColor.s = (100 * frequency)/maxSuffixArrayLength + 40;

            SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, SvgUtils::HsvToRgb(blenchedHsvColor));

        } else if ( stoi(configParameter->at("BASIC_FREQUENCY_COLOR_TYPE")) == 2){
            //the node with a frequency lower than a setted thresold are bleached.

            if (frequency >= stoi(configParameter->at("BASIC_THRESHOLD_FOR_GRADIENT"))){
                //colore pieno
                SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, rgbColor);
            } else {
                //sfumato
                SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, blenchedRgbColor);
            }

        } else {
            SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, rgbColor);

        }

    }

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
        std::cout
                << "The node property file generated with the first program is empty, probably you have passed a bad string path"
                << std::endl;
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
            std::cout << "if you chose TYPE_NODE_DIMENSION=1 you need: \n    OPT_LABEL=1\n"
                         "    OPT_FATHERLABLE=1\n"
                         "    OPT_CHILDREN_INFO=1 \n Or you can chose TYPE_NODE_DIMENSION=2" << std::endl;
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
            std::cout << "The edge info isn't available" << std::endl;
            configParameter->at("SHOW_EDGE_INFO") = "0";
        }
    } else {
        //ok
    }

    return true;
}



