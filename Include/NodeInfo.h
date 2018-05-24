//
// Created by root on 4/29/18.
//
#include <string>
#include "NodeInfoStructure.h"
//#include "NodeInfoStructure.h"

#ifndef VISUALSUFFIXTREE_NODEINFO_H
#define VISUALSUFFIXTREE_NODEINFO_H

using namespace std;

class NodeInfo {

public:

    NodeInfo(NodeInfoStructure *nodeInfoStructure, string * originalString);

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

    void setChildrenId(vector<unsigned long> *childrenId);

    void setNumberOfWinerLink(int n);

    void setWinerLinkId(map<int, unsigned long> *wlId);


    //GETTER

    string getNodeField();

    unsigned long getDepth();

    unsigned long getNodeDepth();

    unsigned long getLb();

    unsigned long getRb();

    unsigned long getLabel();

    unsigned long getFatherLabel();

    unsigned long getEdgeLength();

    int getEdgeCharacterEncoding();

    unsigned long getEdgeIndex();

//    string getEdgeDecoded();

    string getEdge(string * text, unsigned long idx, unsigned long length);

    string partitioner(string *s, unsigned long from, unsigned long to);

    string encodeCharacter(string *s, vector<string> *codification, vector<string> *alphabet);

    string decodeCharacter(string *s, vector<string> *codification, vector<string> *alphabet);

    void setBinaryEdge(string *s);

    void setEdgeIndex(unsigned long idx);

    int getNumbrOfChildren();

    vector<unsigned long> getChildrenId();

    int getNumberOfWl();

    map<int, unsigned long> getWlId();

    map<string, unsigned long> getWlCharacter(vector<string> * alphabet);

    NodeInfoStructure *infoStructure;

    //UTILS
    string print(vector<string> * alphabet);

    string depth;
    string nodeDepth;
    string lb;
    string rb;
    string label;
    string fatherLabel;
    string edgeLength;
    string edge_idx;
    string edgeCharacterEncoding;
    string numberOfChildren;
    vector<unsigned long> childrenId;
    string numberOfWinerLink;
    map<int, unsigned long> wlId;

    string * originalString;

    int BIT_WINERLINK = 16; //todo mettere a 32
    int BIT_IDX_WL = 16;    //todo mettere a 32 ed adattare il codice

private:

    string childrenToEncodedString(vector<unsigned long> v);

    string wlToEncodedString(map<int, unsigned long>);

    void setChildren(string *childrenString);

    void setWl(string *winerLinkString);

};

#endif //VISUALSUFFIXTREE_NODEINFO_H
