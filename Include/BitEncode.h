//
// Created by root on 4/23/18.
//
#include <string>
#include <bitset>

#ifndef VISUALSUFFIXTREE_BITENCODE_H
#define VISUALSUFFIXTREE_BITENCODE_H

using namespace std;

const int bitNodeDepth = 10;
const int bitDepth = 10;
const int bitLb = 16;
const int bitRb = 16;

class BitEncode {

public:


    string nodeDepthToString(unsigned long s) {
        return std::bitset<bitNodeDepth>(s).to_string();
    }

    string depthToString(unsigned long s) {
        return std::bitset<bitDepth>(s).to_string();
    }

    string lbToString(unsigned long s) {
        return std::bitset<bitLb>(s).to_string();
    }

    string rbToString(unsigned long s) {
        return std::bitset<bitRb>(s).to_string();
    }


};


#endif //VISUALSUFFIXTREE_BITENCODE_H
