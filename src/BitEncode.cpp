//
// Created by root on 4/25/18.
//

#include "../Include/BitEncode.h"

using namespace std;

const int bitNodeDepth = 64;
const int bitDepth = 64;
const int bitLb = 32;
const int bitRb = 32;


string BitEncode::nodeDepthToString(unsigned long s) {
    return std::bitset<bitNodeDepth>(s).to_string();
}

string BitEncode::depthToString(unsigned long s) {
    return std::bitset<bitDepth>(s).to_string();
}

string BitEncode::lbToString(unsigned long s) {
    return std::bitset<bitLb>(s).to_string();
}

string BitEncode::rbToString(unsigned long s) {
    return std::bitset<bitRb>(s).to_string();
}

/*
 * In this method I create an Header file to put in the binary file with the bit indication for decoder
 */
string BitEncode::createHeader() {
    //Ogni 8 bit salvo il numero dei bit che userò nella codifica

    string header = "1111111111111111"; //header start
    header += std::bitset<16>(4).to_string(); //devo mettere il numero di parametri successivi
    header += std::bitset<16>(bitNodeDepth).to_string();
    header += std::bitset<16>(bitDepth).to_string();
    header += std::bitset<16>(bitLb).to_string();
    header += std::bitset<16>(bitRb).to_string();
    header += "1111111111111111"; //header end

    return header;
}

