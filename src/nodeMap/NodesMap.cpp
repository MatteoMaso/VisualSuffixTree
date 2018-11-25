//
// Created by root on 11/11/18.
//
//todo this is a new class for the Map
//todo (print) convert all th print with the logger
#include <iostream>
#include "NodesMap.h"
#include "../node/Node_2.h"
#include <map>


void levelDB_test() {

    unsigned int a = 23;
    unsigned int b = 34;

    std::cout << "size of unsigned int: " << sizeof(unsigned int) << std::endl;

    unsigned char bytes[8];
    memcpy(bytes, &a, sizeof(a));
    memcpy(bytes + 4, &b, sizeof(b));



    unsigned int a2;
    unsigned int b2;

    memcpy(&a2, bytes, sizeof(a2));
    memcpy(&b2, bytes + 4, sizeof(b2));

    printf("a: %u\n", a2);
    printf("b: %u\n", b2);



    leveldb::DB* db;
    leveldb::Options options;

    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/root/lDB/", &db);
    assert(status.ok());

    leveldb::Status s;
    std::string value = "100ghghghg";
    std::string key = "ciao";
    //leveldb::Status s = db->Get(leveldb::ReadOptions(), key1, &value);

    if (s.ok()) s = db->Put(leveldb::WriteOptions(), key, value);
    std::string document;
    db->Get(leveldb::ReadOptions(), "ciao", &document);
    std::cout << document << std::endl;
    delete db;
}

NodesMap::NodesMap(const char * fileName) {
    std::cout << "NodesMap creator called, folder: " << fileName << std::endl;

    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, fileName, &db);
    assert(status.ok()); //todo check why it's not correct!!
}



NodesMap::~NodesMap() {
    //todo don't write into the memory if it's in  reading mode!
    std::cout << "NodesMap deconstructor called\n" << std::endl;

    if(writeToMemory()){
        std::cout << "Write in memory ok\n" << std::endl;
    }else{
        std::cout << "NodesMap deconstructor called\n" << std::endl;
    }

    delete db;
}

void NodesMap::showContent(){
    // Create a map iterator and point to beginning of map
    //std::map<nodeNew::index, NodeNew*>::iterator it = local_map.begin();
    std::cout << "Show content\n" << std::endl;
    // Iterate over the map using c++11 range based for loop
    for (std::pair<nodeNew::index, NodeNew*> element : local_map) {
        // Accessing KEY from element

        std::cout << element.first << std::endl;
    }
}

void NodesMap::addNode(NodeNew * n) {
    //todo (memory optimization) add a check to controll if it's to big and flush into the hdd
    local_map.insert (std::pair<nodeNew::index, NodeNew * >(n->get_index(),n));

    //Add memory
    std::cout << "Add node: index: " << n->get_index() << " get depth: " << n->getDepth() << std::endl;
}

void NodesMap::removeNode(nodeNew::index idx) {
    local_map.erase(idx);
}

NodeNew * NodesMap::getNode(nodeNew::index idx) {
    return nullptr;
}

//Load the data from the memory database to the local map
bool NodesMap::readFromMemory() {

    std::cout << "Reading data from the db" << std::endl;

    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        leveldb::Slice key = it->key();
        leveldb::Slice value = it->value();
        //std::string key_str = key.ToString();
        //std::string val_str = value.ToString();
        //unsigned int a = (key.data()[3] << 24) | (key.data()[2] << 16) | (key.data()[1] << 8) | (key.data()[0]);
        //unsigned long a = (key.data()[0] << 56) | (key.data()[1] << 48) | (key.data()[2] << 40) | (key.data()[3] << 32)| (key.data()[4] << 24)| (key.data()[5] << 16)| (key.data()[6] << 8)| (key.data()[7]);
        //unsigned long a2 = (key.data()[7] << 56) | (key.data()[6] << 48) | (key.data()[5] << 40) | (key.data()[4] << 32)| (key.data()[3] << 24)| (key.data()[2] << 16)| (key.data()[1] << 8)| (key.data()[0]);
        //unsigned int f;
        unsigned long d = *key.data();
        //memcpy(&a, &key, sizeof(unsigned int));
        printf("Read key: %u\n", d);
        NodeNew *n3 = new NodeNew();
        local_map.insert (std::pair<nodeNew::index, NodeNew * >(d,n3));
    }
    assert(it->status().ok());  // Check for any errors found during the scan
    delete it;


    return false;
}

//write all the map into a memory DataBase
bool NodesMap::writeToMemory() {
    //take all the element into the levelDB
    std::cout << "Writing element into the memory\n" << std::endl;
    // Map iterator
    for (std::map<nodeNew::index , NodeNew * >::iterator it=local_map.begin(); it!=local_map.end(); ++it){


        unsigned int size = it->second->get_bytes_size();
        std::cout << "Size: " << size << std::endl;

        char data[size];
        it->second->serialize(data);

        char key[sizeof(nodeNew::index)];

        memcpy(key, &it->first, sizeof(nodeNew::index));
        printf("Pre inseriment the key is: %u \n", *key);
        leveldb::Status s = db->Put(leveldb::WriteOptions(), key, data);

        //unsigned long id;
        //leveldb::Status s = db->Get(leveldb::ReadOptions(), key, &id)

    }


    return true;
}


