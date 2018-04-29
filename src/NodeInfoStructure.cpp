//
// Created by root on 4/28/18.
//

#include "../Include/NodeInfoStructure.h"

//For decoder
NodeInfoStructure::NodeInfoStructure(int parameter1[]) {
    for (int i = 0; i <  PARAMETER_NUMBER; i++) {
        parameter[i] = parameter1[i];
    }
}

//FOr encoder
NodeInfoStructure::NodeInfoStructure() {

    parameter[INDEX_DEPTH] = 16;
    parameter[INDEX_NODEDEPTH] = 16;
    parameter[INDEX_LB] = 16;
    parameter[INDEX_RB] = 16;
    parameter[INDEX_EDGELENGTH] = 16;
    parameter[INDEX_EDGECHARACTERENCODING] = 16;
}

string NodeInfoStructure::getString() {
    string temp;
    for (int i = 0; i <  PARAMETER_NUMBER; ++i) {
        temp += std::bitset<16>(parameter[i]).to_string();
    }

    return temp;
}

//gli passo la stringa e lui inizializza i parametri, serve al decoder
bool NodeInfoStructure::setField(string headerInfo) {
    for (int i = 0; i <  PARAMETER_NUMBER; i++) {
        string temp = "";
        for (int j = 0+i*16; j < 16 + i*16; j++) {
            temp += headerInfo.at(j);
        }
        parameter[i] = stoi(temp, nullptr, 2);
    }
}