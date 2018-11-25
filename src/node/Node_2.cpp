//
// Created by root on 11/11/18.
//


//todo this is a new class for the Node
#include <iostream>
#include <cstring>
#include "Node_2.h"


NodeNew::NodeNew() {
    std::cout << "Nodes creator called\n" << std::endl;
}

NodeNew::~NodeNew() {
    std::cout << "Nodes deconstructor called\n" << std::endl;

}

NodeNew::NodeNew(char * raw_data) {

}

int NodeNew::serialize(char * buffer) {

    unsigned int pointer = 0;

    memcpy(buffer, &index, sizeof(nodeNew::index));

    pointer += sizeof(nodeNew::depth);
    memcpy(buffer + pointer, &depth, sizeof(nodeNew::depth));

    return 0;
}






nodeNew::index NodeNew::get_index() {
    return index;
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
    NodeNew::children = (nodeNew::children)children;
    //maybe copy the vector elements ...
    //todo important
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

const unsigned int NodeNew::get_bytes_size() {

    unsigned int tmp = 0;

    tmp += sizeof(nodeNew::index);
    tmp += sizeof(nodeNew::depth);
    tmp += sizeof(nodeNew::nodeDepth);
    tmp += sizeof(nodeNew::lb);
    tmp += sizeof(nodeNew::rb);

    /*
    nodeNew::label label;
    nodeNew::fatherLabel fatherLabel;
    nodeNew::edgeLen edgeLen;
    nodeNew::edgeIdx edgeIdx;
    nodeNew::numberOfChildren numberOfChildren;
    nodeNew::children children;
    nodeNew::numberOfWinerLink numberOfWinerLink;
    nodeNew::winerLink winerLink;
    nodeNew::klDivergence klDivergence;
    nodeNew::pNorm pNorm;
    nodeNew::pNormNoParam pNormNoParam;
    nodeNew::hEntropy hEntropy;
    nodeNew::hEntropy2 hEntropy2;
    */

    return tmp;
}

