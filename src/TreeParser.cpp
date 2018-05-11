#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset> //to use quickly bit representation
#include <vector>
#include <fstream>

#include "../Include/TreeParser.h"
#include "../Include/BitIo.h"
#include "../Include/ConfigParser.h"
#include "../Include/Header.h"
#include "../Include/NodeInfo.h"

#define VERBOSE 0

using namespace std;
using namespace sdsl;

typedef cst_sct3<> cst_t;


TreeParser::TreeParser(char *inputFileName, char *outputFileName, map<string, string> *configParameter) {

    //SUFFIX TREE STRUCTURE
    cst_t cst;                              //declare the suffix tree
    construct(cst, inputFileName, 1);       //initialize the suffix tree
    typedef cst_bfs_iterator<cst_t> iterator;
    iterator begin = iterator(&cst, cst.root());
    iterator end = iterator(&cst, cst.root(), true, true);

    //PREPARE THE OUTPUT FILE AND PARAMETER
    std::ofstream bin_out(outputFileName, std::ios::out | std::ios::binary);

    NodeInfoStructure nodeInfoStructure(configParameter);

    Header header(&nodeInfoStructure);
    string headerString = header.getString();
    printBinFile(headerString, bin_out);        //Print the header into binary file

    BitIo<16> bio;

    string nodeInfo;
    NodeInfo nodeInfoObj(&nodeInfoStructure);

    //Check se il numero di bit sono sufficienti per rappresentare le informazione
    long p = cst.rb(*begin);
    int nBit = log10(p)/log10(2);
    if ( !checkNumberOfBit(nBit, &nodeInfoStructure)){
        exit(-1);
    }


    for (iterator it = begin; it != end; ++it) {

        nodeInfoObj.setDepth(cst.depth(*it));
        nodeInfoObj.setNodeDepth(cst.node_depth(*it));
        nodeInfoObj.setLb(cst.lb(*it));
        nodeInfoObj.setRb(cst.rb(*it));

        if (nodeInfoStructure.OPT_LABEL){
            nodeInfoObj.setLabel(cst.id(*it));
        }

        if (nodeInfoStructure.OPT_FATHERLABLE){
            nodeInfoObj.setFatherLabel(cst.id(cst.parent(*it)));
        }

        if( nodeInfoStructure.OPT_EDGEINFO){
            string new_edge = getEdge(&cst, &it);
            nodeInfoObj.setEdge(&new_edge);
        }

        if ( nodeInfoStructure.OPT_CHILDREN_INFO){
            //SET CHILDREN ID
            vector<int> childrenID; //support structure
            for (auto &child: cst.children(*it)) {
                childrenID.push_back(cst.id(child));
            }
            nodeInfoObj.setChildrenId(&childrenID);
        }


#if VERBOSE == 1
        std::cout << "\n\n\nNodeDepth: " << cst.node_depth(*it) << " Depth: " << cst.depth(*it) << "-[" << cst.lb(*it)
                  << "-"
                  << cst.rb(*it) << "]"
                  << std::endl;//<< "\nAll String length: " << allstring_length << " parent length: " << parent_strLength << "\nEdge: " << edge <<"\nEdge coded: " << e.edgeToString(&edge) << std::endl;

        std::cout << nodeInfoObj.print() << std::endl;
#endif

        //PRINT THE NODE INFO INTO BINARY FILE
        printNode(&nodeInfoObj, &bin_out);

    }

    bin_out.close();
};


void TreeParser::printBinFile(string &s, std::ofstream &bin_out) {

    BitIo<16> bio;
    int counter = s.length() / 16;

    if ((s.length() % 16) != 0) {
        counter++;
    }

    for (int j = 0; j < counter; j++) {

        string temp;

        for (int k = 0 + j * 16; k <= 15 + j * 16; k++) {
            if (k >= s.length()) {
                temp += "0";
            } else {
                temp += s[k];
            }
        }

        bio.push_back(std::bitset<16>(temp));
    }

    bin_out << bio;
};


void TreeParser::printNode(NodeInfo *nodeInfo, std::ofstream *bin_out) {

    string nodeInfoFromObj = nodeInfo->getNodeField();
    int nodeInfoLength = nodeInfoFromObj.size() / 16;
    if (nodeInfoFromObj.size() % 16 != 0) nodeInfoLength++;
    string length = std::bitset<16>(nodeInfoLength).to_string();
    string completeString = length + nodeInfoFromObj;

#if VERBOSE == 1
    std::cout << completeString << std::endl;
#endif

    printBinFile(completeString, *bin_out);


}

string TreeParser::getEdge(cst_t *cst, iterator1 *it) {

    int allstring_length = (int) cst->depth(**it); //Lunghezza dell suffisso dalla radice al nodo interessato
    int parent_strLength = (int) cst->depth(cst->parent(**it));

    string edge;

    if ((cst->node_depth(**it) == 0) ||
        (cst->node_depth(**it) == 1 && allstring_length == 1 && (cst->lb(**it) == cst->rb(**it)))) {
        edge = "$$$";
    } else {

        if (cst->lb(**it) == cst->rb(**it)) {
            //leaf
            edge = "";

            for (int i = parent_strLength + 1; i < allstring_length; i++) {
                edge += cst->edge(**it, i);
            }

            edge += "$";

        } else {
            //internal node
            edge = "";

            for (int i = parent_strLength + 1; i <= allstring_length; i++) {
                edge += cst->edge(**it, i);
            }
        }
    }

    return edge;

}

bool TreeParser::checkNumberOfBit(int nBit, NodeInfoStructure * nodeInfoStructure){
    if (
            nodeInfoStructure->getBitDepth() <= nBit ||
            nodeInfoStructure->getBitNodeDepth() <= nBit ||
            nodeInfoStructure->getBitRb() <= nBit ||
            nodeInfoStructure->getBitLb() <= nBit ){
        std::cout << "You need at least " << nBit + 1<< " for depth, nodedepth, lb, rb, label, fatherLabel" << std::endl;
        return false;
    }

    //todo add check label and father label

    return true;
}
