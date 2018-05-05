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
const int bitLabel = 16;
const int bitFatherLabel = 16;

const int bitEdgeLength = 16;
const int bitEdgeCharacterEncoding = 16;

const int bitNumberOfChildren = 16;
const int bitChildrenId = 16; //se lo metto variabile devo cambiare sotto


NodeInfo::NodeInfo(NodeInfoStructure *nodeInfoStructure) {
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
    temp.append(label);
    temp.append(fatherLabel);
    temp.append(edgeLength);
//    temp.append(edgeCharacterEncoding); questo non serve metterlo va solo nell'header

    temp.append(numberOfChildren);

    temp.append(edge);
    if (this->getNumbrOfChildren() > 0) {
        temp.append(childrenToEncodedString(childrenId));
    }

    return temp;
}

bool NodeInfo::setNodeField(string *nodeField) {

    setDepth(stoi(partitioner(nodeField, 0, 15), nullptr, 2));
    setNodeDepth(stoi(partitioner(nodeField, 16, 31), nullptr, 2));
    setLb(stoi(partitioner(nodeField, 32, 47), nullptr, 2));
    setRb(stoi(partitioner(nodeField, 48, 63), nullptr, 2));
    setLabel(stoi(partitioner(nodeField, 64, 79), nullptr, 2));
    setFatherLabel(stoi(partitioner(nodeField, 80, 95), nullptr, 2));
    setEdgeLength(stoi(partitioner(nodeField, 96, 111), nullptr, 2));
    setNumberOfChildren(stoi(partitioner(nodeField, 112, 127), nullptr, 2));


    int edgeFrom = 128;     //Dopo l'ultimo parametro
    int edgeTo = edgeFrom + getEdgeCharacterEncoding() * getEdgeLength() - 1;
    edge = partitioner(nodeField, edgeFrom, edgeTo);
    setBinaryEdge(&edge);


    int childrenFrom = edgeTo + 1;
    int childrenTo = childrenFrom + 16 * getNumbrOfChildren() - 1;
    string t = partitioner(nodeField, childrenFrom, childrenTo);
    setChildren(&t);


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

void NodeInfo::setLabel(unsigned long n) {
    label = std::bitset<bitLabel>(n).to_string();
}

void NodeInfo::setFatherLabel(unsigned long n) {
    fatherLabel = std::bitset<bitFatherLabel>(n).to_string();
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
//        edge += encodeCharacter(&character, &codification, &alphabet);
        edge += encodeCharacter(&character, &(infoStructure->codification), &(infoStructure->alphabet));
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
        for (int i = 0 + j * bitChar; i < ((j + 1) * bitChar); i++) {
            character += this->edge[i];
        }
        edge += decodeCharacter(&character, &(infoStructure->codification), &(infoStructure->alphabet));
    }

    return edge;
}

string NodeInfo::print() {
    string s;
    s.append("\nLabel:            " + to_string(getLabel()));
    s.append("\nFather's Label:   " + to_string(getFatherLabel()));
    s.append("\nNodeDepth:        " + to_string(getNodeDepth()));
    s.append("\nDepth:            " + to_string(getDepth()));
    s.append("\nLb-Rb:            [" + to_string(getLb()) + "-" + to_string(getRb()) + "]");
    s.append("\nEdge Length:      " + to_string(getEdgeLength()));
    s.append("\nEdge:             " + getEdgeDecoded());
    s.append("\n#Children:         " + to_string(getNumbrOfChildren()));

    for (auto i : getChildrenId()) {
        s.append("\nChildrens:    " + to_string(i));
    }

    return s;
}


string NodeInfo::partitioner(string *s, int from, int to) {

    string a = "";
    for (int i = from; i <= to; i++) {
        a += s->at(i);
    }
    return a;
}

string NodeInfo::encodeCharacter(string * s, vector<string> * codification, vector<string> * alphabet) {

    for (int i = 0; i < alphabet->size(); i++) {
        string a = alphabet->at(i);
        if (a.compare(*s) == 0) {
            return codification->at(i);
        }
    }

    std::cout << "Errore in charEncoding, carattere: " << *s << " non trovato!" << std::endl; //
    exit(12);
}

string NodeInfo::decodeCharacter(string *s, vector<string> *codification, vector<string> *alphabet) {

    for (int i = 0; i < alphabet->size(); i++) {
        string a = codification->at(i);
        if (a.compare(*s) == 0) {
            return alphabet->at(i);
        }
    }

    std::cout << "Errore in charDecoding, carattere: " << s << " non trovato!" << std::endl; //
    exit(12);
}

void NodeInfo::setBinaryEdge(string *s) {
    edge = *s;
}

int NodeInfo::getLabel() {
    return stoi(label, nullptr, 2);
}

int NodeInfo::getFatherLabel() {
    return stoi(fatherLabel, nullptr, 2);
}

void NodeInfo::setNumberOfChildren(int n) {
    numberOfChildren = std::bitset<bitNumberOfChildren>(n).to_string();
}

void NodeInfo::setChildren(string *childrenString) {
    string id;
    childrenId.clear();
    for (int i = 0; i < childrenString->size() / 16; ++i) {
        id = "";
        for (int j = 0 + i * 16; j < ((i + 1) * 16); j++) {
            id += childrenString->at(j);
        }
        childrenId.push_back(stoi(id, nullptr, 2));
    }
}

void NodeInfo::setChildrenId(vector<int> *childrenId) {

    setNumberOfChildren(childrenId->size());
    this->childrenId.clear();
    for (auto i : *childrenId) {
        this->childrenId.push_back(i);
    }
}

int NodeInfo::getNumbrOfChildren() {
    return stoi(numberOfChildren, nullptr, 2);
}

vector<int> NodeInfo::getChildrenId() {
    return this->childrenId;
}

string NodeInfo::childrenToEncodedString(vector<int> v) {

    string tmp = "";

    for (auto i : v) {
        tmp.append(std::bitset<bitChildrenId>(i).to_string());
    }

    return tmp;
}


