//
// Created by root on 4/29/18.
//
#include <string>
#include "NodeInfoStructure.h"

#ifndef VISUALSUFFIXTREE_NODEINFO_H
#define VISUALSUFFIXTREE_NODEINFO_H

using namespace std;

class NodeInfo {

public:

    NodeInfo(NodeInfoStructure *nodeInfoStructure);

    //SETTER
    bool setNodeField(string *nodeField);

    void setDepth(unsigned long n);

    void setNodeDepth(unsigned long n);

    void setLb(unsigned long n);

    void setRb(unsigned long n);

    void setLabel(unsigned long n);

    void setFatherLabel(unsigned long n);

    void setEdgeLength(unsigned long n);

//codifica l'edge da stringa a binario
    void setEdge(string *s);

    void setEdgeCharacterEncoding(unsigned long n);

    void setNumberOfChildren(int n);

    void setChildrenId(vector <int> * childrenId);


    //GETTER

    string getNodeField();

    int getDepth();

    int getNodeDepth();

    int getLb();

    int getRb();

    int getLabel();

    int getFatherLabel();

    int getEdgeLength();

    int getEdgeCharacterEncoding();

    string getEdgeDecoded();


    string partitioner(string *s, int from, int to);

    string encodeCharacter(string *s, vector<string> *codification, vector<string> *alphabet);

    string decodeCharacter(string *s, vector<string> *codification, vector<string> *alphabet);

    void setBinaryEdge(string *s);

    int getNumbrOfChildren();

    vector<int> getChildrenId();

    NodeInfoStructure *infoStructure;

    //UTILS
    string print();

    string depth;
    string nodeDepth;
    string lb;
    string rb;
    string label;
    string fatherLabel;
    string edgeLength;
    string edge;
    string edgeCharacterEncoding;
    string numberOfChildren;
    vector<int> childrenId;

//    vector <string> alphabet = {"a", "c", "g", "t", "$","-"};
////    vector <string> alphabet = {"a", "b", "n", "t", "$","-"};
//    vector <string> codification = {"100", "010", "001", "110", "101", "011"};

private:
    string childrenToEncodedString(vector<int> v);
    void setChildren(string * childrenString);

};

#endif //VISUALSUFFIXTREE_NODEINFO_H
