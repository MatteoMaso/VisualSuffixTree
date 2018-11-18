//
// Created by root on 11/11/18.
//
//todo this is a new class for the Map

#include <iostream>
#include "NodesMap.h"
#include "../node/Node_2.h"
#include <map>


//NodesMap::NodesMap(const char *fileName) {
NodesMap::NodesMap() {
    std::cout << "NodesMap creator called\n" << std::endl;
    //todo open a memory and save the file in the memory
}



NodesMap::~NodesMap() {
    std::cout << "NodesMap deconstructor called\n" << std::endl;

    if(writeToMemory()){
        std::cout << "Write in memory ok\n" << std::endl;
    }else{
        std::cout << "NodesMap deconstructor called\n" << std::endl;
    }

}

void NodesMap::showContent(){
    // Create a map iterator and point to beginning of map
    std::map<nodeNew::index, NodeNew*>::iterator it = local_map.begin();

    // Iterate over the map using c++11 range based for loop
    for (std::pair<nodeNew::index, NodeNew*> element : local_map) {
        // Accessing KEY from element

        std::cout << element.first << std::endl;
    }
}

void NodesMap::addNode(NodeNew * n) {
    //todo (memory optimization) add a check to controll if it's to big and flush into the hdd
    local_map.insert ( std::pair<nodeNew::index, NodeNew * >(n->get_index(),n) );
}

void NodesMap::removeNode(nodeNew::index idx) {
    local_map.erase(idx);
}

NodeNew *NodesMap::getNode(nodeNew::index idx) {
    return nullptr;
}

bool NodesMap::loadFile(const char *fileName) {
    return false;
}

bool NodesMap::writeToMemory() {
    return false;
}


