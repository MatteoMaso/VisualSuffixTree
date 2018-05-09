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

    //BEGIN SVG CREATOR
    std::ofstream svg_out(outputFile, std::ios::out | std::ios::binary);
    svg_out << createSvgHeader(stoi(configParameter->at("WINDOW_WIDTH")), stoi(configParameter->at("WINDOW_HEIGHT")));

    //PARAMETER THAT I NEED
    map<int, ObjNode> hashmap;

    float a, b, c;
    int sons;
    int H = 15;//dovrà poi essere messa nel config e decisa dall'utente
    float rectWidth = stoi(configParameter->at("WINDOW_WIDTH")) - 20 ; //dovrà poi essere messa nel config e decisa dall'utente
    int count = 0;
    int defW = 500; //larghezza del rettangolo
    int fl, l;
    float i, j, z, x0, y0, w;
    double x, y;
    string edge = "";

    x0 = 10;
    y0 = stoi(configParameter->at("WINDOW_HEIGHT")) - 40;


    int nodeInfoLength = 0;

    string nodeInfo = "";

    std::cout << "node info length" << nodeInfoLength << std::endl;

    NodeInfo nodeInfoObj(&nodeStructure);

    float scaleUnit = 0;

    while (!bio2.empty()) {

        //READ AN OTHER NODE
        nodeInfo = readNextNodeInfo(&bio2);

        nodeInfoObj.setNodeField(&nodeInfo);

//        std::cout << nodeInfoObj.print() << std::endl;


        a = nodeInfoObj.getNodeDepth();
        b = nodeInfoObj.getLb();
        c = nodeInfoObj.getRb();

//        edge = nodeInfoObj.getEdgeDecoded();


        //se è la radice la disegno grande come il rettangolo
        if (a == 0) {
            w = rectWidth;
            x = x0;
            y = y0;
            scaleUnit = rectWidth / c;
        } else { //altrimenti scalo la larghezza per la larghezza del suffix interval
            if ((c - b) == 0){
                w = 0;
            } else{
                w = scaleUnit * (c-b);
            }
            x = x0 + b*scaleUnit;
            y = y0 - (a * H) - a*0.7;
        }

//        std::cout << "\nBit Nodedepth: " << a << " [" << b << "-" << c << "]\n" << "Edge\t" << edge << std::endl;

        string temp = "\n<g class=\"func_g\" onmouseover=\"s(this)\" onmouseout=\"c()\" onclick=\"zoom(this)\">\n""<title>";
        temp += to_string(nodeInfoObj.getLabel());
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



//    while (!bio2.empty()) {
//
//        //READ AN OTHER NODE
//        nodeInfo = readNextNodeInfo(&bio2);
//        nodeInfoObj.setNodeField(&nodeInfo);
//
//        std::cout << nodeInfoObj.print() << std::endl;
//
//
//        a = nodeInfoObj.getNodeDepth();
//        b = nodeInfoObj.getLb();
//        c = nodeInfoObj.getRb();
//        fl = nodeInfoObj.getFatherLabel();
//        l = nodeInfoObj.getLabel();
//
//        ObjNode objNode = ObjNode();
//        objNode.setObjNodeDepth(a);
//
//        //dovrei prendermi l'oggetto del padre forse è più fattibile ciclare due volte una votla per creare gli oggetti e l'altra per analizzarli!k
//
//
//
//        //Prende il numero di figli del padre che sono uguali al numero di fratelli escluso se stesso
//        count = hashmap[nodeInfoObj.getFatherLabel()].getNumberOfChildren() - 1;
//
//
//        //creo un array dei figli e li ordino in senso crescente della loro label
//
//        //        while(!bio2.empty()){ //QUI RESTA IN LOOP :(
////                   while(int  i<= count){
////                          if (a != 0 && nodeInfoObj.getNodeDepth() == a && nodeInfoObj.fatherLabel== fl){
////
////                              bros[i] = nodeInfoObj.getLabel();
////                               i++;
////              }
////        }
//
//
//        //count è il numero di fratelli
//        sons = count + 1;
//        //se non sto valutando il padre mi trovo le coordinate di del padre del nodo
//        if(a != 0){
//            ObjNode fatherObj = hashmap[fl];
////            fatherObj = hashmap.find(fl)->second;
//
//            int xF = fatherObj.getObjNodeX();
//            int yF = fatherObj.getObjNodeY();
//            int  wF = fatherObj.getObjNodeWid();
//            x = xF;
//            y = yF + H;
//            w =  wF/sons;
//        }
//
//        else{
//            x = x0 + b;
//            y = y0 + (a * H);
//            w = defW;
//        }
//
//
//
//
//        edge = nodeInfoObj.getEdgeDecoded();
//
//
//
//        H = 15;
//
//
//        objNode.setObjNodeX(x);
//        objNode.setObjNodeY(y);
//        objNode.setObjNodeWid(w);
//        objNode.setNumberOfChildren(nodeInfoObj.getNumbrOfChildren()); //Setta il numero di figli
//
//        pair<int, ObjNode> element = {l, objNode};
//        hashmap.insert(element);
//
//
////        std::cout << "\nBit Nodedepth: " << a << " [" << b << "-" << c << "]\n" << "Edge\t" << edge << std::endl;
//
//        string temp = "\n<g class=\"func_g\" onmouseover=\"s(this)\" onmouseout=\"c()\" onclick=\"zoom(this)\">\n""<title>";
//        temp += edge;
//        temp += "</title><rect x=\"";
//        temp += to_string(x);
//        temp += "\" y=\"";
//        temp += to_string(y);
//        temp += "\" width=\"";
//        temp += to_string(w);
//        temp += "\" ""height=\"15.0\" fill=\"rgb(225,0,0)\" rx=\"2\" ry=\"2\" />\n""</g>";
//
//        char str[temp.length()];
//        strcpy(str, temp.c_str());
//
//        svg_out << str;
//    }
//
//
//    char svgEnd[] = {"</svg>"};  //Close the SVG File
//    svg_out << svgEnd;
//
//
//    bin_in.close();     //Close the input file
//    svg_out.close();    //chiudo il file on output*/
//}




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

    int e = stoi((*bio).pop_front().to_string(), nullptr, 2);

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

string SvgCreator::getWindowsConfigurations(int width, int heigth) {

    string temp = "<rect x=\"0.0\" y=\"0\" width=\"" + to_string(width) + "\" height=\"" + to_string(heigth) + "\" fill=\"url(#background)\"/>";
    temp += "<text text-anchor=\"middle\" x=\"500\" y=\"24\" font-size=\"17\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\">Flame Graph</text>\n"
            "<text text-anchor=\"\" x=\"10.00\" y=\"24\" font-size=\"12\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\" id=\"unzoom\" onclick=\"unzoom()\" style=\"opacity: 0; cursor: pointer;\">Reset Zoom</text>\n"
            "<text text-anchor=\"\" x=\"890.00\" y=\"24\" font-size=\"12\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\" id=\"search\" onmouseover=\"searchover()\" onmouseout=\"searchout()\" onclick=\"search_prompt()\" style=\"opacity: 0.1; cursor: pointer;\">Search</text>\n"
            "<text text-anchor=\"\" x=\"1090.00\" y=\"1249\" font-size=\"12\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\" id=\"matched\"> </text>\n"
            "\n"
            "<text text-anchor=\"\" x=\"10\" y=\"609\" font-size=\"12\" font-family=\"Verdana\" fill=\"rgb(0,0,0)\" id=\"details\"> </text>";

    return temp;


}

string SvgCreator::createSvgHeader(int width, int heigth) {

    string temp = "<svg version=\"1.1\" width=\"" + to_string(width) + "\" height=\"" +to_string(heigth) + "\" onload=\"init(evt)\" viewBox=\"0 0 ";
    temp += to_string(width) + " " + to_string(heigth) + "\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">";
    temp += getHeader("headerSvg.txt"); //Insert the header SVG into the file

    temp += getWindowsConfigurations(width, heigth);

    return temp;
}
