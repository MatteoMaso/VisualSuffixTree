#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset>       //to use quickly bit representation
#include <vector>
#include <fstream>
#include <iosfwd>
#include <bits/ios_base.h>

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
    //Acquire the original string and the size of it
    setOriginalStringParameter(stringFileName);


    //After reading header create the NodeInfoStructure
    NodeInfoStructure nodeStructure = NodeInfoStructure(header.getNodeInfoStructure(), configParameter, stringFileName);

    if (!checkConfigParameter(configParameter, &nodeStructure)) exit(-1);

    bool VERBOSE = (stoi(configParameter->at("VERBOSE"))) == 1;

    //BEGIN SVG CREATOR
    std::ofstream svg_out(outputFile, std::ios::out | std::ios::binary);

    svg_out << SvgUtils::createSvgHeader(stoi(configParameter->at("WINDOW_WIDTH")),
                                         stoi(configParameter->at("WINDOW_HEIGHT")),
                                         stoi(configParameter->at("SVG_FROM_TOP")));

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
    if (modality == "BASIC") {

        while (!bio2.empty()) {

            //READ AN OTHER NODE AND PUT THE INFOMATION INSIDE THE nodeInfoObj
            nodeInfo = readNextNodeInfo(&bio2);
            nodeInfoObj.setNodeField(&nodeInfo);

            if (VERBOSE) std::cout << nodeInfoObj.print(&nodeStructure.alphabet) << std::endl;

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

                //If root set count = 1
                count = (nodeDepth == 0) ? 1 : hashmap[fatherLabel].getNumberOfChildren();

                if (nodeDepth == 0) {
                    w = rootNodeWidth;
                    x = x0;
                    y = y0;
                    numberOfNode = label;

                } else { //altrimenti scalo la larghezza per la larghezza del suffix interval

                    int actSons = hashmap[fatherLabel].getSonsCount();
                    double fatWid = hashmap[fatherLabel].getObjNodeWid();
                    double fatX = hashmap[fatherLabel].getObjNodeX();
                    double fatY = hashmap[fatherLabel].getObjNodeY();
                    w = fatWid / count;
                    x = fatX + (actSons * w);
                    y = (stoi(configParameter->at("SVG_FROM_TOP")) == 1) ? fatY + H + 1 : fatY - H - 1;
                    hashmap[fatherLabel].incCounter();
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
            pair<unsigned long, ObjNode> element = {label, objNode};
            hashmap.insert(element);

            //SETTINGS EDGE INFO
            if (stoi(configParameter->at("SHOW_EDGE_INFO")) == 1) {
                edge = nodeInfoObj.getEdge(nodeInfoObj.getEdgeIndex(), nodeInfoObj.getEdgeLength());
            }

            //SETTING COLOR ACCORDING WITH WHAT I WANT TO SHOW
            string BASIC_INFO_TO_VISUALIZE = configParameter->at("BASIC_INFO_TO_VISUALIZE");
            if (BASIC_INFO_TO_VISUALIZE == "DEPTH") {
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

            } else if (BASIC_INFO_TO_VISUALIZE == "KMER") {


                if (nodeInfoObj.getDepth() >= BASIC_KVALUE_KMER &&
                    hashmap[nodeInfoObj.getFatherLabel()].getObjNodeDepth() < BASIC_KVALUE_KMER) { //Full color
                    SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, rgbColor);
                } else { //Blenched
                    SvgUtils::printSvgNodeBlock(&svg_out, edge, w, x, y, H, blenchedRgbColor);
                }


            } else if (BASIC_INFO_TO_VISUALIZE == "FREQUENCY") {
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

    } else if (modality == "STATISTIC") {
        std::cout << "NOT IMPLEMENTED YET" << std::endl;
    } else if (modality == "MAXREP") {

        int charNumber = (int)nodeStructure.alphabet.size();
        map<unsigned long, tmp_node> maxrep_map;

        unsigned long maxrep_counter = 0, supermaxrep_counter = 0, nearsupermax_counter = 0;

        while (!bio2.empty()) {

            //READ AN OTHER NODE AND PUT THE INFOMATION INSIDE THE nodeInfoObj
            nodeInfo = readNextNodeInfo(&bio2);
            nodeInfoObj.setNodeField(&nodeInfo);

            if (VERBOSE) std::cout << nodeInfoObj.print(&nodeStructure.alphabet) << std::endl;

            tmp_node tmpNode;
            tmpNode.nodeDepth = nodeInfoObj.getNodeDepth();
            tmpNode.depth = nodeInfoObj.getDepth();
            tmpNode.lb = nodeInfoObj.getLb();
            tmpNode.rb = nodeInfoObj.getRb();
            tmpNode.frequency = tmpNode.rb - tmpNode.lb;
            tmpNode.fatherLabel = nodeInfoObj.getFatherLabel();
            tmpNode.label = nodeInfoObj.getLabel();
            tmpNode.numberOfChildren = nodeInfoObj.getNumbrOfChildren();
            tmpNode.numberOfWl = nodeInfoObj.getNumberOfWl();
            tmpNode.edge_index = nodeInfoObj.getEdgeIndex();
            tmpNode.edge_length = nodeInfoObj.getEdgeLength();
            tmpNode.childrenId = nodeInfoObj.getChildrenId();
            tmpNode.wlId = nodeInfoObj.getWlId();
            tmpNode.is_leaf = (nodeInfoObj.getNumbrOfChildren() == 0);
            tmpNode.maxrep_type = (tmpNode.numberOfWl > 1) ? MAXREP_TYPE::maxrep : MAXREP_TYPE::non_supermaximal;

            //means the dimensions of a node is proportional with the depth
            if (tmpNode.nodeDepth == 0) {  //Root is large as the windows minus a border
                tmpNode.w = rootNodeWidth;
                maxSuffixArrayLength = tmpNode.rb;
                tmpNode.posX = x0;
                tmpNode.posY = y0;
                scaleUnit = rootNodeWidth / (tmpNode.rb + 1);
                numberOfNode = tmpNode.label;
            } else { //altrimenti scalo la larghezza per la larghezza del suffix interval
                setPositionTYPE_NODE_DIMENSION3(&tmpNode);
            }

            if ( configParameter->at("MAXREP_MODALITY") == "frequency") {
                double s = ((charNumber) - tmpNode.numberOfWl) * (1.0 / (charNumber));
                tmpNode.opacity = 1 - s;
            }

            maxrep_map.insert({tmpNode.label, tmpNode});
        }

        unsigned long counter = 0;
        std::cout << "\nNumber of node = " << maxrep_map.size() << "\n" << std::endl;

        while (counter < maxrep_map.size()) { //COMPLETE THE SVG CREATION

            tmp_node V = maxrep_map.at(counter);

            //todo sistemare questa opzione
//            if (stoi(configParameter->at("BASIC_CUT_NODE")) == 1) {
//                if (frequency < stoi(configParameter->at("NODE_FREQUENCY_THRESHOLD"))) {
//                    continue;
//
//                }
//            }

            //Find different type of maximal repeat
            if (V.maxrep_type == MAXREP_TYPE::maxrep) {

                maxrep_counter++; //increment the number of maxrep node

                //discover if it's near-supermaximal
                int leafNumber = 0;
                bool is_nearSupMax = false;
                unsigned long U; //id nodo targhet wl(V) label with char of the only one wl(W)
                for (int i = 0; i < V.childrenId.size(); i++) { //per ogni figlio
                    tmp_node W = maxrep_map.at(V.childrenId.at(i));
                    if (!W.is_leaf) continue; //if children is not a leaf continue

                    for (auto i1 : W.wlId) {
                        U = V.wlId.at(i1.first);
                    }

                    if (maxrep_map.at(U).frequency > 1) continue;

                    is_nearSupMax = true;
                    leafNumber++;

                }

                if (is_nearSupMax) {
                    V.maxrep_type = MAXREP_TYPE::nearsupermaximal;
                    nearsupermax_counter++;
                }

                if (leafNumber == V.childrenId.size()) {
                    V.maxrep_type = MAXREP_TYPE::supermaximalrep;
                    supermaxrep_counter++;
                }
            }


            //SETTINGS EDGE INFO
            if (stoi(configParameter->at("SHOW_EDGE_INFO")) == 1) {
                if (V.is_leaf) {
                    if (V.edge_length > stoi(configParameter->at("MAX_LEAF_CHAR"))) {
                        edge = nodeInfoObj.getEdge(V.edge_index, stoul(configParameter->at("MAX_LEAF_CHAR")));
                        edge += "...";
                    } else {
                        edge = nodeInfoObj.getEdge(V.edge_index, V.edge_length);
                    }
                } else {
                    edge = nodeInfoObj.getEdge(V.edge_index, V.edge_length);
                }
            }



            //SETTING COLOR ACCORDING WITH WHAT I WANT TO SHOW
            if (configParameter->at("MAXREP_MODALITY") == "frequency"){
                if (V.maxrep_type == MAXREP_TYPE::non_supermaximal) {
                    SvgUtils::printSvgNodeBlock2(&svg_out, edge, V.w, V.posX, V.posY, H,
                                                 configParameter->at("MAXREP_NONMAXREP_COLOR"), 1);
                } else {
                    SvgUtils::printSvgNodeBlock2(&svg_out, edge, V.w, V.posX, V.posY, H,
                                                 configParameter->at("MAXREP_MAXREP_COLOR"), V.opacity);
                }
            } else if (configParameter->at("MAXREP_MODALITY") == "type"){

                switch (V.maxrep_type){
                    case MAXREP_TYPE ::non_supermaximal :
                        SvgUtils::printSvgNodeBlock2(&svg_out, edge, V.w, V.posX, V.posY, H,
                                                     configParameter->at("MAXREP_NONMAXREP_COLOR"), V.opacity);
                        break;
                    case MAXREP_TYPE ::maxrep :
                        SvgUtils::printSvgNodeBlock2(&svg_out, edge, V.w, V.posX, V.posY, H,
                                                     configParameter->at("MAXREP_MAXREP_COLOR"), V.opacity);
                        break;
                    case MAXREP_TYPE ::nearsupermaximal :
                        SvgUtils::printSvgNodeBlock2(&svg_out, edge, V.w, V.posX, V.posY, H,
                                                     configParameter->at("MAXREP_NEARSUPERMAXIMAL_COLOR"), V.opacity);
                        break;
                    case MAXREP_TYPE ::supermaximalrep :
                            SvgUtils::printSvgNodeBlock2(&svg_out, edge, V.w, V.posX, V.posY, H,
                                                         configParameter->at("MAXREP_SUPERMAXIMAL_COLOR"), V.opacity);
                        break;
                    default :
                        //invalid selection
                        std::cout << "System Error in MAXREP_TYPE of a node" << std::endl;
                        closeOpenFile(&bin_in, &svg_out);
                        exit(-1);
                        break;

                }

            } else {
                //default
                std::cout << "Error in MAXREP_MODALITY!, you can chose: frequency or type" << std::endl;
                closeOpenFile(&bin_in, &svg_out);
                exit(-1);
            }

            counter++;
        }



        infoStatusBar = "REP   Modality: MaxRep     StringLength: " +
                        to_string(stringLength) + "         #Nodes: " + to_string(numberOfNode) + "        Maxrep: " + to_string((maxrep_counter*100)/numberOfNode)
                + "%       NearSuperMax: " + to_string((nearsupermax_counter*100)/numberOfNode) + "%      SuperMax:  " + to_string((supermaxrep_counter*100)/numberOfNode) + "%";
    } else {
        //error
        std::cout << "Error in modality!, you can chose: BASIC, STATISTIC, MAXREP" << std::endl;
        closeOpenFile(&bin_in, &svg_out);
        exit(-1);
    }


    printStatusBar(&svg_out, configParameter, infoStatusBar);
    char svgEnd[] = {"</svg>"};  //Close the SVG File
    svg_out << svgEnd;

    closeOpenFile(&bin_in, &svg_out);
}