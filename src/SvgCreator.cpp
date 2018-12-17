//#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset>       //to use quickly bit representation
#include <vector>
#include <fstream>
#include <iosfwd>
#include <bits/ios_base.h>

#include "SvgCreator.h"
#include "ConfigParser.h"
#include "ObjNode.h"
#include "SvgUtils.h"
#include "nodeMap/NodesMap.h"


SvgCreator::SvgCreator(char *inputFileName, char *outputFile, map<string, string> *configParameter, char *stringFileName) {

    //New data load
    NodesMap my_map = NodesMap(inputFileName, "r");
    if(!my_map.readFromMemory())
    {
        std::cerr << "Error occurred in loading map" << std::endl;
        exit(-1);
    } //load all the node from the db into a local map
    else
        std::cout << "Load data complete. " << std::endl;

    this->configParameter = configParameter;

    //load the entire char sequence in memory todo check
    setOriginalStringParameter(stringFileName);


    //BEGIN SVG CREATOR
    std::ofstream svg_out(outputFile, std::ios::out | std::ios::binary);

    svg_out << SvgUtils::createSvgHeader(stoi(configParameter->at("WINDOW_WIDTH")),
                                         stoi(configParameter->at("WINDOW_HEIGHT")),
                                         stoi(configParameter->at("SVG_FROM_TOP")));
    //PARAMETER THAT I NEED
    SVG_FROM_TOP = stoi(configParameter->at("SVG_FROM_TOP")) == 1;
    BASIC_KVALUE_KMER = stoi(configParameter->at("BASIC_KVALUE_KMER"));
    //string modality = configParameter->at("MODALITY");

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
        y0 = 60;
    } else { //the root is in the bottom
        x0 = 10;
        y0 = stoi(configParameter->at("WINDOW_HEIGHT")) - 100;
    }

    //for each element devo trasformarlo in tmp_basic_nodeinfo
    //todo future create an onject bigger than the normal node object to add more info
    for(int i = 1; i <= my_map.getNumberOfElement(); i++)
    {
        tmp_basic_nodeInfo tmpNode = createTmpNode(my_map.getNode(i));
        general_map.insert({my_map.getNode(i)->get_index(), tmpNode});
    }

    //set root
    setRootPosition(my_map.getNode(my_map.getNumberOfElement()));
    for(int i = 1; i <= my_map.getNumberOfElement(); i++){
        //if not already insert generate the position
        if (plot_map.count(my_map.getNode(i)->get_index()) == 0) {
            setNodePosition(my_map.getNode(i), &my_map);
        }
    };



    if(configParameter->at("MODALITY")== "MAXREP"){
        modality_type = MAXREP;
    }else if(configParameter->at("MODALITY")== "STATISTIC"){
        modality_type = STATISTIC;
    }else if(configParameter->at("MODALITY")== "BASIC"){
        modality_type = BASIC;
    }

    //some statistic parameter to initialize
    double p_pnorm_parameter = stod(configParameter->at("STATISTIC_PNORM_PARAMETER")); //P-parameter user set;
    int statistics_type = stoi(configParameter->at("STATISTIC_TYPE"));
    //    string tau = "STATISTIC_TAU_" + to_string(statistics_type);
    string tau = "STATISTIC_TAU";
    double tau_i = stod(configParameter->at("STATISTIC_TAU"));


    /*
     * Here there's the modality selection. So we have 3 different code sections according to the parameter that we want to analyze.
     * Basic, Statistic and MaxRep
     */
    switch (modality_type) {
        case MODALITY_TYPE::BASIC:
            /**
             * Iter over all the tree's nodes to extract the informations.
             */
            for (std::pair<unsigned long, tmp_basic_nodeInfo> node : general_map) {

                if (configParameter->at("BASIC_INFO_TO_VISUALIZE") == "DEPTH") {

                    basicMod_depth(&node.second, rgbColor);

                } else if (configParameter->at("BASIC_INFO_TO_VISUALIZE") == "KMER") {

                    basicMod_kmer(&node.second, rgbColor);

                } else if (configParameter->at("BASIC_INFO_TO_VISUALIZE") == "FREQUENCY") {

                    basicMod_frequency(&node.second, rgbColor);

                } else {
                    //default
                    exit(-1);
                }
            }

            //Set the status bar
            if (configParameter->at("BASIC_INFO_TO_VISUALIZE") == "DEPTH") {

                infoStatusBar = "BASIC  DEPTH, threshold: " + configParameter->at("BASIC_DEPTH_THRESHOLD");

            } else if (configParameter->at("BASIC_INFO_TO_VISUALIZE") == "KMER") {

                infoStatusBar = "BASIC  KMER, kvalue: " + configParameter->at("BASIC_KVALUE_KMER");

            } else if (configParameter->at("BASIC_INFO_TO_VISUALIZE") == "FREQUENCY") {

                infoStatusBar = "BASIC  FREQUENCY, freq threshold: " + configParameter->at("BASIC_FREQUENCY_THRESHOLD") + " depth threshold: " + configParameter->at("BASIC_DEPTH_THRESHOLD");


            } else {
                //default
                infoStatusBar = "ERROR!! ";
            }

            infoStatusBar +=  "    StringLength: " + to_string(stringLength) + "   #Nodes: " + to_string(numberOfNode) + "";

            break;


        case MODALITY_TYPE::STATISTIC:


            //set color according to the option chosen
            RgbColor gray;
            gray.r = 140;
            gray.b = 140;
            gray.g = 140;


            infoStatusBar = "STAT  Modality:   " + to_string(statistics_type) + " tau:  " + configParameter->at(tau) +
                            "    StringLength: " +
                            to_string(stringLength) + "         #Nodes: " + to_string(numberOfNode) + "";

            for (std::pair<unsigned long, tmp_basic_nodeInfo> node : general_map) {

                switch (statistics_type) {
                    case 1:

                        //Patch  because a node that it is not a maximal repeat can't have an hight kl_divergence
                        //TODO check why there's this problem
                        //fix the form
                        if( node.second.maxrep_type != MAXREP_TYPE::non_supermaximal  ){
                            if (node.second.kl_divergence >= tau_i) {
                                plot_map[node.first].colorString = "red";
                            }else {
                                plot_map[node.first].colorString = "gray";
                            }
                        }else {
                            //error_counter++;
                            plot_map[node.first].colorString = "gray";
                        }

                        break;

                    case 2:

                        if (node.second.p_norm >= stod(configParameter->at(tau))) {
                            plot_map[node.first].colorString = "red";
                        } else {
                            plot_map[node.first].colorString = "gray";
                        }


                        break;

                    case 3:

                        if (node.second.p_normNoParam >= stod(configParameter->at(tau))) {
                            plot_map[node.first].colorString = "red";
                        } else {
                            plot_map[node.first].colorString = "gray";
                        }


                        break;


                    case 4:

                        if (node.second.h_entropy >= stod(configParameter->at(tau))) {
                            plot_map[node.first].colorString = "red";
                        } else {
                            plot_map[node.first].colorString = "gray";
                        }


                        break;

                    case 5:

                        if (node.second.h_entropySpecial >= stod(configParameter->at(tau))) {
                            plot_map[node.first].colorString = "red";
                        } else {
                            plot_map[node.first].colorString = "gray";
                        }


                        break;


                    default:
                        //error
                        break;

                }

            }

            break;

        case MODALITY_TYPE::MAXREP:

            auto new_alphabet = new vector<char>;
            getAlphabet(stringFileName, new_alphabet);

            //int charNumber = (int) nodeStructure.alphabet.size();
            unsigned long maxrep_counter = 0, supermaxrep_counter = 0, nearsupermax_counter = 0;

            if (configParameter->at("MAXREP_MODALITY") == "frequency") {

                for (std::pair<unsigned long, tmp_basic_nodeInfo> node : general_map) {
                    maxrep_frequency(&node.second, (int)new_alphabet->size());
                }

                infoStatusBar = "REP   Modality: MaxRep Frequency    StringLength: " +
                                to_string(stringLength) + "         #Nodes: " + to_string(numberOfNode) + "";

            } else if (configParameter->at("MAXREP_MODALITY") == "type") {
                for (std::pair<unsigned long, tmp_basic_nodeInfo> node : general_map) {
                    maxrep_type(&node.second, &maxrep_counter, &supermaxrep_counter, &nearsupermax_counter);
                }

                infoStatusBar = "REP   Modality: MaxRep Type    StringLength: " +
                                to_string(stringLength) + "         #Nodes: " + to_string(numberOfNode) +
                                "        Maxrep: " +
                                to_string((maxrep_counter * 100) / numberOfNode)
                                + "%       NearSuperMax: " + to_string((nearsupermax_counter * 100) / numberOfNode) +
                                "%      SuperMax:  " + to_string((supermaxrep_counter * 100) / numberOfNode) + "%";

            } else {
                exit(-1);
            }

            break;

    }


    /**
     * For all the category there's the Node info option, so if the user select SHOW_EDGE_INFO we add the string to the edge.
     */
    if (stoi(configParameter->at("SHOW_EDGE_INFO")) == 1) {

        int max_char = stoi(configParameter->at("MAX_LEAF_CHAR"));

        for (std::pair<unsigned long, tmp_basic_nodeInfo> node : general_map) {

            if (node.second.is_leaf) {
                if (node.second.edge_length > max_char) {
                    edge = getEdge(node.second.edge_index, max_char);
                    edge += "...";
                } else {
                    edge = getEdge(node.second.edge_index, node.second.edge_length);
                }
            } else {
                edge = getEdge(node.second.edge_index, node.second.edge_length);
            }

            general_map[node.first].edge = edge;

        }

    }

    /**
     * PRINT SVG FILE
     * Iterator over all the node information. For each node print the output in a different way according to the
     * selected modality.
     */
    for (std::pair<unsigned long, plotting_info> node : plot_map) {

        switch (modality_type) {
            case MODALITY_TYPE::BASIC:

                SvgUtils::printSvgNodeBlock(&svg_out, general_map[node.first].edge, node.second.w,
                                            node.second.posX, node.second.posY, H,
                                            node.second.color, node.second.opacity);
                break;

            case MODALITY_TYPE::STATISTIC:

                SvgUtils::printSvgNodeBlock2(&svg_out, general_map[node.first].edge, node.second.w, node.second.posX, node.second.posY, H,
                                             node.second.colorString , node.second.opacity);
                break;

            case MODALITY_TYPE::MAXREP:
                SvgUtils::printSvgNodeBlock2(&svg_out, general_map[node.first].edge, node.second.w, node.second.posX, node.second.posY, H,
                                             node.second.colorString, node.second.opacity);
                break;
        }
    };


    printStatusBar(&svg_out, configParameter, infoStatusBar);
    char svgEnd[] = {"</svg>"};  //Close the SVG File
    svg_out << svgEnd;


}

//todo aggiungere il taglio della lunghezza sulle foglie anche nella modalità BASIC