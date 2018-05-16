//
// Created by root on 4/28/18.
//

#include <iostream>
#include <math.h>       /* log10 */
#include <sstream>
#include <map>
#include <fstream>
#include "../Include/ConfigParser.h"
#include "../Include/NodeInfoStructure.h"


//FOR ENCODER
NodeInfoStructure::NodeInfoStructure(map<string, string> *configParameter, char *inputFileName) {

    long stringLength = getStringLength(inputFileName);
    string alphabet = getAlphabet(inputFileName);
    
    //NUMBER OF BIT FOR EACH FIELD REPRESENTATION
    //AQUIRE PARAMETER FROM CONFIG FILE
    this->parameter[INDEX_BIT_DEPTH] = stoi(configParameter->at("bitDepth"));
    this->parameter[INDEX_BIT_NODEDEPTH] = stoi(configParameter->at("bitNodeDepth"));
    this->parameter[INDEX_BIT_LB] = stoi(configParameter->at("bitLb"));
    this->parameter[INDEX_BIT_RB] = stoi(configParameter->at("bitRb"));
    this->parameter[INDEX_BIT_LABEL] = stoi(configParameter->at("bitLabel"));
    this->parameter[INDEX_BIT_FATHERLABEL] = stoi(configParameter->at("bitFatherLabel"));
    this->parameter[INDEX_BIT_EDGELENGTH] = stoi(configParameter->at("bitEdgeLength"));
    this->parameter[INDEX_BIT_EDGECHARACTERENCODING] = stoi(configParameter->at("bitEdgeCharacterEncoding"));
    this->parameter[INDEX_BIT_NUMBEROFCHILDREN] = stoi(configParameter->at("bitNumberOfChildren"));
    this->parameter[INDEX_BIT_CHILDRENID] = stoi(configParameter->at("bitChildrenId")); //se lo metto variabile devo cambiare sotto
//    this->parameter[INDEX_BIT_STRINGLENGTH] = stringLength;

    //AQUIRE THE INFO THAT I WANT TO REPRESENT FROM THE CONFIG FILE
    //standard information:
    this->OPT_DEPTH = true;
    this->OPT_NODEDEPTH = true;
    this->OPT_LB = true;
    this->OPT_RB = true;

    //LABEL INFO
    if (stoi(configParameter->at("OPT_LABEL")) == 1) {
        this->OPT_LABEL = true;
    } else {
        this->OPT_LABEL = false;
    }
    //FATHER LABEL INFO
    if (stoi(configParameter->at("OPT_FATHERLABLE")) == 1) {
        this->OPT_FATHERLABLE = true;
    } else {
        this->OPT_FATHERLABLE = false;
    }

    //ENGE LABEL INFO
    if (stoi(configParameter->at("OPT_EDGEINFO")) == 1) {
        this->OPT_EDGEINFO = true;
    } else {
        this->OPT_EDGEINFO = false;
    }

    //CHILDREN INFO
    if (stoi(configParameter->at("OPT_CHILDREN_INFO")) == 1) {
        this->OPT_CHILDREN_INFO = true;
    } else {
        this->OPT_CHILDREN_INFO = false;
    }

    setAlphabet(alphabet);
}

//FOR DECODER
NodeInfoStructure::NodeInfoStructure(string headerInfo, map<string, string> *configParameter) {

    setField(headerInfo);

    setAlphabet(configParameter->at("ALPHABET"));

}

int boolToInt(bool val){
    if (val){
        return 1;
    }

    return 0;
}

string NodeInfoStructure::getString() {
    string temp;
    for (int i = 0; i < PARAMETER_NUMBER; ++i) {
        temp += std::bitset<16>(parameter[i]).to_string();
    }

    temp += std::bitset<16>(boolToInt(OPT_DEPTH)).to_string();
    temp += std::bitset<16>(boolToInt(OPT_NODEDEPTH)).to_string();
    temp += std::bitset<16>(boolToInt(OPT_LB)).to_string();
    temp += std::bitset<16>(boolToInt(OPT_RB)).to_string();
    temp += std::bitset<16>(boolToInt(OPT_LABEL)).to_string();
    temp += std::bitset<16>(boolToInt(OPT_FATHERLABLE)).to_string();
    temp += std::bitset<16>(boolToInt(OPT_EDGEINFO)).to_string();
    temp += std::bitset<16>(boolToInt(OPT_CHILDREN_INFO)).to_string();

    return temp;
}

bool getParameter(string headerInfo, int from) {

    string temp = "";
    int parametro = from * 16;
    for (int i = parametro; i < parametro + 16; i++) {
        temp += headerInfo.at(i);
    }

    int x = stoi(temp);

    if (x == 1) {
        return true;
    }

    return false;
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

    int pippo = PARAMETER_NUMBER;

//    for (int k = 160; k < 2000; ++k) {
//        std::cout << headerInfo.at(k) << std::endl;
//    }

    OPT_DEPTH = getParameter(headerInfo, pippo++);
    OPT_NODEDEPTH = getParameter(headerInfo, pippo++);
    OPT_LB = getParameter(headerInfo, pippo++);
    OPT_RB = getParameter(headerInfo, pippo++);
    OPT_LABEL = getParameter(headerInfo, pippo++);
    OPT_FATHERLABLE = getParameter(headerInfo, pippo++);
    OPT_EDGEINFO = getParameter(headerInfo, pippo++);
    OPT_CHILDREN_INFO = getParameter(headerInfo, pippo++);

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

long NodeInfoStructure::getStringLength(char *inputFileName) {
    string txt;
    ifstream file(inputFileName);

    if (file.is_open())
        while (file.good())
            getline(file, txt);
    file.close();

    return txt.length();
}

bool contains(vector<string> * character, string c){
    for (int i = 0; i < character->size(); i++) {
        string s = character->at(i);
        if ( character->at(i).compare(c) == 0 ){
            return true;
        }
    }

    return false;
}

string NodeInfoStructure::getAlphabet(char *inputFileName) {

    string txt;
    ifstream file(inputFileName);

    if (file.is_open())
        while (file.good())
            getline(file, txt);
    file.close();
    
    vector<string> character = {};
    string c;
    for (int i = 0; i < txt.length(); i++) {
        c = txt[i];
        if ( !contains(&character, c) ){
            character.push_back(c);
        }
    }

    string alp = "$";

    for (int j = 0; j < character.size(); j++) {
        std::cout << character.at(j) <<std::endl;
        string ca = string(character.at(j));
        alp.append("," + character.at(j));
//        alp += "," + ;
    }
    
    return alp;
}











