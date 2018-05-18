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


class SvgCreator{

public:

    long numberOfNode;

    long stringLength;

    SvgCreator(char *inputFileName, char *outputFile, map<string, string> *configParameter, char * stringFileName);

    void openFile(std::ifstream *bin_in, char *inputFileName, BitIo<16> *bio);

    string readNextNodeInfo(BitIo<16> *bio);

    string infoStatusBar;

private:

    map<string, string> *configParameter;
    map<int, ObjNode> hashmap; //Useful only when we represent the dimension of the child equel to the dim of the brother
    int H; //the height of the block
    double x0, y0, w, x, y, rectWidth, rootNodeWidth;
    int count, fatherLabel, label, frequency, maxSuffixArrayLength, lb, rb, nodeDepth, depth;
    string edge = "";
    double scaleUnit = 0;
    string nodeInfo;
    bool SVG_FROM_TOP;
    int BASIC_KVALUE_KMER;
    bool BASIC_KMER;

    void printStatusBar(std::ofstream *svg_out, map<string, string> *configParameter, string infoToPrint);

    void setPositionTYPE_NODE_DIMENSION2(){
//
//        if ((frequency) == 0) {
//            w = 0;
//        } else {
//            w = scaleUnit * (frequency);
//        }

        w = scaleUnit  * (frequency + 1);
        x = x0 + lb * scaleUnit;

        if (SVG_FROM_TOP) {
            y = y0 + (nodeDepth * H) + nodeDepth * 0.7;
        } else {
            y = y0 - (nodeDepth * H) - nodeDepth * 0.7;
        }
    }

    void colorSetter(RgbColor *rgbColor, RgbColor *blenchedRgbColor, HsvColor *hsvColor,HsvColor *blenchedHsvColor, map<string, string> *configParameter){

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
//                svgColor = "(128,0,0)";
                rgbColor->r = 128;
                rgbColor->g = 0;
                rgbColor->b = 0;
            } else if (color == 9) {
//                svgColor = "(0,0,128)";
                rgbColor->r = 0;
                rgbColor->g = 0;
                rgbColor->b = 128;
            } else if (color == 10) {
//                svgColor = "(128,128,0)";
                rgbColor->r = 128;
                rgbColor->g = 128;
                rgbColor->b = 0;
            } else if (color == 11) {
//                svgColor = "(128,0,128)";
                rgbColor->r = 128;
                rgbColor->g = 0;
                rgbColor->b = 128;
            } else if (color == 12) {
//                svgColor = "(255,0,0)";
                rgbColor->r = 255;
                rgbColor->g = 0;
                rgbColor->b = 0;
            } else if (color == 13) {
//                svgColor = "(192,192,192)";
                rgbColor->r = 192;
                rgbColor->g = 192;
                rgbColor->b = 192;
            } else if (color == 14) {
//                svgColor = "(0,128,128)";
                rgbColor->r = 0;
                rgbColor->g = 128;
                rgbColor->b = 128;
            } else if (color == 15) {
//                svgColor = "(0,255,255)";
                rgbColor->r = 0;
                rgbColor->g = 255;
                rgbColor->b = 255;
            } else if (color == 16) {
//                svgColor = "(255,255,0)";
                rgbColor->r = 255;
                rgbColor->g = 255;
                rgbColor->b = 0;
            } else{
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

    bool checkConfigParameter(map<string, string> *configParameter, NodeInfoStructure * nodeInfoStructure);

    long getStringLength(char * inputFileName);
};

#endif //VISUALSUFFIXTREE_SVGCREATOR_H
