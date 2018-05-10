//
// Created by alessia on 07/05/18.
//

#include <string>
#include <iostream>
#include <bitset>

#include "../Include/ObjNode.h"

using namespace std;

ObjNode::ObjNode() {
     objNodeDepth = 0;
     objNodeX = 0;
     objNodeY = 0;
     objNodeWid = 0;
     numberOfChildren = 0;
     sonsCount = 0;
}

int ObjNode::getObjNodeDepth(){
    return objNodeDepth;
}

void ObjNode::setObjNodeDepth(int objNodeDepth) {
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

float ObjNode::getObjNodeWid(){
    return objNodeWid;
}

void ObjNode::setObjNodeWid(float objNodeWid) {
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