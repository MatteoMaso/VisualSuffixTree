//
// Created by alessia on 07/05/18.
//
#include <string>

#ifndef VISUALSUFFIXTREE_OBJNODE_H
#define VISUALSUFFIXTREE_OBJNODE_H

using namespace std;

class ObjNode{

public:
    ObjNode();

    ObjNode(int objNodeDepth);


    int objNodeDepth;
    int objNodeX;
    int objNodeY;
    int objNodeWid;
    int numberOfChildren;
    //int ovjNodeHeigth; per ora li facciamo tutti alla stessa altezza


};
#endif //VISUALSUFFIXTREE_OBJNODE_H
