//
// Created by root on 4/29/18.
//

#ifndef VISUALSUFFIXTREE_SVGCREATOR_H
#define VISUALSUFFIXTREE_SVGCREATOR_H

#include "BitIo.h"
#include "ConfigParser.h"
#include "SvgUtils.h"
#include "NodeInfoStructure.h"


class SvgCreator{

public:



    SvgCreator(char *inputFileName, char *outputFile, map<string, string> *configParameter);

    void openFile(std::ifstream *bin_in, char *inputFileName, BitIo<16> *bio);

    string readNextNodeInfo(BitIo<16> *bio);

private:

    bool checkConfigParameter(map<string, string> *configParameter, NodeInfoStructure * nodeInfoStructure);
};

#endif //VISUALSUFFIXTREE_SVGCREATOR_H
