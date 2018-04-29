//
// Created by root on 4/28/18.
//

#include <bitset>
#include <vector>
#include <iostream>
#include <string>
#include "../Include/NodeInfo.h"

using namespace std;

const int bitDepth = 16;
const int bitNodeDepth = 16;
const int bitLb = 16;
const int bitRb = 16;
const int bitEdgeLength = 16;
const int bitEdgeCharacterEncoding = 16;

NodeInfo::NodeInfo(NodeInfoStructure * nodeInfoStructure) {
    //Save the address oh the structure to avoid multiple copies
    this->infoStructure = nodeInfoStructure;

    setEdgeCharacterEncoding(3);
}

string NodeInfo::getNodeField() {
    string temp = "";

    temp.append(depth);
    temp.append(nodeDepth);
    temp.append(lb);
    temp.append(rb);
    temp.append(edgeLength);
//    temp.append(edgeCharacterEncoding); questo non serve metterlo va solo nell'header

    temp.append(edge);
    return temp;
}

bool NodeInfo::setNodeField(string *nodeField) {

    setDepth(stoi(partitioner(nodeField, 0, 15), nullptr, 2));
    setNodeDepth(stoi(partitioner(nodeField, 16, 31), nullptr, 2));
    setLb(stoi(partitioner(nodeField, 32, 47), nullptr, 2));
    setRb(stoi(partitioner(nodeField, 48, 63), nullptr, 2));
    setEdgeLength(stoi(partitioner(nodeField, 64, 79), nullptr, 2));

    int edgeFrom = 80;
    int edgeTo = 80 + getEdgeCharacterEncoding()*getEdgeLength() -1;
    edge = partitioner(nodeField, edgeFrom, edgeTo);
    setBinaryEdge(&edge);

    //todo add edge e lunghezza edge
    //todo implement il resto
}

void NodeInfo::setDepth(unsigned long n) {
    depth = std::bitset<bitDepth>(n).to_string();
}

void NodeInfo::setNodeDepth(unsigned long n) {
    nodeDepth = std::bitset<bitNodeDepth>(n).to_string();
}

void NodeInfo::setLb(unsigned long n) {
    lb = std::bitset<bitLb>(n).to_string();
}

void NodeInfo::setRb(unsigned long n) {
    rb = std::bitset<bitRb>(n).to_string();
}

void NodeInfo::setEdgeLength(unsigned long n) {
    edgeLength = std::bitset<bitEdgeLength>(n).to_string();
}

//codifica l'edge da stringa a binario
void NodeInfo::setEdge(string *s) {
    edgeLength = std::bitset<bitEdgeLength>(s->size()).to_string();
    string character;
    string edge = "";
    for (int i = 0; i < s->size(); i++) {
        character = "";
        character += s->at(i);
        edge += encodeCharacter(&character, &codification, &alphabet);
    }
    this->edge = edge;
}

void NodeInfo::setEdgeCharacterEncoding(unsigned long n) {
    edgeCharacterEncoding = std::bitset<bitEdgeCharacterEncoding>(n).to_string();
}



int NodeInfo::getDepth() {
    return stoi(depth, nullptr, 2);;
}

int NodeInfo::getNodeDepth() {
    return stoi(nodeDepth, nullptr, 2);
}

int NodeInfo::getLb() {
    return stoi(lb, nullptr, 2);;
}

int NodeInfo::getRb() {
    return stoi(rb, nullptr, 2);
}

int NodeInfo::getEdgeLength() {
    return stoi(edgeLength, nullptr, 2);
}

int NodeInfo::getEdgeCharacterEncoding() {
    return stoi(edgeCharacterEncoding, nullptr, 2);
}

string NodeInfo::getEdgeDecoded() {
    int bitChar = stoi(edgeCharacterEncoding, nullptr, 2);
    string character;
    string edge = "";
    for (int j = 0; j < stoi(edgeLength, nullptr, 2); j++) {
        character = "";
        for (int i = 0+j*bitChar; i < ((j+1)*bitChar); i++) {
            character+=this->edge[i];
        }
        edge += decodeCharacter(&character, &codification, &alphabet);
    }

    return edge;
}

string NodeInfo::print() {
    string s;
    s.append("\nNodeDepth:   " + to_string(getNodeDepth()));
    s.append("\nDepth:       " + to_string(getDepth()));
    s.append("\nLb-Rb:       [" + to_string(getLb()) + "-" + to_string(getRb()) + "]");
    s.append("\nEdge Length: "+to_string(getEdgeLength()));
    s.append("\nEdge:        "+getEdgeDecoded());
    //todo to complete

    return s;
}


string NodeInfo::partitioner(string *s, int from, int to) {

    string a = "";
    for (int i = from; i <= to; i++) {
        a += s->at(i);
    }
    return a;
}

string NodeInfo::encodeCharacter(string *s, vector <string> *codification, vector <string> *alphabet) {

    for (int i = 0; i < alphabet->size(); i++) {
        string  a = alphabet->at(i);
        if (a.compare(*s) == 0){
            return codification->at(i);
        }
    }

    std::cout << "Errore in charEncoding, carattere: "<< s << " non trovato!" << std::endl; //
    exit(12);
}

string NodeInfo::decodeCharacter(string *s, vector<string> *codification, vector<string> *alphabet) {

    for (int i = 0; i < alphabet->size(); i++) {
        string  a = codification->at(i);
        if (a.compare(*s) == 0){
            return alphabet->at(i);
        }
    }

    std::cout << "Errore in charDecoding, carattere: "<< s << " non trovato!" << std::endl; //
    exit(12);
}

void NodeInfo::setBinaryEdge(string *s) {
    edge = *s;
}


