//
// Created by alessia on 07/05/18.
//

#include <string>
#include <iostream>
#include <bitset>

#include "ObjNode.h"

using namespace std;

ObjNode::ObjNode() {
    objNodeDepth = 0;
    objNodeX = 0;
    objNodeY = 0;
    objNodeWid = 0;
    numberOfChildren = 0;
    sonsCount = 0;
}

unsigned long ObjNode::getObjNodeDepth(){
    return objNodeDepth;
}

void ObjNode::setObjNodeDepth(unsigned long objNodeDepth) {
    this->objNodeDepth = objNodeDepth;
}

double ObjNode::getObjNodeX(){
    return objNodeX;
}

void ObjNode::setObjNodeX(double objNodeX) {
    this->objNodeX = objNodeX;
}

double ObjNode::getObjNodeY(){
    return objNodeY;
}

void ObjNode::setObjNodeY(double objNodeY) {
    this->objNodeY = objNodeY;
}

double ObjNode::getObjNodeWid(){
    return objNodeWid;
}

void ObjNode::setObjNodeWid(double objNodeWid) {
    this->objNodeWid = objNodeWid;
}

int ObjNode::getNumberOfChildren(){
    return numberOfChildren;
}

void ObjNode::setNumberOfChildren(int numberOfChildren) {
    this->numberOfChildren = numberOfChildren;
}



void ObjNode::incCounter(){
    this->sonsCount++;
}

int ObjNode::getSonsCount() {
    return sonsCount;
}