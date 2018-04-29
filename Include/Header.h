//
// Created by root on 4/29/18.
//
#include <string>

#include "../Include/NodeInfoStructure.h"
#include "BitIo.h"


#ifndef VISUALSUFFIXTREE_HEADER_H
#define VISUALSUFFIXTREE_HEADER_H

using namespace std;

class Header{

public:
    Header();

    Header(NodeInfoStructure * nodeInfoStructure);

//Passo lo stream del file a cui deve andare a leggere l'header
    bool readHeader(BitIo<16> * bio2);

    string getNodeInfoStructure();

//ritorna in stringa l'header che va messo nel file binario
    string getString();

    string INITHEADER = "1111111111111111";
    string ENDHEADER = "1111111111111111";

    int nodeInfoStructure_length;
    string nodeInfoStructure;

};

#endif //VISUALSUFFIXTREE_HEADER_H
