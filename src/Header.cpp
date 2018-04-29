//
// Created by root on 4/28/18.
//
#include <string>
#include <iostream>
#include <bitset>

#include "../Include/Header.h"
#include "../Include/NodeInfoStructure.h"

using namespace std;

Header::Header() {
    //todo
    //header usato nel svg
}

Header::Header(NodeInfoStructure * nodeInfoStructure) {
    //Header used into the data program
    this->nodeInfoStructure = nodeInfoStructure->getString();

    nodeInfoStructure_length = (this->nodeInfoStructure).size() / 16;
    if (((this->nodeInfoStructure).size() % 16) != 0) nodeInfoStructure_length++;

}

//Passo lo stream del file a cui deve andare a leggere l'header
bool Header::readHeader(BitIo<16> * bio2){

    //Read header
    string temp = bio2->pop_front().to_string();
    if (temp.compare(INITHEADER) != 0) {
        std::cout << "Error in header reading" << std::endl;
        exit(-1);
    }

    nodeInfoStructure_length = stoi(bio2->pop_front().to_string(), nullptr, 2);
    temp = "";
    for (int k = 0; k < nodeInfoStructure_length; ++k) {
        temp += bio2->pop_front().to_string();
    }
    nodeInfoStructure = temp;

    temp = bio2->pop_front().to_string();
    if (temp.compare(ENDHEADER) == 0) {
        std::cout << "Header read correctly" << std::endl;
    } else {
        std::cout << "Error in header end reading" << std::endl;
        exit(-1);
    }

    return true;
}

string Header::getNodeInfoStructure(){
    return this->nodeInfoStructure;
}

//ritorna in stringa l'header che va messo nel file binario
string Header::getString(){

    //warning se modifico qui devo modificare anche il readHeader sopra!!!!!!!!!!!!!!!!!!!!!!!!!
    string header = "";
    header += INITHEADER;
    header += std::bitset<16>(nodeInfoStructure_length).to_string(); //devo mettere il numero di parametri successivi
    header += nodeInfoStructure;
    header += ENDHEADER; //header end

    return header;
}
