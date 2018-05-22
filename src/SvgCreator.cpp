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

SvgCreator::SvgCreator(char *inputFileName, char *outputFile, map<string, string> *configParameter,
                       char *stringFileName) {

    this->configParameter = configParameter;
    //READ BINARY FILE
    std::ifstream bin_in(inputFileName, std::ios::binary);
    BitIo<16> bio2;
    openFile(&bin_in, inputFileName, &bio2);

    //PARAMETER CONFIGURATION
    Header header = Header();
    header.readHeader(&bio2);
    stringLength = getStringLength(stringFileName);


    string originalString;
    ifstream file(stringFileName);
    //todo spostare questa cosa da qualche altra parte
    if (file.is_open())
        while (file.good())
            getline(file, originalString);
    file.close();



    //After reading header create the NodeInfoStructure
    NodeInfoStructure nodeStructure = NodeInfoStructure(header.getNodeInfoStructure(), configParameter, stringFileName);

    if (!checkConfigParameter(configParameter, &nodeStructure)) exit(-1);

    bool VERBOSE = (stoi(configParameter->at("VERBOSE"))) == 1;

    //BEGIN SVG CREATOR
    std::ofstream svg_out(outputFile, std::ios::out | std::ios::binary);

    svg_out << SvgUtils::createSvgHeader(stoi(configParameter->at("WINDOW_WIDTH")),
                                         stoi(configParameter->at("WINDOW_HEIGHT")), stoi(configParameter->at("SVG_FROM_TOP")));

    //PARAMETER THAT I NEED
    SVG_FROM_TOP = stoi(configParameter->at("SVG_FROM_TOP")) == 1;
    BASIC_KVALUE_KMER = stoi(configParameter->at("BASIC_KVALUE_KMER"));
    string modality = configParameter->at("MODALITY");

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
        y0 = stoi(configParameter->at("WINDOW_HEIGHT")) - 100;
    }


    NodeInfo nodeInfoObj(&nodeStructure, &originalString);
    //BASIC MODALITY
    if (modality.compare("BASIC") == 0) {


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
            fatherLabel = nodeInfoObj.getFatherLabel();
            label = nodeInfoObj.getLabel();


            if (stoi(configParameter->at("BASIC_CUT_NODE")) == 1) {
                if (frequency < stoi(configParameter->at("NODE_FREQUENCY_THRESHOLD"))) {
                    continue;
                }
            }

            if (stoi(configParameter->at("TYPE_NODE_DIMENSION")) == 1) {
                //means each children have the same dimension of their brother

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
                    numberOfNode = label;

                } else { //altrimenti scalo la larghezza per la larghezza del suffix interval

                    hashmap[fatherLabel].incCounter();
                    int actSons = hashmap[fatherLabel].getSonsCount();
                    float fatWid = hashmap[fatherLabel].getObjNodeWid();
                    double fatX = hashmap[fatherLabel].getObjNodeX();
                    double fatY = hashmap[fatherLabel].getObjNodeY();
                    if ((rb == lb)) {
                        w = 0;

                    } else { //(rb != lb)
                        w = fatWid / count;
                    }

                    x = fatX + (actSons * w);
                    if (stoi(configParameter->at("SVG_FROM_TOP")) == 1) {
                        y = fatY + H + 1;
                    } else {
                        y = fatY - H - 1;
                    }
                }

            } else if (stoi(configParameter->at("TYPE_NODE_DIMENSION")) == 2) {
                //means the dimensions of a node is proportional with the depth
                if (nodeDepth == 0) {  //Root is large as the windows minus a border
                    w = rootNodeWidth;
                    maxSuffixArrayLength = rb;
                    x = x0;
                    y = y0;
                    scaleUnit = rootNodeWidth / (rb + 1);
                    numberOfNode = label;
                } else {
                    //altrimenti scalo la larghezza per la larghezza del suffix interval
                    setPositionTYPE_NODE_DIMENSION2();
                }

            } else {
                printf("Error not valid TYPE_NODE_DIMENSION");
                exit(-1);
            }

            ObjNode objNode = ObjNode(); //lo creo fuori dalle varie opzioni
            objNode.setObjNodeDepth(nodeDepth);
            objNode.setObjNodeWid(w);
            objNode.setObjNodeX(x);
            objNode.setObjNodeY(y);
            objNode.setNumberOfChildren(nodeInfoObj.getNumbrOfChildren());
            objNode.setObjNodeDepth(depth);
            pair<int, ObjNode> element = {label, objNode};
            hashmap.insert(element);

            //SETTINGS EDGE INFO
            if (stoi(configParameter->at("SHOW_EDGE_INFO")) == 1) {
                edge = nodeInfoObj.getEdge(&originalString, nodeInfoObj.getEdgeIndex(), nodeInfoObj.getEdgeLength());
            }

            //SETTING COLOR ACCORDING WITH WHAT I WANT TO SHOW
            string BASIC_INFO_TO_VISUALIZE = configParameter->at("BASIC_INFO_TO_VISUALIZE");
            if (BASIC_INFO_TO_VISUALIZE.compare("DEPTH") == 0) {
                long depthThreshold = stoi(configParameter->at("BASIC_DEPTH_THRESHOLD"));
                if (stoi(configParameter->at("BASIC_DEPTH_WITH_THRESHOLD")) == 1) {
                    if (nodeInfoObj.getDepth() > depthThreshold) {
                        SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, blenchedRgbColor);
                    } else {
                        SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, rgbColor);
                    }
                } else {
                    //COLOR WITH A DEPTH GRADIENT
                    //bisogna farla in proporzione alla depth della stringa
                    std::cout << "to be implement" << std::endl;
                }

            } else if (BASIC_INFO_TO_VISUALIZE.compare("KMER") == 0) {


                if (nodeInfoObj.getDepth() >= BASIC_KVALUE_KMER &&
                    hashmap[nodeInfoObj.getFatherLabel()].getObjNodeDepth() < BASIC_KVALUE_KMER) { //Full color
                    SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, rgbColor);
                } else { //Blenched
                    SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, blenchedRgbColor);
                }


            } else if (BASIC_INFO_TO_VISUALIZE.compare("FREQUENCY") == 0) {
                if (stoi(configParameter->at("BASIC_FREQUENCY_COLOR_TYPE")) == 1) {
                    //the frequency is representing with a gradient color
                    blenchedHsvColor.v = 100;
                    blenchedHsvColor.s = (100 * frequency) / maxSuffixArrayLength + 40;

                    SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, SvgUtils::HsvToRgb(blenchedHsvColor));

                } else if (stoi(configParameter->at("BASIC_FREQUENCY_COLOR_TYPE")) == 2) {
                    //the node with a frequency lower than a setted thresold are bleached.
                    if ((frequency >= stoi(configParameter->at("BASIC_FREQUENCY_THRESHOLD"))) &&
                        (depth >= stoi(configParameter->at("BASIC_DEPTH_THRESHOLD")))) { //Full color
                        SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, rgbColor);
                    } else { //Blenched
                        SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, blenchedRgbColor);
                    }
                } else {
                    //deault
                    std::cout << "Error occure in config frequency option" << std::endl;
                }

            } else {
                //default
                SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, rgbColor);
            }


        }

        infoStatusBar = "STATUS BAR    Modality: Basic     StringLength: " +
                        to_string(stringLength) + "       #Nodes: " + to_string(numberOfNode);

    } else if (modality.compare("STATISTIC") == 0) {
        std::cout << "NOT IMPLEMENTED YET" << std::endl;
    } else if (modality.compare("MAXREP") == 0) {

        enum MAXREP_TYPE {maxrep = 1, supermaximalrep = 2, nearsupermaximal = 3, non_supermaximal = 0};

        MAXREP_TYPE max_type;
        int nWl = 0;    //#number of Winer Link

        int charNumber = 5; //todo trovare sto valore in modo parametrico
        double opacity = 1;

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
            fatherLabel = nodeInfoObj.getFatherLabel();
            label = nodeInfoObj.getLabel();

            nWl = nodeInfoObj.getNumberOfWl();

            if( nWl > 1 ){
                max_type = MAXREP_TYPE::maxrep;
            } else if (false){
                //todo per i supermaximal
                max_type = MAXREP_TYPE::supermaximalrep;
            }else if (false){
                //todo per i near supermaximal
                max_type = MAXREP_TYPE::nearsupermaximal;
            }else {
                //Default
                max_type = MAXREP_TYPE::non_supermaximal;
            }


//            if (stoi(configParameter->at("BASIC_CUT_NODE")) == 1) {
//                if (frequency < stoi(configParameter->at("NODE_FREQUENCY_THRESHOLD"))) {
//                    continue;
//                }
//            }


            //means the dimensions of a node is proportional with the depth
            if (nodeDepth == 0) {  //Root is large as the windows minus a border
                w = rootNodeWidth;
                maxSuffixArrayLength = rb;
                x = x0;
                y = y0;
                scaleUnit = rootNodeWidth / (rb + 1);
                numberOfNode = label;
            } else {
                //altrimenti scalo la larghezza per la larghezza del suffix interval
                setPositionTYPE_NODE_DIMENSION2();
            }


            ObjNode objNode = ObjNode(); //lo creo fuori dalle varie opzioni
            objNode.setObjNodeDepth(nodeDepth);
            objNode.setObjNodeWid(w);
            objNode.setObjNodeX(x);
            objNode.setObjNodeY(y);
            objNode.setNumberOfChildren(nodeInfoObj.getNumbrOfChildren());
            objNode.setObjNodeDepth(depth);
            pair<int, ObjNode> element = {label, objNode};
            hashmap.insert(element);

            //SETTINGS EDGE INFO
            if (stoi(configParameter->at("SHOW_EDGE_INFO")) == 1) {
                edge = nodeInfoObj.getEdge(&originalString, nodeInfoObj.getEdgeIndex(), nodeInfoObj.getEdgeLength()
                );
            }

            if (stoi(configParameter->at("MAXREP_BLEND_WL")) == 1){
                double s = (charNumber - nWl) * (1.0/charNumber);
                opacity = 1-s;
            }


            //SETTING COLOR ACCORDING WITH WHAT I WANT TO SHOW
            if (stoi(configParameter->at("MAXREP_SHOW_MAX_REP")) == 1){
                //colora i max rep e sfuma gli altri
                if ( max_type == MAXREP_TYPE::maxrep ){
                    SvgUtils::printSvgNodeBlock2(&svg_out, edge, w, x, y, H, configParameter->at("MAXREP_MAXREP_COLOR"), opacity);
                } else if (max_type == MAXREP_TYPE::supermaximalrep){
                    SvgUtils::printSvgNodeBlock2(&svg_out, edge, w, x, y, H, configParameter->at("MAXREP_SUPERMAXIMAL_COLOR"), 1);
                }else if (max_type == MAXREP_TYPE::nearsupermaximal){
                    SvgUtils::printSvgNodeBlock2(&svg_out, edge, w, x, y, H, configParameter->at("MAXREP_NEARSUPERMAXIMAL_COLOR"), 1);
                }else {
                    //non supermaximal
                    SvgUtils::printSvgNodeBlock2(&svg_out, edge, w, x, y, H, configParameter->at("MAXREP_NONMAXREP_COLOR"), 1);
                }


            }else {
                //default
                SvgUtils::printSvgNodeBlock2(&svg_out, edge, w, x, y, H, configParameter->at("MAXREP_NONMAXREP_COLOR"), 1);

            }

        }

        infoStatusBar = "STATUS BAR    Modality: MaxRep     StringLength: " +
                        to_string(stringLength) + "       #Nodes: " + to_string(numberOfNode);
    } else {
        //error
        exit(-1);
    }


    printStatusBar(&svg_out, configParameter, infoStatusBar);
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

    string modality = configParameter->at("MODALITY");

    if (modality.compare("BASIC") == 0) {
        //If I want to use the same dimension for each brother
        int TYPE_NODE_DIMENSION = stoi(configParameter->at("TYPE_NODE_DIMENSION"));
        if (TYPE_NODE_DIMENSION == 1 || TYPE_NODE_DIMENSION == 2) {
            //ok
        } else {
            TYPE_NODE_DIMENSION = 2; //default 2
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
    } else if (modality.compare("STATISTIC") == 0) {

    } else if (modality.compare("MAXREP") == 0) {

    } else {
        //error
        std::cout << "MODALITY WRONG, you must chose one among: BASIC, STATISTIC, MAXREP" << std::endl;
        return false;
    }


    return true;
}

void SvgCreator::printStatusBar(std::ofstream *svg_out, map<string, string> *configParameter, string infoToPrint) {

    int font_size = 15;
    int heigth = 40 + (font_size + 5);
    int x = 0;
    int width = stoi(configParameter->at("WINDOW_WIDTH"));

    int y = stoi(configParameter->at("WINDOW_HEIGHT")) - heigth;
    string bar = "<rect x=\"" + to_string(x) + "\" y=\"" + to_string(y) + "\" rx=\"0\" ry=\"0\" width=\"" +
                 to_string(width) + "\" height=\"" + to_string(heigth) + "\"\n"
                                                                         "  style=\"fill:white;stroke:black;stroke-width:1;opacity:1.0\" />";

    int textX = x + 50;
    int textY = y + 40;


    for (int i = 0; i < 1; i++) {
        int y1 = textY + i * (font_size + 5);
        bar += "<text x=\"" + to_string(textX) + "\" y=\"" + to_string(y1) + "\" \n";
        bar += "font-family=\"Verdana\" font-size=\"" + to_string(font_size) + "\" fill=\"black\" >\n";
        bar += infoToPrint;
        bar += "  </text>";
    }


    char str[bar.length()];
    strcpy(str, bar.c_str());

    *svg_out << str;

}

long SvgCreator::getStringLength(char *inputFileName) {
    string txt;
    ifstream file(inputFileName);

    if (file.is_open())
        while (file.good())
            getline(file, txt);
    file.close();

    return txt.length();
}
