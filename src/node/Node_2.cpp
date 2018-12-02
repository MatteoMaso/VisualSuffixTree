//
// Created by root on 11/11/18.
//


//this is a new class for the Node
#include <iostream>
#include <cstring>
#include "Node_2.h"
#include <vector>
#include <sstream>


NodeNew::NodeNew() {
    std::cout << "Nodes creator called\n" << std::endl;
}

//Constructor from data
NodeNew::NodeNew(std::string ss){

    NodeNew::children = new std::vector<nodeNew::index>();

    std::cout << "Value read: " << ss << std::endl;

    std::string s;

    // Returns first token
    char * token = strtok((char *)ss.c_str(), ":");
    char * value = strtok(NULL, ":");

    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL)
    {
        if(std::strcmp(token, "index")== 0){
            NodeNew::index = strtoul (value, NULL, 0);
        } else if(std::strcmp(token, "depth")== 0){
            NodeNew::depth = strtoul (value, NULL, 0);
        }else if(std::strcmp(token, "nodeDepth")== 0){
            NodeNew::nodeDepth = strtoul (value, NULL, 0);
        }else if(std::strcmp(token, "lb")== 0){
            NodeNew::lb = strtoul (value, NULL, 0);
        }else if(std::strcmp(token, "rb")== 0){
            NodeNew::rb = strtoul (value, NULL, 0);
        }else if(std::strcmp(token, "label")== 0){
            NodeNew::label = strtoul (value, NULL, 0);
        }else if(std::strcmp(token, "fatherLabel")== 0){
            NodeNew::fatherLabel = strtoul (value, NULL, 0);
        }else if(std::strcmp(token, "edgeLen")== 0){
            NodeNew::edgeLen = strtoul (value, NULL, 0);
        }else if(std::strcmp(token, "edgeIdx")== 0){
            NodeNew::edgeIdx = strtoul (value, NULL, 0);
        }else if(std::strcmp(token, "numberOfChildren")== 0){
            NodeNew::numberOfChildren = strtoul (value, NULL, 0);
        }else if(std::strcmp(token, "children")== 0){
            NodeNew::children->push_back(strtoul (value, NULL, 0));
        }

        token = strtok(NULL, ":");
        value = strtok(NULL, ":");

    }

}


NodeNew::~NodeNew() {
    std::cout << "Nodes deconstructor called\n" << std::endl;
}

int NodeNew::serialize(std::ostringstream  * valueStream) {

    *valueStream << "index:" << index << ":";

    *valueStream << "depth:" << depth<< ":";

    *valueStream << "nodeDepth:" << nodeDepth<< ":";

    *valueStream << "lb:" << lb<< ":";

    *valueStream << "rb:" << rb<< ":";

    *valueStream << "label:" << label<< ":";

    *valueStream << "fatherLabel:" << fatherLabel<< ":";

    *valueStream << "edgeLen:" << edgeLen<< ":";

    *valueStream << "edgeIdx:" << edgeIdx<< ":";

    *valueStream << "numberOfChildren:" << numberOfChildren<< ":";

    //Se number of children > 0 set children
    if(NodeNew::numberOfChildren > 0){
        for(int i = 0; i < numberOfChildren; i++){
            if(children->size() > i){

                *valueStream << "children:" << children->at(i)<< ":";

            }else{
                std::cout << "Children not present" << std::endl;
            }
        }
    }

    return 0;
}


int NodeNew::serialize(unsigned char * buffer) {
    //Add a check of the buffer size

    unsigned int pointer = 0;
    memcpy(buffer, &index, sizeof(nodeNew::index));

    pointer += sizeof(nodeNew::depth);
    memcpy(buffer + pointer, &depth, sizeof(nodeNew::depth));

    pointer += sizeof(nodeNew::nodeDepth);
    memcpy(buffer + pointer, &nodeDepth, sizeof(nodeNew::nodeDepth));

    pointer += sizeof(nodeNew::lb);
    memcpy(buffer + pointer, &lb, sizeof(nodeNew::lb));

    pointer += sizeof(nodeNew::rb);
    memcpy(buffer + pointer, &rb, sizeof(nodeNew::rb));

    //label
    pointer += sizeof(nodeNew::label);
    memcpy(buffer + pointer, &label, sizeof(nodeNew::label));

    //father label
    pointer += sizeof(nodeNew::fatherLabel);
    memcpy(buffer + pointer, &fatherLabel, sizeof(nodeNew::fatherLabel));

    //edge len
    pointer += sizeof(nodeNew::edgeLen);
    memcpy(buffer + pointer, &edgeLen, sizeof(nodeNew::edgeLen));

    //edge idx
    pointer += sizeof(nodeNew::edgeIdx);
    memcpy(buffer + pointer, &edgeIdx, sizeof(nodeNew::edgeIdx));

    //Number of children
    pointer += sizeof(nodeNew::numberOfChildren);
    memcpy(buffer + pointer, &numberOfChildren, sizeof(nodeNew::numberOfChildren));

    //Se number of children > 0 set children
    if(NodeNew::numberOfChildren > 0){
        for(int i = 0; i < numberOfChildren; i++){
            if(children->size() > i){
                pointer += sizeof(nodeNew::index);
                memcpy(buffer + pointer, &(children->at(i)), sizeof(nodeNew::index));
                //printf("Figlio: %u\n", children->at(i));
            }else{
                std::cout << "Children not present" << std::endl;
            }
        }
    }


    //for(int i = 0; i < 5; i++){
    //    buffer[i] = 0b11110001;
    //}
    //etc

    //TODO add other element update pointer and add element ...

    return 0;
}






nodeNew::index NodeNew::get_index() {
    return NodeNew::index;
}

void NodeNew::set_index(nodeNew::index idx) {
    NodeNew::index = idx;
}

nodeNew::depth NodeNew::getDepth() const {
    return depth;
}

void NodeNew::setDepth(nodeNew::depth depth) {
    NodeNew::depth = depth;
}

nodeNew::nodeDepth NodeNew::getNodeDepth() const {
    return nodeDepth;
}

void NodeNew::setNodeDepth(nodeNew::nodeDepth nodeDepth) {
    NodeNew::nodeDepth = nodeDepth;
}

nodeNew::lb NodeNew::getLb() const {
    return lb;
}

void NodeNew::setLb(nodeNew::lb lb) {
    NodeNew::lb = lb;
}

nodeNew::rb NodeNew::getRb() const {
    return rb;
}

void NodeNew::setRb(nodeNew::rb rb) {
    NodeNew::rb = rb;
}

nodeNew::label NodeNew::getLabel() const {
    return label;
}

void NodeNew::setLabel(nodeNew::label label) {
    NodeNew::label = label;
}

nodeNew::fatherLabel NodeNew::getFatherLabel() const {
    return fatherLabel;
}

void NodeNew::setFatherLabel(nodeNew::fatherLabel fatherLabel) {
    NodeNew::fatherLabel = fatherLabel;
}

nodeNew::edgeLen NodeNew::getEdgeLen() const {
    return edgeLen;
}

void NodeNew::setEdgeLen(nodeNew::edgeLen edgeLen) {
    NodeNew::edgeLen = edgeLen;
}

nodeNew::edgeIdx NodeNew::getEdgeIdx() const {
    return edgeIdx;
}

void NodeNew::setEdgeIdx(nodeNew::edgeIdx edgeIdx) {
    NodeNew::edgeIdx = edgeIdx;
}

nodeNew::numberOfChildren NodeNew::getNumberOfChildren() const {
    return numberOfChildren;
}

void NodeNew::setNumberOfChildren(nodeNew::numberOfChildren numberOfChildren) {
    NodeNew::numberOfChildren = numberOfChildren;
}

const std::vector<nodeNew::index> *NodeNew::getChildren() const {
    return children;
}

void NodeNew::setChildren(const std::vector<nodeNew::index> *children) {
    NodeNew::children = new std::vector<nodeNew::index>(); //support structure

    for(int i = 0; i < children->size(); i++){
        NodeNew::children->push_back(children->at(i));
    }

    setNumberOfChildren(children->size());
}

nodeNew::numberOfWinerLink NodeNew::getNumberOfWinerLink() const {
    return numberOfWinerLink;
}

void NodeNew::setNumberOfWinerLink(nodeNew::numberOfWinerLink numberOfWinerLink) {
    NodeNew::numberOfWinerLink = numberOfWinerLink;
}

const std::map<int, unsigned long> *NodeNew::getWinerLink() const {
    return winerLink;
}

void NodeNew::setWinerLink(std::map<int, unsigned long> *winerLink) {
    NodeNew::winerLink = winerLink;
    //todo check the correctness
}

nodeNew::klDivergence NodeNew::getKlDivergence() const {
    return klDivergence;
}

void NodeNew::setKlDivergence(nodeNew::klDivergence klDivergence) {
    NodeNew::klDivergence = klDivergence;
}

nodeNew::pNorm NodeNew::getPNorm() const {
    return pNorm;
}

void NodeNew::setPNorm(nodeNew::pNorm pNorm) {
    NodeNew::pNorm = pNorm;
}

nodeNew::pNormNoParam NodeNew::getPNormNoParam() const {
    return pNormNoParam;
}

void NodeNew::setPNormNoParam(nodeNew::pNormNoParam pNormNoParam) {
    NodeNew::pNormNoParam = pNormNoParam;
}

nodeNew::hEntropy NodeNew::getHEntropy() const {
    return hEntropy;
}

void NodeNew::setHEntropy(nodeNew::hEntropy hEntropy) {
    NodeNew::hEntropy = hEntropy;
}

nodeNew::hEntropy2 NodeNew::getHEntropy2() const {
    return hEntropy2;
}

void NodeNew::setHEntropy2(nodeNew::hEntropy2 hEntropy2) {
    NodeNew::hEntropy2 = hEntropy2;
}

nodeNew::frequency NodeNew::getFrequency() const {
    return rb - lb + 1;
}


const unsigned int NodeNew::get_bytes_size() {

    unsigned int tmp = 0;

    tmp += sizeof(nodeNew::index);
    tmp += sizeof(nodeNew::depth);
    tmp += sizeof(nodeNew::nodeDepth);
    tmp += sizeof(nodeNew::lb);
    tmp += sizeof(nodeNew::rb);
    tmp += sizeof(nodeNew::label);
    tmp += sizeof(nodeNew::fatherLabel);
    tmp += sizeof(nodeNew::edgeLen);
    tmp += sizeof(nodeNew::edgeIdx);
    tmp += sizeof(nodeNew::numberOfChildren);
    tmp += sizeof(nodeNew::children);
    /*
    nodeNew::numberOfWinerLink numberOfWinerLink;
    nodeNew::winerLink winerLink;
    nodeNew::klDivergence klDivergence;
    nodeNew::pNorm pNorm;
    nodeNew::pNormNoParam pNormNoParam;
    nodeNew::hEntropy hEntropy;
    nodeNew::hEntropy2 hEntropy2;
    */

    return tmp; //todo fix
}

std::string NodeNew::toString() {

    std::ostringstream oss;
    oss << "Node info\nKey: " << NodeNew::index <<
        "\nDepth: " << getDepth() ;



    return oss.str();
}

