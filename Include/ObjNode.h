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

    int getObjNodeDepth();

    void setObjNodeDepth(int objNodeDepth);

    int getObjNodeX();

    void setObjNodeX(int objNodeX);

    int getObjNodeY();

    void setObjNodeY(int objNodeY);

    int getObjNodeWid();

    void setObjNodeWid(int objNodeWid);

    int getNumberOfChildren();

    void setNumberOfChildren(int numberOfChildren);

    int getSonsCount();

    int incCounter();


private:
    int objNodeDepth;
    int objNodeX;
    int objNodeY;
    int objNodeWid;
    int numberOfChildren;
    int sonsCount = 0;
    //int ovjNodeHeigth; per ora li facciamo tutti alla stessa altezza


};
#endif //VISUALSUFFIXTREE_OBJNODE_H
