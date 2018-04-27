//
// Created by root on 4/23/18.
//
#include <string>
#include <bitset>

#ifndef VISUALSUFFIXTREE_BITENCODE_H
#define VISUALSUFFIXTREE_BITENCODE_H

using namespace std;

class BitEncode {

public:


    BitEncode();

    string nodeDepthToString(unsigned long s);

    string depthToString(unsigned long s);

    string lbToString(unsigned long s) ;

    string rbToString(unsigned long s);

    string edgeToString(string *s);

    string getEdgeLength(unsigned long s);

        /*
         * In this method I create an Header file to put in the binary file with the bit indication for decoder
         */
    string createHeader();

    int charEncoding(string inputLine, vector<string> *charCoding);

private:
    int bitCharEncoding;
    vector <string> charCoding;
    string encodeChar(char c, string *alphabet);

};

#endif //VISUALSUFFIXTREE_BITENCODE_H
