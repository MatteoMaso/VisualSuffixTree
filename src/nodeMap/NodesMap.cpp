//
// Created by root on 11/11/18.
//
//todo this is a new class for the Map
//todo (print) convert all th print with the logger
#include <iostream>
#include "NodesMap.h"
#include "../node/Node_2.h"
#include <map>
#include <exception>
#include <sstream>

NodesMap::NodesMap(const char * fileName, std::string modeDb) {

    leveldb::Options options;
    options.create_if_missing = true;

    if(modeDb.compare("r")){
        DBMODE = "r";
    } else if (modeDb.compare("w")){
        DBMODE = "w";
    } else {
        //Error no valid db modality found
    }

    std::cout << "NodesMap creator called, folder: " << fileName << std::endl;
    leveldb::Status status = leveldb::DB::Open(options, fileName, &db);
    if (!status.ok()) std::cerr << status.ToString() << std::endl;
    assert(status.ok()); //todo check why it's not correct!!
}



NodesMap::~NodesMap() {

    if(DBMODE.compare("r")){

    } else if (DBMODE.compare("w")){
        if(writeToMemory()) {
            std::cout << "Write in memory ok\n" << std::endl;
        }

    } else {
        //Error no valid db modality found
        std::cerr << "Set db modality\n" << std::endl;
    }
    //delete db; //todo check why arrise error


    return;
}

void NodesMap::showDBcontent(){
    std::cout << "Show DATABASE CONTENT\n" << std::endl;
    try{
        leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            //leveldb::Slice key = it->key();
            leveldb::Slice value = it->value();

            NodeNew * n3 = new NodeNew(value.ToString());
        }

        assert(it->status().ok());  // Check for any errors found during the scan
        delete it;
    } catch (...){
        std::cerr << "Error occurred in reading element from db" << std::endl;
    }
}

void NodesMap::showContent(){
    // Create a map iterator and point to beginning of map
    std::cout << "Show content\n" << std::endl;
    // Iterate over the map using c++11 range based for loop
    for (std::pair<nodeNew::index, NodeNew * > element : local_map) {
        // Accessing KEY from element
        std::cout << "element " << element.second->get_index() << std::endl;
        std::cout << element.second->toString() << " end to string" << std::endl;
    }
}

void NodesMap::addNode(NodeNew * n) {
    //todo (memory optimization) add a check to control if it's to big and flush into the hdd
    local_map.insert (std::pair<nodeNew::index, NodeNew * >(n->get_index(),n));

    //std::cout << "Add node: index: " << n->get_index() << " depth: "<< n->getDepth()<<" children: "<<n->getNumberOfChildren()   <<" wl: "<<n->getNumberOfWinerLink() << std::endl;
}

void NodesMap::removeNode(nodeNew::index idx) {
    local_map.erase(idx);
}

NodeNew * NodesMap::getNode(nodeNew::index idx) {
    return local_map.at(idx);
}

//Load the data from the memory database to the local map
bool NodesMap::readFromMemory() {

    std::cout << "Reading data from the db" << std::endl;
    try{
        leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            //leveldb::Slice key = it->key();
            leveldb::Slice value = it->value();

            NodeNew * n3 = new NodeNew(value.ToString());
            local_map.insert (std::pair<nodeNew::index, NodeNew * >(n3->get_index(),n3));
        }
        assert(it->status().ok());  // Check for any errors found during the scan
        delete it;
    } catch (...){
        std::cerr << "Error occurred in reading element from db" << std::endl;
        return false;
    }

    return true;
}

//write all the map into a memory DataBase
bool NodesMap::writeToMemory() {
    //Take all the element into the levelDB
    std::cout << "Writing element into the memory\n" << std::endl;

    // Map iterator
    for (std::map<nodeNew::index , NodeNew * >::iterator it=local_map.begin(); it!=local_map.end(); ++it){

        std::ostringstream * valueStream = new std::ostringstream;
        it->second->serialize(valueStream);

        //Insert key and value
        leveldb::Status s = db->Put(leveldb::WriteOptions(), std::to_string(it->second->get_index()) , valueStream->str());

        delete valueStream;
    }

    return true;
}

unsigned long NodesMap::getNumberOfElement() {
    return local_map.size();
}


