//
// Created by root on 4/29/18.
//
#include <string>
#include "NodeInfo.h"

#ifndef VISUALSUFFIXTREE_TREEPARSER_H
#define VISUALSUFFIXTREE_TREEPARSER_H

using namespace std;
using namespace sdsl;

typedef cst_sct3<> cst_t;

typedef cst_bfs_iterator<cst_t> iterator1;

class TreeParser {
public:



    bool VERBOSE;

    TreeParser(char *inputFileName, char *outputFileName, map<string, string> *configParameter);

    void printBinFile(string &s, std::ofstream &bin_out);

    void printNode(NodeInfo *nodeInfo, std::ofstream *bin_out);

    string getEdge(cst_t *cst, iterator1 *it);


    const cst_t::char_type a = 'a';
    const cst_t::char_type c = 'c';
    const cst_t::char_type g = 'g';
    const cst_t::char_type t = 't';
    const cst_t::char_type n = 'n';
    const cst_t::char_type b = 'b';

    const cst_t::char_type alphabet[6] = {a,c,g,t,b,n};
    //todo spostare e definire uguale all'alfabeto

private:

    bool checkNumberOfBit(int nBit, NodeInfoStructure *nodeInfoStructure);

//    vector<unsigned long> getWlVector(cst_t * cst, iterator *it, unsigned long rootId);


};

#endif //VISUALSUFFIXTREE_TREEPARSER_H
