//
// Created by root on 4/29/18.
//
#include <string>
#include <bitset>


#ifndef VISUALSUFFIXTREE_NODEINFOSTRUCTURE_H
#define VISUALSUFFIXTREE_NODEINFOSTRUCTURE_H

using namespace std;

class NodeInfoStructure{

public:
    NodeInfoStructure(int parameter1[]);

    //For encoder
    NodeInfoStructure();

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

    NodeInfoStructure * nodeInfoStructure;

};


#endif //VISUALSUFFIXTREE_NODEINFOSTRUCTURE_H
