//
// Created by root on 4/23/18.
//
#include <string>
#include <bitset>
#include "BitIo.h"

#ifndef VISUALSUFFIXTREE_BITDECODER_H
#define VISUALSUFFIXTREE_BITDECODER_H

using namespace std;

class BitDecoder {

public:

    BitDecoder(BitIo<16> *b);

    int getNodeInfoLength();

    string getNodeDepth(string nodeInfo);

    string getDepth(string nodeInfo);

    string getLb(string nodeInfo);

    string getRb(string nodeInfo);

    string getEdge(string nodeInfo);

    string edgeStringExtractor(int cNumber, string nodeInfo);

private:

    static const int PARAMETER_NUMBER = 10;

    int coding[PARAMETER_NUMBER] = {0, 0, 0, 0, 0, 0, 0, 0};

    const int BITSTRINGLENGTH = 1;
    const int BITCHARREPRESENTATION = 2;
    const int BITNODEDEPTH = 3; //fino a 1024
    const int BITDEPTH = 4;
    const int BITLB = 5;
    const int BITRB = 6;
    const int BITEDGELENGTH = 7;

    string partitioner(string s, int element);

    bool initializeHeader(BitIo<16> *bio2, int *coding, int dim);

    string encodeCharacter(string c);


};

#endif //VISUALSUFFIXTREE_BITDECODER_H
