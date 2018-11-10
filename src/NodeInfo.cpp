//
// Created by root on 4/28/18.
//

#include <bitset>
#include <vector>
#include <iostream>
#include <string>
#include "NodeInfo.h"
#include "Utils.h"
#include "Utils.h"

using namespace std;



NodeInfo::NodeInfo(NodeInfoStructure *nodeInfoStructure, string * originalString) {
    //Save the address oh the structure to avoid multiple copies
    this->infoStructure = nodeInfoStructure;
    this->originalString = originalString;
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
        temp.append(edge_idx);
        //    temp.append(edgeCharacterEncoding); questo non serve metterlo va solo nell'header
    }

    if (infoStructure->OPT_CHILDREN_INFO) {
        temp.append(numberOfChildren);
        if (this->getNumbrOfChildren() > 0) {
            temp.append(childrenToEncodedString(childrenId));
        }
    }

    if(true){ //Add winer link
        temp.append(numberOfWinerLink);
        if (this->getNumberOfWl() > 0) {
            temp.append(wlToEncodedString(wlId));
        }
    }

    temp.append(kl_divergence);
    temp.append(p_norm);
    temp.append(p_normNoParam);
    temp.append(h_entropy);
    temp.append(h_entropySpecial);

    return temp;
}


bool NodeInfo::setNodeField(string * nodeField) {

//    int from = 0;
//    int to = from + infoStructure->getBitDepth() - 1;
//    setDepth(stoi(partitioner(nodeField, from, to), nullptr, 2));
//
//    from = to + 1;
//    to = from + infoStructure->getBitNodeDepth() - 1;
//    setNodeDepth(stoi(partitioner(nodeField, from, to), nullptr, 2));
//
//    from = to + 1;
//    to = from + infoStructure->getBitLb() - 1;
//    setLb(stoi(partitioner(nodeField, from, to), nullptr, 2));
//
//    from = to + 1;
//    to = from + infoStructure->getBitRb() - 1;
//    setRb(stoi(partitioner(nodeField, from, to), nullptr, 2));
//
//    if (infoStructure->OPT_LABEL) {
//        from = to + 1;
//        to = from + infoStructure->getBitLabel() - 1;
//        setLabel(stoi(partitioner(nodeField, from, to), nullptr, 2));
//    }
//
//    if (infoStructure->OPT_FATHERLABLE) {
//        from = to + 1;
//        to = from + infoStructure->getBitFatherLabel() - 1;
//        setFatherLabel(stoi(partitioner(nodeField, from, to), nullptr, 2));
//    }
//
//    if (infoStructure->OPT_EDGEINFO) {
//        from = to + 1;
//        to = from + infoStructure->getBitEdgeLength() - 1;
//        setEdgeLength(stoi(partitioner(nodeField, from, to), nullptr, 2));
//        from = to + 1;
//        to = from + 32 - 1;
//        edge_idx = partitioner(nodeField, from, to);
//    }
//
//    if (infoStructure->OPT_CHILDREN_INFO) {
//        from = to + 1;
//        to = from + infoStructure->getBitNumberOfChildren() - 1;
//        setNumberOfChildren(stoi(partitioner(nodeField, from, to), nullptr, 2));
//
//        from = to + 1;
//        to = from + 32 * getNumbrOfChildren() - 1;
//        string t = partitioner(nodeField, from, to);
//        setChildren(&t);
//    }
//
//    if(true){ //GET WINER LINK
//        from = to + 1;
//        to = from + infoStructure->getBitNumberOfWinerLink() - 1;
//        setNumberOfWinerLink(stoi(partitioner(nodeField, from, to), nullptr, 2));
//
//        from = to + 1;
//        to = from + (BIT_WINERLINK + BIT_IDX_WL) * getNumberOfWl() - 1;
//        string t = partitioner(nodeField, from, to);
//        setWl(&t);
//    }
//
//
//    if(true){ //SET STATISTICS
//        from = to + 1;
//        to = from + infoStructure->getBitNumberOfStatistics() - 1;
//        setKl_divergence(strToFLoat(partitioner(nodeField, from, to)));
//
//        from = to + 1;
//        to = from + infoStructure->getBitNumberOfStatistics() - 1;
//        setP_norm(strToFLoat(partitioner(nodeField, from, to)));
//
//        from = to + 1;
//        to = from + infoStructure->getBitNumberOfStatistics() - 1;
//        setP_normNoParam(strToFLoat(partitioner(nodeField, from, to)));
//
//        from = to + 1;
//        to = from + infoStructure->getBitNumberOfStatistics() - 1;
//        setH_entropy(strToFLoat(partitioner(nodeField, from, to)));
//
//        from = to + 1;
//        to = from + infoStructure->getBitNumberOfStatistics() - 1;
//        setH_entropySpecial(strToFLoat(partitioner(nodeField, from, to)));
//    }

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
        to = from + 32 - 1;
        edge_idx = partitioner(nodeField, from, to);
    }

    if (infoStructure->OPT_CHILDREN_INFO) {
        from = to + 1;
        to = from + infoStructure->getBitNumberOfChildren() - 1;
        setNumberOfChildren(stoi(partitioner(nodeField, from, to), nullptr, 2));

        from = to + 1;
        to = from + 32 * getNumbrOfChildren() - 1;
        string t = partitioner(nodeField, from, to);
        setChildren(&t);
    }

    if(true){ //GET WINER LINK
        from = to + 1;
        to = from + infoStructure->getBitNumberOfWinerLink() - 1;
        setNumberOfWinerLink(stoi(partitioner(nodeField, from, to), nullptr, 2));

        from = to + 1;
        to = from + (BIT_WINERLINK + BIT_IDX_WL) * getNumberOfWl() - 1;
        string t = partitioner(nodeField, from, to);
        setWl(&t);
    }


    if(true){ //SET STATISTICS
        from = to + 1;
        to = from + infoStructure->getBitNumberOfStatistics() - 1;
        kl_divergence = partitioner(nodeField, from, to);

        from = to + 1;
        to = from + infoStructure->getBitNumberOfStatistics() - 1;
        p_norm = partitioner(nodeField, from, to);

        from = to + 1;
        to = from + infoStructure->getBitNumberOfStatistics() - 1;
        p_normNoParam = partitioner(nodeField, from, to);

        from = to + 1;
        to = from + infoStructure->getBitNumberOfStatistics() - 1;
        h_entropy = partitioner(nodeField, from, to);

        from = to + 1;
        to = from + infoStructure->getBitNumberOfStatistics() - 1;
        h_entropySpecial = partitioner(nodeField, from, to);
    }


}

void NodeInfo::setDepth(unsigned long n) {
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

void NodeInfo::setEdgeIndex(unsigned long idx){
    edge_idx = toBinFormat(32, idx);
}

void NodeInfo::setEdgeCharacterEncoding(unsigned long n) {
    edgeCharacterEncoding = toBinFormat(infoStructure->getBitEdgeCharacterEncoding(), n);
}


unsigned long NodeInfo::getDepth() {
    return stoi(depth, nullptr, 2);
}

unsigned long NodeInfo::getNodeDepth() {
    return stoi(nodeDepth, nullptr, 2);
}

unsigned long NodeInfo::getLb() {
    return stoi(lb, nullptr, 2);
}

unsigned long NodeInfo::getRb() {
    return stoi(rb, nullptr, 2);
}

unsigned long NodeInfo::getEdgeLength() {
    return stoi(edgeLength, nullptr, 2);
}

int NodeInfo::getEdgeCharacterEncoding() {
    return stoi(edgeCharacterEncoding, nullptr, 2);
}

unsigned long NodeInfo::getEdgeIndex(){
    return stoi(edge_idx, nullptr, 2);
}

string NodeInfo::getEdge(unsigned long idx, unsigned long length){
    string edge = "";

    for (unsigned long j = idx; j < idx + length; j++) {
        if ( j == originalString->length()) {
            edge += "$";
        } else {
            edge += originalString->at(j);
        }
    }

    return edge;
}

string NodeInfo::print(vector<string> * alphabet) {
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
        s.append("\nEdge:             " + getEdge(getEdgeIndex(), getEdgeLength()));
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

    if(true){
        s.append("\n#Winer Link:       " + to_string(getNumberOfWl()));
        if ( getNumberOfWl() > 0 ){
            for (auto i : getWlId()) {
                s.append("\nWinerLink char: " + alphabet->at(i.first)+ "   wl: "+ to_string(i.second));
            }
        }
    }



    return s;
}


string NodeInfo::partitioner(string *s, unsigned long from, unsigned long to) {

    string a = "";
    for (unsigned long i = from; i <= to; i++) {
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

unsigned long NodeInfo::getLabel() {
    return stoul(label, nullptr, 2);
}

unsigned long NodeInfo::getFatherLabel() {
    return stoul(fatherLabel, nullptr, 2);
}

void NodeInfo::setNumberOfChildren(int n) {
    numberOfChildren = toBinFormat(infoStructure->getBitNumberOfChildren(), n);
}

void NodeInfo::setWl(string * winerLinkString) {
    int idx;
    unsigned  long l;
    wlId.clear();
    int from = 0;
    int to = BIT_IDX_WL - 1;

    for (int i = 0; i < winerLinkString->size() / (BIT_IDX_WL + BIT_WINERLINK); ++i) {

        idx = stoi(partitioner(winerLinkString, from, to ), nullptr, 2);
        from = to + 1;
        to = from + BIT_WINERLINK -1;
        l = stoi(partitioner(winerLinkString, from, to ), nullptr, 2);
        from = to + 1;
        to = from + BIT_IDX_WL -1;

        wlId.insert({idx, l});
    }
}

void NodeInfo::setChildren(string *childrenString) {
    //todo check numero bit e int type
    int bitChildrenId = 32; //todo dev'essere coerente al config file
    string id;
    childrenId.clear();
    for (int i = 0; i < childrenString->size() / bitChildrenId; ++i) {
        id = "";
        for (int j = 0 + i * bitChildrenId; j < ((i + 1) * bitChildrenId); j++) {
            id += childrenString->at(j);
        }
        childrenId.push_back(stoi(id, nullptr, 2));
    }
}

void NodeInfo::setChildrenId(vector<unsigned long> *childrenId) {

    setNumberOfChildren(childrenId->size());
    this->childrenId.clear();
    for (auto i : *childrenId) {
        this->childrenId.push_back(i);
    }
}

int NodeInfo::getNumbrOfChildren() {
    return stoi(numberOfChildren, nullptr, 2);
}

vector<unsigned long> NodeInfo::getChildrenId() {
    return this->childrenId;
}

string NodeInfo::childrenToEncodedString(vector<unsigned long> v) {

    string tmp = "";

    for (auto i : v) {
        tmp.append(toBinFormat(infoStructure->getBitChildrenId(), i));
    }

    return tmp;
}

string NodeInfo::wlToEncodedString(map<int, unsigned long> v) {

    string tmp = "";

    for (auto i : v) {
        tmp.append(toBinFormat(BIT_IDX_WL , i.first));
        tmp.append(toBinFormat(BIT_WINERLINK , i.second));
    }

    return tmp;
}

void NodeInfo::setNumberOfWinerLink(int n) {
    numberOfWinerLink = toBinFormat(infoStructure->getBitNumberOfWinerLink(), n);
}

//void NodeInfo::setWinerLinkId(map<string, unsigned long> *wlId) {
void NodeInfo::setWinerLinkId(map<int, unsigned long> *wlId) {
    setNumberOfWinerLink(wlId->size()); //SET THE NUMBER OF WINER LINK
    this->wlId.clear();
    for (auto i : *wlId) {
        this->wlId.insert(i);
    }
}

int NodeInfo::getNumberOfWl() {
    return stoi(numberOfWinerLink, nullptr, 2);
}

map<int, unsigned long> NodeInfo::getWlId() {
    return this->wlId;
}

map<string, unsigned long> NodeInfo::getWlCharacter(vector<string> * alphabet) {
    map<string, unsigned long> tmp;
    for (int i = 0; i < wlId.size(); i++) {
        tmp.insert({alphabet->at(i), wlId.at(i)});
    }
    return tmp;
}

float NodeInfo::getKl_divergence() {
    return strToFLoat(kl_divergence);
}

float NodeInfo::getP_norm() {
    return strToFLoat(p_norm);
}

float NodeInfo::getP_normNoParam() {
    return strToFLoat(p_normNoParam);
}

float NodeInfo::getH_entropy() {
    return strToFLoat(h_entropy);
}

float NodeInfo::getH_entropySpecial() {
    return strToFLoat(h_entropySpecial);
}


void NodeInfo::setKl_divergence(float n) {
    kl_divergence = floatToBin(&n);
}

void NodeInfo::setP_norm(float n) {
    p_norm = floatToBin(&n);
}

void NodeInfo::setP_normNoParam(float n) {
    p_normNoParam = floatToBin(&n);
}

void NodeInfo::setH_entropy(float n) {
    h_entropy = floatToBin(&n);
}

void NodeInfo::setH_entropySpecial(float n) {
    h_entropySpecial = floatToBin(&n);
}



