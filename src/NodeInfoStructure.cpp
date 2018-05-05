//
// Created by root on 4/28/18.
//

#include <iostream>
#include "../Include/NodeInfoStructure.h"
#include <math.h>       /* log10 */
#include <sstream>

//For decoder
NodeInfoStructure::NodeInfoStructure(int parameter1[], map<string, string> *configParameter) {
    for (int i = 0; i < PARAMETER_NUMBER; i++) {
        parameter[i] = parameter1[i];
    }

    setAlphabet(configParameter->at("ALPHABET"));
}

//FOr encoder
NodeInfoStructure::NodeInfoStructure(map<string, string> *configParameter) {

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

    int numberOfElement = (alphabetString.length() / 2) +1;

    int numberOfBit = log10(numberOfElement)/log10(2) +  0.999999999;
    if (numberOfBit == 0) numberOfBit++;

    stringstream ss(alphabetString);
    string s;
    while (getline(ss, s, ',')) {
        this->alphabet.push_back(s);
    }

    //todo risolvere il passaggio dei bit

    for (int i = 0; i < numberOfElement; i++) {
        this->codification.push_back(std::bitset<3>(i).to_string());
    }

//    temp += std::bitset<16>(parameter[i]).to_string();
//
//    if (numberOfElement <= 2){
//
//    } else if ( numberOfElement > 2 && numberOfElement <= 4){
//
//    } else if ( numberOfElement > 4 && numberOfElement <= 8){
//
//    } else {
//        std::cout << "todo implement more character" << std::endl;
////        exit(15);
//    }

}
