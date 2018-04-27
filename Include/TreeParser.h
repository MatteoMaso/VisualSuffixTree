//
// Created by root on 4/16/18.
//

#ifndef VISUALSUFFIXTREE_TREEPARSER_H
#define VISUALSUFFIXTREE_TREEPARSER_H

using namespace std;

class TreeParser {

public:
    TreeParser(char *inputFileName, char *outputFileName);

    void printBinFile(string &s, std::ofstream &bin_out);

    string charEncoding(char &c, vector <string> &a, string &inputLine);



};


#endif //VISUALSUFFIXTREE_TREEPARSER_H
