//
// Created by root on 11/11/18.
//

#ifndef VISUALSUFFIXTREE_NODESMAP_H
#define VISUALSUFFIXTREE_NODESMAP_H

#include <map>
#include "../node/Node.h"

//todo this is a new class for the Map

/*
 * This class contains all the nodes and manage the memory storage of them
 */

class NodesMap {
public:

    NodesMap(const char * fileName);
    ~NodesMap(); //deconstructor

    void addNode(NodeNew * n);             //add a node into the local map

    void removeNode(nodeNew::index);       //remove the node required

    NodeNew * getNode(nodeNew::index);        //return the Node required

    //todo create an iterator


private:

    //todo create the local map with key node and value the node

    bool loadFile(const char * fileName); //Load the file from the memory into the local map

    bool writeToMemory();               //This method takes the node from the local map and put into the computer memory

};


#endif //VISUALSUFFIXTREE_NODESMAP_H
