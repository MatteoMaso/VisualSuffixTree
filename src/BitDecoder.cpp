//
// Created by root on 4/25/18.
//

#include "../Include/BitDecoder.h"

BitDecoder::BitDecoder(BitIo<16> *b) {
    initializeHeader(b, coding, 10);
}

static const int PARAMETER_NUMBER = 10;

int BitDecoder::getNodeInfoLength() {

    int nodeInfoDim = 0;

    for (int i = 2; i < PARAMETER_NUMBER; ++i) {
        nodeInfoDim += coding[i];
    }

    if ((nodeInfoDim % 16) == 0) {
        return nodeInfoDim / 16;
    } else {
        return nodeInfoDim / 16 + 1;
    }
}

//todo passare la stringa per riferimento
string BitDecoder::partitioner(string s, int element) {

    int from = 0;
    for (int j = 2; j < element - 1; j++) {
        from += coding[j];
    }

    int to = from + coding[element - 1] - 1;
    string a = "";
    for (int i = from; i <= to; ++i) {
        a += s[i];
    }
    return a;
}

string BitDecoder::getNodeDepth(string nodeInfo) {
    return partitioner(nodeInfo, BITNODEDEPTH);
}

string BitDecoder::getDepth(string nodeInfo) {
    return partitioner(nodeInfo, BITDEPTH);
}

string BitDecoder::getLb(string nodeInfo) {
    return partitioner(nodeInfo, BITLB);
}

string BitDecoder::getRb(string nodeInfo) {
    return partitioner(nodeInfo, BITRB);
}

string BitDecoder::getEdge(string nodeInfo) {

    //Poi se so quanti caratteri ho posso leggerli
    int numberOfCharacter = stoi(partitioner(nodeInfo, BITEDGELENGTH), nullptr, 2);
    return edgeStringExtractor(numberOfCharacter, nodeInfo);
}

string BitDecoder::edgeStringExtractor(int cNumber, string nodeInfo) {

    int from = 0;
    for (int j = 2; j < BITEDGELENGTH; j++) {
        from += coding[j];
    }

    int pointer = from;
    string edge = "";
    string c = "";
    for (int k = 1; k <= cNumber; k++) { //Read cNumberOfCharacter

        c = "";
        for (int i = 0; i < coding[BITCHARREPRESENTATION - 1]; i++) {
            c += nodeInfo[pointer++];
        }
        edge += encodeCharacter(c);
    }
    return edge;
}

string BitDecoder::encodeCharacter(string c) {

    return "A"; //todo da implementare
}

bool BitDecoder::initializeHeader(BitIo<16> *bio2, int *coding, int dim) {
    //Header length number*16bit
    bool headerFinished = false;
    while (!headerFinished) {

        //Read header
        string temp = bio2->pop_front().to_string();
        if (temp.compare("1111111111111111") != 0) {
            std::cout << "Error in header reading" << std::endl;
            exit(-1);
        }

        temp = bio2->pop_front().to_string();
        int numberParameter = stoi(temp, nullptr, 2);
        for (int k = 0; k < numberParameter; ++k) {
            temp = bio2->pop_front().to_string();
            coding[k] = stoi(temp, nullptr, 2);
            std::cout << stoi(temp, nullptr, 2) << std::endl; //capire come mai non converte il binario in intero
        }

        std::cout << "numero di parametri: " << numberParameter << std::endl;

        temp = bio2->pop_front().to_string();
        if (temp.compare("1111111111111111") == 0) {
            headerFinished = true;
            std::cout << "Header read correctly" << std::endl;

        } else {
            std::cout << "Error in header end reading" << std::endl;
            exit(-1);
        }

        return true;

    }
}