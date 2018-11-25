//
// Created by root on 11/11/18.
//

#ifndef VISUALSUFFIXTREE_NODE_H
#define VISUALSUFFIXTREE_NODE_H

//todo this is a new class for the Node

#include <vector>
#include <map>

namespace nodeNew{

    //todo (optimization) decide the type of the parameter

    typedef unsigned long index;
    typedef unsigned long depth;
    typedef unsigned long nodeDepth;
    typedef unsigned long lb;
    typedef unsigned long rb;
    typedef unsigned long label;
    typedef unsigned long fatherLabel;
    typedef unsigned long edgeLen;
    typedef unsigned int edgeIdx;
    typedef int numberOfChildren;
    typedef std::vector<nodeNew::index> * children;
    typedef int numberOfWinerLink;
    typedef std::map<int, unsigned long> * winerLink;
    typedef float klDivergence;
    typedef float pNorm;
    typedef float pNormNoParam;
    typedef float hEntropy;
    typedef float hEntropy2;

}

class NodeNew {

public:

    NodeNew();         //Normal constructor

    ~NodeNew();        //Deconstructor

    NodeNew(char * raw_data);  //constructor from the raw_data, the same produced from the serialize

    int serialize(char * buffer); //put the data of the node in a series of bytes in the buffer and return the length of bytes used

    const unsigned int get_bytes_size();

    //GETTER AND SETTER
    nodeNew::index get_index();

    void set_index(nodeNew::index idx);

    nodeNew::depth getDepth() const;

    void setDepth(nodeNew::depth depth);

    nodeNew::nodeDepth getNodeDepth() const;

    void setNodeDepth(nodeNew::nodeDepth nodeDepth);

    nodeNew::lb getLb() const;

    void setLb(nodeNew::lb lb);

    nodeNew::rb getRb() const;

    void setRb(nodeNew::rb rb);

    nodeNew::label getLabel() const;

    void setLabel(nodeNew::label label);

    nodeNew::fatherLabel getFatherLabel() const;

    void setFatherLabel(nodeNew::fatherLabel fatherLabel);

    nodeNew::edgeLen getEdgeLen() const;

    void setEdgeLen(nodeNew::edgeLen edgeLen);

    nodeNew::edgeIdx getEdgeIdx() const;

    void setEdgeIdx(nodeNew::edgeIdx edgeIdx);

    nodeNew::numberOfChildren getNumberOfChildren() const;

    void setNumberOfChildren(nodeNew::numberOfChildren numberOfChildren);

    const std::vector<nodeNew::index> *getChildren() const;

    void setChildren(const std::vector<nodeNew::index> *children);

    nodeNew::numberOfWinerLink getNumberOfWinerLink() const;

    void setNumberOfWinerLink(nodeNew::numberOfWinerLink numberOfWinerLink);

    const std::map<int, unsigned long> *getWinerLink() const;

    void setWinerLink(std::map<int, unsigned long> *winerLink);

    nodeNew::klDivergence getKlDivergence() const;

    void setKlDivergence(nodeNew::klDivergence klDivergence);

    nodeNew::pNorm getPNorm() const;

    void setPNorm(nodeNew::pNorm pNorm);

    nodeNew::pNormNoParam getPNormNoParam() const;

    void setPNormNoParam(nodeNew::pNormNoParam pNormNoParam);

    nodeNew::hEntropy getHEntropy() const;

    void setHEntropy(nodeNew::hEntropy hEntropy);

    nodeNew::hEntropy2 getHEntropy2() const;

    void setHEntropy2(nodeNew::hEntropy2 hEntropy2);

private:

    nodeNew::index index;
    nodeNew::depth depth;
    nodeNew::nodeDepth nodeDepth;
    nodeNew::lb lb;
    nodeNew::rb rb;
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


};


#endif //VISUALSUFFIXTREE_NODE_H
