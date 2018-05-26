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

class NodeInfoStructure {

public:

    NodeInfoStructure(map<string, string> *configParameter, char *inputFileName);

    //For encoder
    NodeInfoStructure(string headerInfo, map<string, string> *configParameter, char *stringFileName);

    string getString();

    //gli passo la stringa e lui inizializza i parametri, serve al decoder
    bool setField(string headerInfo);

    static const int PARAMETER_NUMBER = 11;
    int parameter[PARAMETER_NUMBER] = {};

    const int INDEX_BIT_DEPTH = 0;
    const int INDEX_BIT_NODEDEPTH = 1;
    const int INDEX_BIT_LB = 2;
    const int INDEX_BIT_RB = 3;
    const int INDEX_BIT_LABEL = 4;
    const int INDEX_BIT_FATHERLABEL = 5;
    const int INDEX_BIT_EDGELENGTH = 6;
    const int INDEX_BIT_EDGECHARACTERENCODING = 7;
    const int INDEX_BIT_NUMBEROFCHILDREN = 8;
    const int INDEX_BIT_CHILDRENID = 9; //se lo metto variabile devo cambiare sotto
    const int INDEX_BIT_NUMBEROFWL = 10;
    //IF ADD OTHER PARAMETER INCREMENT THE NUMBER OF PARAMETER_NUMBER

    vector<string> alphabet;
    vector<string> codification;

    //true se l'informazione è contenuta false altrimenti
    bool OPT_DEPTH;
    bool OPT_NODEDEPTH;
    bool OPT_LB;
    bool OPT_RB;
    bool OPT_LABEL;
    bool OPT_FATHERLABLE;
    bool OPT_EDGEINFO; //comprende la edge length e la edge characterEncoding
    bool OPT_CHILDREN_INFO;

    int getBitDepth() {
        return this->parameter[INDEX_BIT_DEPTH];
    }

    int getBitNodeDepth() {
        return this->parameter[INDEX_BIT_NODEDEPTH];
    }

    int getBitLb() {
        return this->parameter[INDEX_BIT_LB];
    }

    int getBitRb() {
        return this->parameter[INDEX_BIT_RB];
    }

    int getBitLabel() {
        return this->parameter[INDEX_BIT_LABEL];
    }

    int getBitFatherLabel() {
        return this->parameter[INDEX_BIT_FATHERLABEL];
    }

    int getBitEdgeLength() {
        return this->parameter[INDEX_BIT_EDGELENGTH];
    }

    int getBitEdgeCharacterEncoding() {
        return this->parameter[INDEX_BIT_EDGECHARACTERENCODING];
    }

    int getBitNumberOfChildren() {
        return this->parameter[INDEX_BIT_NUMBEROFCHILDREN];
    }

    int getBitChildrenId() {
        return this->parameter[INDEX_BIT_CHILDRENID];
    }

    int getBitNumberOfWinerLink() {
        return this->parameter[INDEX_BIT_NUMBEROFWL];
    }

private:

    string getAlphabet(char *inputFileName);

    long getStringLength(char * inputFileName);

    void setAlphabet(string alphabetString);

};


#endif //VISUALSUFFIXTREE_NODEINFOSTRUCTURE_H
