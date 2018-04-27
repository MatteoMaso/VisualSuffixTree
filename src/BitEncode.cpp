//
// Created by root on 4/25/18.
//

#include <vector>
#include <iostream>
#include "../Include/BitEncode.h"

BitEncode::BitEncode() {

    string alphabet = "acgt$-";
    int charBitEncoding = charEncoding(alphabet, &charCoding);
    bitCharEncoding = charBitEncoding;
}

using namespace std;

const int bitNodeDepth = 32;
const int bitDepth = 32;
const int bitLb = 32;
const int bitRb = 32;
const int bitEdgeLength = 32;
const int bitForStrLengh = 16;

int bitCharEncoding = 0;

vector <string> charCoding;

int BitEncode::charEncoding(string inputLine, vector<string> *charCoding){
    int length = 0;
    if (inputLine.length() <= 2){
        //1 bit
        charCoding->emplace_back("0");
        charCoding->emplace_back("1");
        length = 1;
    } else if ( inputLine.length() > 2 && inputLine.length() <= 4){
        //2 bit
        charCoding->emplace_back("00");
        charCoding->emplace_back("01");
        charCoding->emplace_back("10");
        charCoding->emplace_back("11");
        length = 2;
    } else if ( inputLine.length() > 4 && inputLine.length() <= 8){
        //3 bit
        charCoding->emplace_back("000");
        charCoding->emplace_back("001");
        charCoding->emplace_back("010");
        charCoding->emplace_back("011");
        charCoding->emplace_back("100");
        charCoding->emplace_back("101");
        charCoding->emplace_back("110");
        charCoding->emplace_back("111");
        length = 3;

    } else {
        std::cout << "To much alphabet character! Todo implement" << std::endl; //
        exit(1);
    }

    for (int j = 0; j < inputLine.length(); j++) {
        std::cout << "Char: " << inputLine[j] << " Encoded: " << charCoding->at(j) << std::endl; //
    }

    return length;
}


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

//Codifica la stringa in bit dall'edge usando la codifica dei caratteri nuova
string BitEncode::edgeToString(string *s) {
    string temp = "";
    string alphabet2 = "acgt$-";
    for (int k = 0; k < s->size(); k++) {
        temp += encodeChar(s->at(k), &alphabet2);
    }
    return temp;
}

string BitEncode::encodeChar(char c, string *alphabet){

    int max = (*alphabet).size();
    string temp = "";
    for (int i = 0; i < max; i++) {
        char  c1 = alphabet->at(i);
        if ( c1 == c ){
            temp = charCoding.at(i);
            return temp;
        }
    }

    std::cout << "Probably you have wrong the alphabet!!" << std::endl;
    exit(26);
}

string BitEncode::getEdgeLength(unsigned long s){
    return std::bitset<bitEdgeLength>(s).to_string();
}

/*
 * In this method I create an Header file to put in the binary file with the bit indication for decoder
 */
string BitEncode::createHeader() {
    //Ogni 8 bit salvo il numero dei bit che userò nella codifica

    string header = "1111111111111111"; //header start
    header += std::bitset<16>(7).to_string(); //devo mettere il numero di parametri successivi
    header += std::bitset<16>(bitForStrLengh).to_string(); //Questi bit li estraggo prima di passare l'informazione
    header += std::bitset<16>(bitCharEncoding).to_string();

    header += std::bitset<16>(bitNodeDepth).to_string();
    header += std::bitset<16>(bitDepth).to_string();
    header += std::bitset<16>(bitLb).to_string();
    header += std::bitset<16>(bitRb).to_string();
    header += std::bitset<16>(bitEdgeLength).to_string();
    header += "1111111111111111"; //header end

    return header;
}

