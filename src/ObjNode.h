//
// Created by alessia on 07/05/18.
//
#ifndef VISUALSUFFIXTREE_OBJNODE_H
#define VISUALSUFFIXTREE_OBJNODE_H

#include <string>

using namespace std;

class ObjNode{

public:
    ObjNode();

    unsigned long getObjNodeDepth();

    void setObjNodeDepth(unsigned long objNodeDepth);

    double getObjNodeX();

    void setObjNodeX(double objNodeX);

    double getObjNodeY();

    void setObjNodeY(double objNodeY);

    double getObjNodeWid();

    void setObjNodeWid(double objNodeWid);

    int getNumberOfChildren();

    void setNumberOfChildren(int numberOfChildren);

    int getSonsCount();

    void incCounter();


private:
    unsigned long objNodeDepth;
    double objNodeX;
    double objNodeY;
    double objNodeWid;
    int numberOfChildren;
    int sonsCount = 0;
    //int ovjNodeHeigth; per ora li facciamo tutti alla stessa altezza


};
#endif //VISUALSUFFIXTREE_OBJNODE_H
