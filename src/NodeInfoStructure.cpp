//
// Created by root on 4/28/18.
//

#include <iostream>
#include "../Include/NodeInfoStructure.h"
#include <math.h>       /* log10 */
#include <sstream>
#include "../Include/ConfigParser.h"


//For decoder
NodeInfoStructure::NodeInfoStructure(int parameter[], map<string, string> *configParameter) {

    //NUMBER OF BIT FOR EACH FIELD REPRESENTATION
    //il decoder deve settare questi da config parameter penso
    bitDepth = stoi(configParameter->at("bitDepth"));
    bitNodeDepth = stoi(configParameter->at("bitNodeDepth"));
    bitLb = stoi(configParameter->at("bitLb"));
    bitRb = stoi(configParameter->at("bitRb"));
    bitLabel = stoi(configParameter->at("bitLabel"));
    bitFatherLabel = stoi(configParameter->at("bitFatherLabel"));
    bitEdgeLength = stoi(configParameter->at("bitEdgeLength"));
    bitEdgeCharacterEncoding = stoi(configParameter->at("bitEdgeCharacterEncoding"));
    bitNumberOfChildren = stoi(configParameter->at("bitNumberOfChildren"));
    bitChildrenId = stoi(configParameter->at("bitChildrenId")); //se lo metto variabile devo cambiare sotto

    for (int i = 0; i < PARAMETER_NUMBER; i++) {
        this->parameter[i] = parameter[i];
    }

    setAlphabet(configParameter->at("ALPHABET"));
}

//For encoder
NodeInfoStructure::NodeInfoStructure(map<string, string> *configParameter) {

    //Dovrei leggerli dall'header
    //todo
    bitDepth=16;
    bitNodeDepth=16;
    bitLb=16;
    bitRb=16;
    bitLabel=16;
    bitFatherLabel=16;
    bitEdgeLength=16;
    bitEdgeCharacterEncoding=16;
    bitNumberOfChildren=16;
    bitChildrenId=16;


    parameter[INDEX_DEPTH] = 16;
    parameter[INDEX_NODEDEPTH] = 16;
    parameter[INDEX_LB] = 16;
    parameter[INDEX_RB] = 16;
    parameter[INDEX_EDGELENGTH] = 16;
    parameter[INDEX_EDGECHARACTERENCODING] = 16;

    setAlphabet(configParameter->at("ALPHABET"));
}

string NodeInfoStructure::getString() {
    string temp;
    for (int i = 0; i < PARAMETER_NUMBER; ++i) {
        temp += std::bitset<16>(parameter[i]).to_string();
    }

    return temp;
}

//gli passo la stringa e lui inizializza i parametri, serve al decoder
bool NodeInfoStructure::setField(string headerInfo) {
    for (int i = 0; i < PARAMETER_NUMBER; i++) {
        string temp = "";
        for (int j = 0 + i * 16; j < 16 + i * 16; j++) {
            temp += headerInfo.at(j);
        }
        parameter[i] = stoi(temp, nullptr, 2);
    }
}

void NodeInfoStructure::setAlphabet(string alphabetString) {

    if (alphabetString.length() <= 0 || (alphabetString.length() % 2) == 0) { //If length is <= 0 or if it is even
        std::cout << "Error in alphabet string length: " << alphabetString.length() << std::endl;
        exit(-1);
    }

    int numberOfElement = (alphabetString.length() / 2) + 1;

    int numberOfBit = log10(numberOfElement) / log10(2) + 0.999999999;
    if (numberOfBit == 0) numberOfBit++;

    stringstream ss(alphabetString);
    string s;
    while (getline(ss, s, ',')) {
        this->alphabet.push_back(s);
    }


    switch (numberOfBit) {
        case 1:
            for (int i = 0; i < numberOfElement; i++) { this->codification.push_back(std::bitset<1>(i).to_string()); }
            break;

        case 2:
            for (int i = 0; i < numberOfElement; i++) { this->codification.push_back(std::bitset<2>(i).to_string()); }
            break;

        case 3:
            for (int i = 0; i < numberOfElement; i++) { this->codification.push_back(std::bitset<3>(i).to_string()); }
            break;

        case 4:
            for (int i = 0; i < numberOfElement; i++) { this->codification.push_back(std::bitset<4>(i).to_string()); }
            break;

        case 5:
            for (int i = 0; i < numberOfElement; i++) { this->codification.push_back(std::bitset<5>(i).to_string()); }
            break;

        default:
            for (int i = 0; i < numberOfElement; i++) { this->codification.push_back(std::bitset<16>(i).to_string()); }
            break;
    }
}


