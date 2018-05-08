//
// Created by root on 4/29/18.
//
#include <string>
#include <bitset>
#include <map>
#include <vector>
#include "ConfigParser.h"


#ifndef VISUALSUFFIXTREE_NODEINFOSTRUCTURE_H
#define VISUALSUFFIXTREE_NODEINFOSTRUCTURE_H

using namespace std;

class NodeInfoStructure{

public:

    int bitDepth;
    int bitNodeDepth;
    int bitLb;
    int bitRb;
    int bitLabel;
    int bitFatherLabel;
    int bitEdgeLength;
    int bitEdgeCharacterEncoding;
    int bitNumberOfChildren;
    int bitChildrenId; //se lo metto variabile devo cambiare sotto

    NodeInfoStructure(int parameter1[], map<string, string> *configParameter);

    //For encoder
    NodeInfoStructure(map<string, string> *configParameter);

    string getString();

    //gli passo la stringa e lui inizializza i parametri, serve al decoder
    bool setField(string headerInfo);

    int parameter[20] = {16, 16, 16, 16, 16, 16, 16, 16, 16};

    const int INDEX_DEPTH = 0;
    const int INDEX_NODEDEPTH = 1;
    const int INDEX_LB = 2;
    const int INDEX_RB = 3;
    const int INDEX_EDGELENGTH = 4;
    const int INDEX_EDGECHARACTERENCODING = 5;

    const int PARAMETER_NUMBER = 7;

    vector <string> alphabet;
    vector <string> codification;

private:

    void setAlphabet(string alphabetString);

};


#endif //VISUALSUFFIXTREE_NODEINFOSTRUCTURE_H
