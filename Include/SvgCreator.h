//
// Created by root on 4/29/18.
//

#ifndef VISUALSUFFIXTREE_SVGCREATOR_H
#define VISUALSUFFIXTREE_SVGCREATOR_H

#include "BitIo.h"
#include "ConfigParser.h"
#include "SvgUtils.h"
#include "NodeInfoStructure.h"
#include "ObjNode.h"


class SvgCreator {

public:

    long numberOfNode;

    long stringLength;

    SvgCreator(char *inputFileName, char *outputFile, map<string, string> *configParameter, char *stringFileName);

    void openFile(std::ifstream *bin_in, char *inputFileName, BitIo<16> *bio) {
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

    string readNextNodeInfo(BitIo<16> *bio) {

        string nodeInfo = "";

        int e = stoi((*bio).pop_front().to_string(), nullptr, 2);

        for (int i = 0; i < e; i++) {
            nodeInfo += (*bio).pop_front().to_string();
        }

        return nodeInfo;
    }

    string infoStatusBar;

    void setOriginalStringParameter(char *originalStringFileName) {
        this->originalString = "";
        ifstream file(originalStringFileName);
        if (file.is_open())
            while (file.good())
                getline(file, this->originalString);
        file.close();

        this->stringLength = this->originalString.size();
    }

private:

    enum MAXREP_TYPE {
        non_supermaximal = 0 , maxrep = 1, nearsupermaximal = 2 , supermaximalrep = 3
    };


    enum MODALITY_TYPE{
        BASIC = 0, STATISTIC = 1, MAXREP = 2
    };

    MODALITY_TYPE modality_type = MODALITY_TYPE ::BASIC;

    //Struttura inizializzata comune a tutte le modalità contiene poco più delle info contenute nel file iniziale
    struct tmp_basic_nodeInfo{

        unsigned long label;
        unsigned long nodeDepth;
        unsigned long depth;
        unsigned long lb;
        unsigned long rb;
        unsigned long frequency;
        unsigned long fatherLabel;
        bool is_leaf;
        int numberOfChildren;
        vector<unsigned long> childrenId;
        unsigned long edge_length;
        unsigned long edge_index;

        //WINER LINK INFORMATION
        int numberOfWl;
        map<int, unsigned long> wlId;
        MAXREP_TYPE maxrep_type = MAXREP_TYPE::non_supermaximal;

    };

    std::map<unsigned long, tmp_basic_nodeInfo> general_map; //First map with the initial data, id = label nodo

    struct plotting_info{

        //PLOTTING INFORMATION
        double posX;
        double posY;
        double w;
        double opacity = 1;
        int child_set = 0; //number of child already set
        RgbColor color;
        string colorString;

    };

    std::map<unsigned long, plotting_info> plot_map; //Contain information reguarding plotting

    //passo un node di tipo tmp_basic_nodeInfo e lui salva la posizione nella struttura designata
    void setNodePosition(tmp_basic_nodeInfo * node){

        plotting_info position;

        //Search if father already set
        if(plot_map.count(node->fatherLabel) == 0){
            setNodePosition(&general_map[node->fatherLabel]);
        }

        plotting_info *  father = &plot_map[node->fatherLabel];

        switch (stoi(configParameter->at("TYPE_NODE_DIMENSION")))
        {
            case 1:
                //means each children have the same dimension of their brother
                position.w = father->w / general_map[node->fatherLabel].numberOfChildren;
                position.posX = father->posX + (father->child_set * position.w);
                father->child_set++; //incrementa il numero di figli già settati
            break;

            case 2:
                //means the dimensions of a node is proportional with the frequency
                position.w = scaleUnit * (node->frequency + 1);
                position.posX = x0 + node->lb * scaleUnit;
            break;

            default:
                // istruzioni
                std::cout << "error in type_node_chosing" << std::endl;
                exit(-1);
                break;
        }

        switch (stoi(configParameter->at("SVG_FROM_TOP"))){
            case 1: //from top
                position.posY = y0 + node->nodeDepth * (H + 0.7);
                break;

            case 0: //from bottom
                position.posY = y0 - node->nodeDepth * (H + 0.7);
                break;

            default:
                std::cout << "error in svg from top" << std::endl;
                exit(-1);

        }


        this->plot_map.insert({node->label, position}); //insert position in the map
    }

    void basicMod_depth(tmp_basic_nodeInfo * node, RgbColor rgbColor){

        long depthThreshold = stoi(configParameter->at("BASIC_DEPTH_THRESHOLD"));
        if (stoi(configParameter->at("BASIC_DEPTH_WITH_THRESHOLD")) == 1) {
            if (node->depth > depthThreshold) {
                plot_map[node->label].color = rgbColor;
                plot_map[node->label].opacity = 0.5;
            } else {
                plot_map[node->label].color = rgbColor;
            }
        } else {
            //COLOR WITH A DEPTH GRADIENT
            //bisogna farla in proporzione alla depth della stringa
            std::cout << "to be implement" << std::endl;
        }
    }

    void basicMod_kmer(tmp_basic_nodeInfo * node, RgbColor rgbColor){

        if (node->depth >= BASIC_KVALUE_KMER && general_map[node->fatherLabel].nodeDepth < BASIC_KVALUE_KMER)
        { //Full color
            plot_map[node->label].color = rgbColor;
        } else { //Blenched
            plot_map[node->label].color = rgbColor;
            plot_map[node->label].opacity = 0.5;
        }
    }

    void basicMod_frequency(tmp_basic_nodeInfo * node, RgbColor rgbColor){

        if (stoi(configParameter->at("BASIC_FREQUENCY_COLOR_TYPE")) == 1) {
            //the frequency is representing with a gradient color
//            blenchedHsvColor.v = 100;
//            blenchedHsvColor.s = (100 * frequency) / maxSuffixArrayLength + 40;
            //todo implementare gradiente logaritmico

            //todo non devo passare rgb ma una sfumatura dello stesso in base al gradiente logaritmico
            plot_map[node->label].color = rgbColor;

        } else if (stoi(configParameter->at("BASIC_FREQUENCY_COLOR_TYPE")) == 2) {
            //the node with a frequency lower than a setted thresold are bleached.
            if ((node->frequency >= stoi(configParameter->at("BASIC_FREQUENCY_THRESHOLD"))) &&
                (node->depth >= stoi(configParameter->at("BASIC_DEPTH_THRESHOLD")))) { //Full color
                plot_map[node->label].color = rgbColor;
            } else { //Blenched
                plot_map[node->label].color = rgbColor;
                plot_map[node->label].opacity = 0.5;
            }
        } else {
            //deault
            std::cout << "Error occure in config frequency option" << std::endl;
        }
    }

    void setRootPosition(tmp_basic_nodeInfo * node){
        plotting_info root;

        root.w = rootNodeWidth;
        root.posX = x0;
        root.posY = y0;

        //todo togliere da qui l'inizializzazione dei seguenti 3 parametri
        numberOfNode = node->label;
        maxSuffixArrayLength = node->rb;
        scaleUnit = rootNodeWidth / (node->rb + 1);

        this->plot_map.insert({node->label, root}); //insert position in the map
    }


    //old parameter%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    struct tmp_node {
        unsigned long label;
        unsigned long nodeDepth;
        unsigned long depth;
        unsigned long lb;
        unsigned long rb;
        unsigned long frequency;
        unsigned long fatherLabel;
        bool is_leaf;
        int numberOfChildren;
        vector<unsigned long> childrenId;
        unsigned long edge_length;
        unsigned long edge_index;

        //WINER LINK INFORMATION
        int numberOfWl;
        map<int, unsigned long> wlId;
        MAXREP_TYPE maxrep_type = MAXREP_TYPE::non_supermaximal;

        //PLOTTING INFORMATION
        double posX;
        double posY;
        double w;
        double opacity = 1;
    };




    map<string, string> *configParameter;
    map<int, ObjNode> hashmap; //Useful only when we represent the dimension of the child equal to the dim of the brother
    int H; //the height of the block
    double x0, y0, w, x, y, rectWidth, rootNodeWidth;
    unsigned long fatherLabel, label, frequency, maxSuffixArrayLength, lb, rb, nodeDepth, depth;
    int count;
    string edge = "";
    double scaleUnit = 0;
    string nodeInfo;
    bool SVG_FROM_TOP;
    int BASIC_KVALUE_KMER;

    string originalString; //the original string that we have analized

    void printStatusBar(std::ofstream *svg_out, map<string, string> *configParameter, string infoToPrint) {

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

    void setPositionTYPE_NODE_DIMENSION3(tmp_node *tmpNode) {
        tmpNode->w = scaleUnit * (tmpNode->frequency + 1);
        tmpNode->posX = x0 + tmpNode->lb * scaleUnit;

        if (SVG_FROM_TOP) {
            tmpNode->posY = y0 + (tmpNode->nodeDepth * H) + tmpNode->nodeDepth * 0.7;
        } else {
            tmpNode->posY = y0 - (tmpNode->nodeDepth * H) - tmpNode->nodeDepth * 0.7;
        }
    }

    void setPositionTYPE_NODE_DIMENSION2() {
        w = scaleUnit * (frequency + 1);
        x = x0 + lb * scaleUnit;

        if (SVG_FROM_TOP) {
            y = y0 + (nodeDepth * H) + nodeDepth * 0.7;
        } else {
            y = y0 - (nodeDepth * H) - nodeDepth * 0.7;
        }
    }

    void colorSetter(RgbColor *rgbColor, RgbColor *blenchedRgbColor, HsvColor *hsvColor, HsvColor *blenchedHsvColor,
                     map<string, string> *configParameter) {

        //COLOR PARAMETER
        if (configParameter->at("BASIC_COLOR").compare("RGB") == 0) {

            rgbColor->r = stoi(configParameter->at("BASIC_COLOR_VAL_1"));
            rgbColor->g = stoi(configParameter->at("BASIC_COLOR_VAL_2"));
            rgbColor->b = stoi(configParameter->at("BASIC_COLOR_VAL_3"));

            *hsvColor = SvgUtils::RgbToHsv(*rgbColor);
        } else if (configParameter->at("BASIC_COLOR").compare("HSV") == 0) {
            string defaultColor = "HSV";

            hsvColor->h = stoi(configParameter->at("BASIC_COLOR_VAL_1"));
            hsvColor->s = 100;//stoi(configParameter->at("BASIC_COLOR_VAL_2"));
            hsvColor->v = stoi(configParameter->at("BASIC_COLOR_VAL_3"));
            *rgbColor = SvgUtils::HsvToRgb(*hsvColor);

        } else {

            int color = stoi(configParameter->at("BASIC_COLOR"));
//            string svgColor = "(0,0,0)"; //default nero

            //setto le impostazioni del colore dei rettangoli per l'svg
            if (color == 1) {
//                svgColor = "(0,255,255)";
                rgbColor->r = 0;
                rgbColor->g = 255;
                rgbColor->b = 255;
            } else if (color == 2) {
//                svgColor = "(0,0,0)";
                rgbColor->r = 0;
                rgbColor->g = 0;
                rgbColor->b = 0;
            } else if (color == 3) {
//                svgColor = "(0,0,255)";
                rgbColor->r = 0;
                rgbColor->g = 0;
                rgbColor->b = 255;
            } else if (color == 4) {
//                svgColor = "(255,0,255)";
                rgbColor->r = 255;
                rgbColor->g = 0;
                rgbColor->b = 255;
            } else if (color == 5) {
//                svgColor = "(128,128,128)";
                rgbColor->r = 128;
                rgbColor->g = 128;
                rgbColor->b = 128;
            } else if (color == 6) {
//                svgColor = "(0,128,0)";
                rgbColor->r = 0;
                rgbColor->g = 128;
                rgbColor->b = 0;
            } else if (color == 7) {
//                svgColor = "(0,255,0)";
                rgbColor->r = 0;
                rgbColor->g = 255;
                rgbColor->b = 0;
            } else if (color == 8) {
                rgbColor->r = 128;
                rgbColor->g = 0;
                rgbColor->b = 0;
            } else if (color == 9) {
                rgbColor->r = 0;
                rgbColor->g = 0;
                rgbColor->b = 128;
            } else if (color == 10) {
                rgbColor->r = 128;
                rgbColor->g = 128;
                rgbColor->b = 0;
            } else if (color == 11) {
                rgbColor->r = 128;
                rgbColor->g = 0;
                rgbColor->b = 128;
            } else if (color == 12) {
                rgbColor->r = 255;
                rgbColor->g = 0;
                rgbColor->b = 0;
            } else if (color == 13) {
                rgbColor->r = 192;
                rgbColor->g = 192;
                rgbColor->b = 192;
            } else if (color == 14) {
                rgbColor->r = 0;
                rgbColor->g = 128;
                rgbColor->b = 128;
            } else if (color == 15) {
                rgbColor->r = 0;
                rgbColor->g = 255;
                rgbColor->b = 255;
            } else if (color == 16) {
                rgbColor->r = 255;
                rgbColor->g = 255;
                rgbColor->b = 0;
            } else {
                //default
                rgbColor->r = 255;
                rgbColor->g = 255;
                rgbColor->b = 0;
            }

            *hsvColor = SvgUtils::RgbToHsv(*rgbColor);

        }

        *blenchedHsvColor = *hsvColor;
        blenchedHsvColor->s = 50;
        *blenchedRgbColor = SvgUtils::HsvToRgb(*blenchedHsvColor);
    }

    bool checkConfigParameter(map<string, string> *configParameter, NodeInfoStructure *nodeInfoStructure) {

        string barInfo = "VISUALIZATION MODALITY:  " + configParameter->at("MODALITY");

        string modality = configParameter->at("MODALITY");


        if (modality.compare("BASIC") == 0) {
            modality_type = MODALITY_TYPE ::BASIC;
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
            modality_type = MODALITY_TYPE ::STATISTIC;

        } else if (modality.compare("MAXREP") == 0) {
            modality_type = MODALITY_TYPE ::MAXREP;

        } else {
            //error
            std::cout << "MODALITY WRONG, you must chose one among: BASIC, STATISTIC, MAXREP" << std::endl;
            return false;
        }


        return true;
    }

    void closeOpenFile(std::ifstream * bin_in, std::ofstream * svg_out){
        bin_in->close(); //Close the input file
        svg_out->close(); //chiudo il file on output*/
    }

};

#endif //VISUALSUFFIXTREE_SVGCREATOR_H
