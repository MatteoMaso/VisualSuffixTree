#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset>       //to use quickly bit representation
#include <vector>
#include <fstream>

#include "../Include/SvgCreator.h"
#include "../Include/BitIo.h"
#include "../Include/BitDecoder.h"
#include "../Include/ConfigParser.h"

using namespace std;

SvgCreator::SvgCreator(char *inputFileName, char *outputFile) {

    map<string, string> configParameter;
    ConfigParser cfPars("./Settings/config.cfg", &configParameter); //Initialize the configurations Parameter

    //BEGIN SVG CREATOR
    std::ofstream svg_out(outputFile, std::ios::out | std::ios::binary);


    std::ifstream bin_in(inputFileName, std::ios::binary);
    if (!bin_in.is_open()) {
        std::cout << "I'm not able to open file: " << inputFileName
                  << " probably you must create the file test.bin inside Output with "
                     " the first program" << std::endl;
    }

    BitIo<16> bio2;
    bin_in >> bio2;


    svg_out << getHeader("headerSvg.txt"); //Insert the header SVG into the file

    int a, b, c;
    int x, y, i, j, z, x0, y0, H, w;
    string edge;

    x0 = 10;
    y0 = 40;
    H = 15;


    //Check that the node property file generate with the first program must contain informations
    if (bio2.size() == 8) {
        std::cout
                << "The node property file generated with the first program is empty, probably you have passed a bad string path"
                << std::endl;
        exit(-1);
    }


    BitDecoder decoder(&bio2);


    int nodeInfoLength = 0;

    std::cout << "node info length" << nodeInfoLength << std::endl;

    while (!bio2.empty()) {

        nodeInfoLength = stoi(bio2.pop_front().to_string(), nullptr, 2);

        string nodeInfo = "";

        for (int i = 0; i < nodeInfoLength; ++i) {
            nodeInfo += bio2.pop_front().to_string();
        }

//        std::cout << nodeInfo << std::endl;

        a = stoi(decoder.getNodeDepth(nodeInfo), nullptr, 2);
        b = stoi(decoder.getLb(nodeInfo), nullptr, 2);
        c = stoi(decoder.getRb(nodeInfo), nullptr, 2);

        edge = decoder.getEdge(nodeInfo);

        x = x0 + b;
        H = 15;
        y = y0 + (a * H);
        w = (c - b) + 1;

        std::cout << "\nBit Nodedepth: " << a << " [" << b << "-" << c << "]\n" << "Edge\t" << edge << std::endl;

        string temp = "\n<g class=\"func_g\" onmouseover=\"s(this)\" onmouseout=\"c()\" onclick=\"zoom(this)\">\n""<title>";
        temp += edge;
        temp += "</title><rect x=\"";
        temp += to_string(x);
        temp += "\" y=\"";
        temp += to_string(y);
        temp += "\" width=\"";
        temp += to_string(w);
        temp += "\" ""height=\"15.0\" fill=\"rgb(225,0,0)\" rx=\"2\" ry=\"2\" />\n""</g>";

        char str[temp.length()];
        strcpy(str, temp.c_str());

        svg_out << str;
    }


    char svgEnd[] = {"</svg>"};  //Close the SVG File
    svg_out << svgEnd;


    bin_in.close();     //Close the input file
    svg_out.close();    //chiudo il file on output
};
