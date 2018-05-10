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

    double getObjNodeX();

    void setObjNodeX(double objNodeX);

    double getObjNodeY();

    void setObjNodeY(double objNodeY);

    float getObjNodeWid();

    void setObjNodeWid(float objNodeWid);

    int getNumberOfChildren();

    void setNumberOfChildren(int numberOfChildren);

    int getSonsCount();

    void incCounter();


private:
    int objNodeDepth;
    double objNodeX;
    double objNodeY;
    float objNodeWid;
    int numberOfChildren;
    int sonsCount = 0;
    //int ovjNodeHeigth; per ora li facciamo tutti alla stessa altezza


};
#endif //VISUALSUFFIXTREE_OBJNODE_H
