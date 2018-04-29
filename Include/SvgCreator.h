//
// Created by root on 4/29/18.
//

#ifndef VISUALSUFFIXTREE_SVGCREATOR_H
#define VISUALSUFFIXTREE_SVGCREATOR_H

#include "BitIo.h"

class SvgCreator{

public:
    SvgCreator(char *inputFileName, char *outputFile);

    void openFile(std::ifstream *bin_in, char *inputFileName, BitIo<16> *bio);

    string readNextNodeInfo(BitIo<16> *bio);

    string getHeader(string fileName);
};

#endif //VISUALSUFFIXTREE_SVGCREATOR_H
