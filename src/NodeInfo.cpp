//
// Created by root on 4/28/18.
//

#include <bitset>
#include <vector>
#include <iostream>
#include <string>
#include "../Include/NodeInfo.h"
#include "../Include/Utils.h"

using namespace std;

NodeInfo::NodeInfo(NodeInfoStructure *nodeInfoStructure) {
    //Save the address oh the structure to avoid multiple copies
    this->infoStructure = nodeInfoStructure;

    setEdgeCharacterEncoding(3);
}

string NodeInfo::getNodeField() {
    string temp = "";

    //Default
    temp.append(depth);
    temp.append(nodeDepth);
    temp.append(lb);
    temp.append(rb);

    if (infoStructure->OPT_LABEL) {
        temp.append(label);
    }

    if (infoStructure->OPT_FATHERLABLE) {
        temp.append(fatherLabel);
    }

    if (infoStructure->OPT_EDGEINFO) {
        temp.append(edgeLength);
        temp.append(edge);
        //    temp.append(edgeCharacterEncoding); questo non serve metterlo va solo nell'header
    }

    if (infoStructure->OPT_CHILDREN_INFO) {
        temp.append(numberOfChildren);
        if (this->getNumbrOfChildren() > 0) {
            temp.append(childrenToEncodedString(childrenId));
        }
    }

    return temp;
}

bool NodeInfo::setNodeField(string * nodeField) {

    int from = 0;
    int to = from + infoStructure->getBitDepth() - 1;
    setDepth(stoi(partitioner(nodeField, from, to), nullptr, 2));

    from = to + 1;
    to = from + infoStructure->getBitNodeDepth() - 1;
    setNodeDepth(stoi(partitioner(nodeField, from, to), nullptr, 2));

    from = to + 1;
    to = from + infoStructure->getBitLb() - 1;
    setLb(stoi(partitioner(nodeField, from, to), nullptr, 2));

    from = to + 1;
    to = from + infoStructure->getBitRb() - 1;
    setRb(stoi(partitioner(nodeField, from, to), nullptr, 2));

    if (infoStructure->OPT_LABEL) {
        from = to + 1;
        to = from + infoStructure->getBitLabel() - 1;
        setLabel(stoi(partitioner(nodeField, from, to), nullptr, 2));
    }

    if (infoStructure->OPT_FATHERLABLE) {
        from = to + 1;
        to = from + infoStructure->getBitFatherLabel() - 1;
        setFatherLabel(stoi(partitioner(nodeField, from, to), nullptr, 2));
    }

    if (infoStructure->OPT_EDGEINFO) {
        from = to + 1;
        to = from + infoStructure->getBitEdgeLength() - 1;
        setEdgeLength(stoi(partitioner(nodeField, from, to), nullptr, 2));
        from = to + 1;
        to = from + getEdgeCharacterEncoding() * getEdgeLength() - 1;
        edge = partitioner(nodeField, from, to);
        setBinaryEdge(&edge);
    }

    if (infoStructure->OPT_CHILDREN_INFO) {
        from = to + 1;
        to = from + infoStructure->getBitNumberOfChildren() - 1;
        setNumberOfChildren(stoi(partitioner(nodeField, from, to), nullptr, 2));

        from = to + 1;
        to = from + 16 * getNumbrOfChildren() - 1;
        string t = partitioner(nodeField, from, to);
        setChildren(&t);
    }

}

void NodeInfo::setDepth(unsigned long n) {
//    depth = std::bitset<bitDepth>(n).to_string();
    depth = toBinFormat(infoStructure->getBitDepth(), n);
}

void NodeInfo::setNodeDepth(unsigned long n) {
    nodeDepth = toBinFormat(infoStructure->getBitNodeDepth(), n);
}

void NodeInfo::setLb(unsigned long n) {
    lb = toBinFormat(infoStructure->getBitLb(), n);
}

void NodeInfo::setRb(unsigned long n) {
    rb = toBinFormat(infoStructure->getBitRb(), n);
}

void NodeInfo::setLabel(unsigned long n) {
    label = toBinFormat(infoStructure->getBitLabel(), n);
}

void NodeInfo::setFatherLabel(unsigned long n) {
    fatherLabel = toBinFormat(infoStructure->getBitFatherLabel(), n);
}

void NodeInfo::setEdgeLength(unsigned long n) {
    edgeLength = toBinFormat(infoStructure->getBitEdgeLength(), n);
}

//codifica l'edge da stringa a binario
void NodeInfo::setEdge(string *s) {
    edgeLength = toBinFormat(infoStructure->getBitEdgeLength(), s->size());
//    edgeLength = std::bitset<bitEdgeLength>(s->size()).to_string();
    string character;
    string edge = "";
    for (int i = 0; i < s->size(); i++) {
        character = "";
        character += s->at(i);
        edge += encodeCharacter(&character, &(infoStructure->codification), &(infoStructure->alphabet));
    }
    this->edge = edge;
}

void NodeInfo::setEdgeCharacterEncoding(unsigned long n) {
    edgeCharacterEncoding = toBinFormat(infoStructure->getBitEdgeCharacterEncoding(), n);
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
    s.append("\nNodeDepth:        " + to_string(getNodeDepth()));
    s.append("\nDepth:            " + to_string(getDepth()));
    s.append("\nLb-Rb:            [" + to_string(getLb()) + "-" + to_string(getRb()) + "]");


    if (infoStructure->OPT_LABEL) {
        s.append("\nLabel:            " + to_string(getLabel()));
    } else {
        s.append("\nLabel:            NOT SET");

    }

    if (infoStructure->OPT_FATHERLABLE) {
        s.append("\nFather's Label:   " + to_string(getFatherLabel()));
    } else {
        s.append("\nFather's Label:   NOT SET");

    }

    if (infoStructure->OPT_EDGEINFO) {
        s.append("\nEdge Length:      " + to_string(getEdgeLength()));
        s.append("\nEdge:             " + getEdgeDecoded());
    } else {
        s.append("\nEdge:             NOT SET");
    }

    if (infoStructure->OPT_CHILDREN_INFO) {
        s.append("\n#Children:         " + to_string(getNumbrOfChildren()));

        if (this->getNumbrOfChildren() > 0) {
            for (auto i : getChildrenId()) {
                s.append("\nChildrens:    " + to_string(i));
            }
        }
    } else {
        s.append("\n#Children:         NOT SET");
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

string NodeInfo::encodeCharacter(string *s, vector<string> *codification, vector<string> *alphabet) {

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
    numberOfChildren = toBinFormat(infoStructure->getBitNumberOfChildren(), n);
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
        tmp.append(toBinFormat(infoStructure->getBitChildrenId(), i));
    }

    return tmp;
}

