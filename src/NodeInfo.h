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

    //codifica l'edgeIdx da stringa a binario
    void setEdge(string *s);

    void setEdgeCharacterEncoding(unsigned long n);

    void setNumberOfChildren(int n);

    void setChildrenId(vector<unsigned long> *childrenId);

    void setNumberOfWinerLink(int n);

    void setWinerLinkId(map<int, unsigned long> *wlId);

    void setKl_divergence(float n);

    void setP_norm(float n);

    void setP_normNoParam(float n);

    void setH_entropy(float n);

    void setH_entropySpecial(float n);

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

    string getEdge(unsigned long idx, unsigned long length);

    string partitioner(string *s, unsigned long from, unsigned long to);

    string encodeCharacter(string *s, vector<string> *codification, vector<string> *alphabet);

    string decodeCharacter(string *s, vector<string> *codification, vector<string> *alphabet);

    void setBinaryEdge(string *s);

    void setEdgeIndex(unsigned long idx);

    int getNumbrOfChildren();

    vector<unsigned long> getChildrenId();

    int getNumberOfWl();

    map<int, unsigned long> getWlId();

    float getKl_divergence();
    float getP_norm();
    float getP_normNoParam();
    float getH_entropy();
    float getH_entropySpecial(); //to change
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

    string kl_divergence;
    string p_norm;
    string p_normNoParam;
    string h_entropy;
    string h_entropySpecial; //to change



    string * originalString;

    int BIT_WINERLINK = 32;
    int BIT_IDX_WL = 32;

    string floatToBin(float * f){

        unsigned long mask = std::bitset<32>(1).to_ulong();
        string sd = "";
        unsigned int * inta = reinterpret_cast<unsigned int*>(f);
        for (int i  = 0; i<32; i++){
            ( *inta & (mask << i)) == 0 ? sd += "0" : sd += "1";
        }

        return sd;
    }

    float strToFLoat(string s){


        int n = 0;
        for(int i = 0; i < 32; ++i)
        {
            n |= (s[i] - 48) << i;
        }

        return *(float *)&n;
    }



        private:

    string childrenToEncodedString(vector<unsigned long> v);

    string wlToEncodedString(map<int, unsigned long>);

    void setChildren(string *childrenString);

    void setWl(string *winerLinkString);


};

#endif
