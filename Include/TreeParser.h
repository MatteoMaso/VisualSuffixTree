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
    TreeParser(char *inputFileName, char *outputFileName, map<string, string> *configParameter);

    void printBinFile(string &s, std::ofstream &bin_out);

    string charEncoding(char &c, vector<string> &a, string &inputLine);


    void printNode(NodeInfo *nodeInfo, std::ofstream *bin_out);


    string getEdge(cst_t *cst, iterator1 *it);

};

#endif //VISUALSUFFIXTREE_TREEPARSER_H
