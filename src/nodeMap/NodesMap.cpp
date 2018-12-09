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

    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        leveldb::Slice key = it->key();
        leveldb::Slice value = it->value();
        std::cout << "size of value " << value.size() << std::endl;

        unsigned long d = *key.data();
        //memcpy(&a, &key, sizeof(unsigned int));
        printf("Read key: %u\n", d);
        NodeNew *n3 = new NodeNew(value.data());
    }
}

void NodesMap::showContent(){
    // Create a map iterator and point to beginning of map
    std::cout << "Show content\n" << std::endl;
    // Iterate over the map using c++11 range based for loop
    for (std::pair<nodeNew::index, NodeNew*> element : local_map) {
        // Accessing KEY from element
        std::cout << element.first << std::endl;
        std::cout << element.second->toString() << std::endl;
    }
}

void NodesMap::addNode(NodeNew * n) {
    //todo (memory optimization) add a check to controll if it's to big and flush into the hdd

    local_map.insert (std::pair<nodeNew::index, NodeNew * >(n->get_index(),n));

    //Add memory
    std::cout << "Add node: index: " << n->get_index() << " depth: "<< n->getDepth()<<" children: "<<n->getNumberOfChildren()   <<" wl: "<<n->getNumberOfWinerLink() << std::endl;
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
            leveldb::Slice key = it->key();
            leveldb::Slice value = it->value();
            //std::cout << "size of value " << value.size() << std::endl;
            //std::string key_str = key.ToString();
            //std::string val_str = value.ToString();
            //unsigned int a = (key.data()[3] << 24) | (key.data()[2] << 16) | (key.data()[1] << 8) | (key.data()[0]);
            //unsigned long a = (key.data()[0] << 56) | (key.data()[1] << 48) | (key.data()[2] << 40) | (key.data()[3] << 32)| (key.data()[4] << 24)| (key.data()[5] << 16)| (key.data()[6] << 8)| (key.data()[7]);
            //unsigned long a2 = (key.data()[7] << 56) | (key.data()[6] << 48) | (key.data()[5] << 40) | (key.data()[4] << 32)| (key.data()[3] << 24)| (key.data()[2] << 16)| (key.data()[1] << 8)| (key.data()[0]);
            //unsigned int f;
            unsigned long d = *key.data();
            
            value.ToString();
            NodeNew * n3 = new NodeNew(value.data());
            local_map.insert (std::pair<nodeNew::index, NodeNew * >(d,n3));
        }
        assert(it->status().ok());  // Check for any errors found during the scan
        delete it;
    } catch (...){
        std::cerr << "Error occurred in reading element from db" << std::endl;
        return false;
    }

    return true;
}
#include <sstream>
//write all the map into a memory DataBase
bool NodesMap::writeToMemory() {
    //Take all the element into the levelDB
    std::cout << "Writing element into the memory\n" << std::endl;

    // Map iterator
    for (std::map<nodeNew::index , NodeNew * >::iterator it=local_map.begin(); it!=local_map.end(); ++it){

        //Manage the key
        char key[sizeof(nodeNew::index)];
        memcpy(key, &it->first, sizeof(nodeNew::index));

        //it->second->serialize(buffer);
        std::ostringstream * valueStream = new std::ostringstream;
        //use the string to save the value... optimize in the future!!
        it->second->serialize(valueStream);

        std::cout << "value: " << valueStream->str() << std::endl;

        //Insert key and value
        leveldb::Status s = db->Put(leveldb::WriteOptions(), key, valueStream->str());

        delete valueStream;
    }

    return true;
}

unsigned long NodesMap::getNumberOfElement() {
    return local_map.size();
}


