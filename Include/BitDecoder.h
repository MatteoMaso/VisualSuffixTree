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


private:

    static const int PARAMETER_NUMBER = 10;

    int coding[PARAMETER_NUMBER] = {0, 0, 0, 0, 0};

    int bitNodeDepth = 1; //fino a 1024
    int bitDepth = 2;
    int bitLb = 3;
    int bitRb = 4;
    int bitCharRepresentation = 5;

    string partitioner(string s, int element);

    bool initializeHeader(BitIo<16> *bio2, int *coding, int dim);

};

#endif //VISUALSUFFIXTREE_BITDECODER_H
