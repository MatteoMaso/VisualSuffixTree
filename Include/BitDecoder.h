//
// Created by root on 4/23/18.
//
#include <string>
#include <bitset>

#ifndef VISUALSUFFIXTREE_BITDECODER_H
#define VISUALSUFFIXTREE_BITDECODER_H

using namespace std;

class BitDecoder {

public:

    BitDecoder() {
        coding[bitNodeDepth-1] = 10;
        coding[bitDepth-1] = 10;
        coding[bitLb-1] = 16;
        coding[bitRb-1] = 16;
        coding[bitCharRepresentation-1] = 2;
    }

    string getNodeDepth(string nodeInfo) {
        return partitioner(nodeInfo, bitNodeDepth);
    }

    string getDepth(string nodeInfo) {
        return partitioner(nodeInfo, bitDepth);
    }

    string getLb(string nodeInfo) {
        return partitioner(nodeInfo, bitLb);
    }

    string getRb(string nodeInfo) {
        return partitioner(nodeInfo, bitRb);
    }

    int coding[10] = {10 ,10, 16, 16, 2};
    //potrei renderlo più veloce salvandomi degli altri vettori

    int bitNodeDepth = 1; //fino a 1024
    int bitDepth = 2;
    int bitLb = 3;
    int bitRb = 4;
    int bitCharRepresentation = 5;

private:
    string partitioner(string s, int element){

        int from = 0;

        for (int j = 0; j < element - 1; j++) {
            from += coding[j];
        }

        int to = from + coding[element-1] - 1;

        string a = "";

        for (int i = from; i <= to; ++i) {
            a += s[i];
        }

        return a;
    }

};

#endif //VISUALSUFFIXTREE_BITDECODER_H
