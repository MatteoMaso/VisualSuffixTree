//
// Created by root on 11/11/18.
//

#ifndef VISUALSUFFIXTREE_NODE_H
#define VISUALSUFFIXTREE_NODE_H

//todo this is a new class for the Node

namespace nodeNew{

    typedef int index;
}

class NodeNew {

public:

    NodeNew();         //Normal constructor

    ~NodeNew();        //Deconstructor

    NodeNew(char * raw_data);  //constructor from the raw_data, the same produced from the serialize

    int serialize(char * buffer); //put the data of the node in a series of bytes in the buffer and return the length of bytes used

    //!!!!!! check how to pass the object created to the map class in order to avoid the space wasting ...

private:

    nodeNew::index index;
    //todo add all the data that I want to save

};


#endif //VISUALSUFFIXTREE_NODE_H
