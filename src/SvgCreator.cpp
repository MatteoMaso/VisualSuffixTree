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


using namespace std;

SvgCreator::SvgCreator(char *inputFileName, char *outputFile, map<string, string> *configParameter) {

    //READ BINARY FILE
    std::ifstream bin_in(inputFileName, std::ios::binary);
    BitIo<16> bio2;
    openFile(&bin_in,inputFileName, &bio2);


//    //HUMAN CONFIGURATIONS
//    map<string, string> configParameter;
//    ConfigParser cfPars("./Settings/config.cfg", &configParameter); //Initialize the configurations Parameter

    //PARAMETER CONFIGURATION
    Header header = Header();
    header.readHeader(&bio2);

    //After reading header create the NodeInfoStructure
    NodeInfoStructure nodeStructure = NodeInfoStructure();
    nodeStructure.setField(header.getNodeInfoStructure());


    //BEGIN SVG CREATOR
    std::ofstream svg_out(outputFile, std::ios::out | std::ios::binary);
    svg_out << getHeader("headerSvg.txt"); //Insert the header SVG into the file


    //PARAMETER THAT I NEED
    int a, b, c;
    int x, y, i, j, z, x0, y0, H, w;
    string edge = "";

    x0 = 10;
    y0 = 40;
    H = 15;


    int nodeInfoLength = 0;

    string nodeInfo = "";

    std::cout << "node info length" << nodeInfoLength << std::endl;

    NodeInfo nodeInfoObj(&nodeStructure);
    while (!bio2.empty()) {

        //READ AN OTHER NODE
        nodeInfo = readNextNodeInfo(&bio2);
        nodeInfoObj.setNodeField(&nodeInfo);

        std::cout << nodeInfoObj.print() << std::endl;


        a = nodeInfoObj.getNodeDepth();
        b = nodeInfoObj.getLb();
        c = nodeInfoObj.getRb();

        edge = nodeInfoObj.getEdgeDecoded();

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
    svg_out.close();    //chiudo il file on output*/
}




void SvgCreator::openFile(std::ifstream *bin_in, char *inputFileName, BitIo<16> *bio) {
//    std::ifstream bin_in(inputFileName, std::ios::binary);
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

    int e =  stoi((*bio).pop_front().to_string(), nullptr, 2);

    for (int i = 0; i < e; i++) {
        nodeInfo += (*bio).pop_front().to_string();
    }

    return nodeInfo;
}

string SvgCreator::getHeader(string fileName) {
    {

        string txt;
        string temp;
        ifstream file;
        file.open(fileName);

        if (file.is_open()) {
//        std::cout << "File open" << std::endl;

            while (!file.eof()) {
                getline(file, temp);
                txt += temp + "\n";
            }

        } else {
            std::cout << "I'm not able to read the header!" << std::endl;
        }

        file.close();
        return txt;
    }
}
