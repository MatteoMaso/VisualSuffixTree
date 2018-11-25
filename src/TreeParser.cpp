#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset> //to use quickly bit representation
#include <vector>
#include <fstream>

#include "TreeParser.h"
#include "BitIo.h"
#include "ConfigParser.h"
#include "Header.h"
#include "NodeInfo.h"
#include "nodeMap/NodesMap.h"
#include <iostream>
using namespace std;
using namespace sdsl;

typedef cst_sct3<> cst_t;


TreeParser::TreeParser(char * inputFileName, char * outputFileName, map<string, string> * configParameter) {

    //new alphabet, i should check if the getAlphabet return true otherwise some error happened
    vector<char> * new_alphabet = new vector<char>;
    getAlphabet(inputFileName, new_alphabet);
    //todo (control) check if the character is ok for a dna or somthing like thet string
    std::cout << *new_alphabet << std::endl;


    //SUFFIX TREE STRUCTURE
    construct(cst, inputFileName, 1);                       //initialize the suffix tree
    typedef cst_bfs_iterator<cst_t> iterator;
    iterator begin = iterator(&cst, cst.root());
    iterator end = iterator(&cst, cst.root(), true, true);

    std::cout << "finish tree costruction" << std::endl;

    /*
    string originalString;
    ifstream file(inputFileName);
    if (file.is_open())
        while (file.good())
            getline(file, originalString);
    file.close();
    */

    p_pnorm_parameter = stod(configParameter->at("STATISTIC_PNORM_PARAMETER")); //P-parameter user set;

    //PREPARE THE OUTPUT FILE AND PARAMETER
    //std::ofstream bin_out(outputFileName, std::ios::out | std::ios::binary);

    //NodeInfoStructure nodeInfoStructure(configParameter, inputFileName);

    //Header header(&nodeInfoStructure);
    //string headerString = header.getString();
    //printBinFile(headerString, bin_out);        //Print the header into binary file

    //BitIo<16> bio;

    //string nodeInfo;
    //NodeInfo nodeInfoObj(&nodeInfoStructure, &originalString);

    //Check se il numero di bit sono sufficienti per rappresentare le informazione
    /*
    long p = cst.rb(*begin);
    int nBit = log10(p) / log10(2);
    if (!checkNumberOfBit(nBit, &nodeInfoStructure)) {
        exit(-1);
    } */

    numberOfNode = cst.id(cst.root());
    std::cout << "Number of Node: " << numberOfNode << std::endl;


    csa_wt<> csa = cst.csa;

    //MAP OF THE WINER LINK key = index of the alphabet character
    map<int, nodeNew::index > wl;

    NodesMap my_map = NodesMap(outputFileName);

    for (iterator1 it = begin; it != end; ++it) {

        //create the node in this way is memory expensive
        //maybe I could have one unique node, add the element and in each loop save just his
        //serialization version.. todo think about
        NodeNew *  nodeNew1 = new NodeNew(); //todo (optional) non creare inside , it depends

        //add element
        nodeNew1->set_index((cst.id(*it)));
        nodeNew1->setDepth(cst.depth(*it));
        nodeNew1->setNodeDepth(cst.node_depth(*it));
        nodeNew1->setLb(cst.lb(*it));
        nodeNew1->setRb(cst.rb(*it));
        nodeNew1->setLabel(cst.id(*it));
        nodeNew1->setFatherLabel(cst.id(cst.parent(*it)));
        nodeNew1->setEdgeLen(cst.depth(*it)-cst.depth(cst.parent(*it)));
        nodeNew1->setEdgeIdx(cst.sn(cst.leftmost_leaf(*it))+cst.depth(cst.parent(*it)));
        //SET CHILDREN ID //todo (optional) make it better
        vector<nodeNew::index> childrenID; //support structure
        for (auto &child: cst.children(*it)) {
            childrenID.push_back(cst.id(child));
        }
        nodeNew1->setChildren(&childrenID);

        //ADD WINER LINK
        wl.clear();
        for (int i = 0; i < new_alphabet->size(); i++) {
            const cst_t::char_type c = new_alphabet->at(i);
            nodeNew::index t = cst.id(cst.wl(*it, c));

            if (t != numberOfNode) { // == number of node when there isn't a valid wl
                pair<int, unsigned long> pair = {i, t};
                wl.insert(pair);
            }
        }
        nodeNew1->setWinerLink(&wl);


        //ADD SET STATISTIC
        nodeNew1->setKlDivergence(kl_divergence(&it));
        nodeNew1->setPNorm(p_norm(&it));
        nodeNew1->setPNormNoParam(p_normNoparam(&it));
        nodeNew1->setHEntropy(entropy(&it));
        nodeNew1->setHEntropy2(entropySpecial(&it));


        //For calculate the max depth of the tree
        //todo (optional) check a smart way to do it
        if (cst.is_leaf(*it)){
            if ( cst.node_depth(*it) > tree_max_depth){
                tree_max_depth = cst.node_depth(*it);
            }
        }


        my_map.addNode(nodeNew1);

    }

    my_map.showContent();

    std::cout << "Tree max depth: " << tree_max_depth << std::endl;

};



/*
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
*/

/*
void TreeParser::printNode(NodeInfo *nodeInfo, std::ofstream *bin_out) {

    string nodeInfoFromObj = nodeInfo->getNodeField();
    int nodeInfoLength = nodeInfoFromObj.size() / 16;
    if (nodeInfoFromObj.size() % 16 != 0) nodeInfoLength++;
    string length = std::bitset<16>(nodeInfoLength).to_string();
    string completeString = length + nodeInfoFromObj;

    if (VERBOSE) {
        std::cout << completeString << std::endl;
    }

    printBinFile(completeString, *bin_out);
}
 */

/*
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
*/

/*
bool TreeParser::checkNumberOfBit(int nBit, NodeInfoStructure *nodeInfoStructure) {
    if (
            nodeInfoStructure->getBitDepth() <= nBit ||
            nodeInfoStructure->getBitNodeDepth() <= nBit ||
            nodeInfoStructure->getBitRb() <= nBit ||
            nodeInfoStructure->getBitLb() <= nBit) {
        std::cout << "You need at least " << nBit + 1 << " for depth, nodedepth, lb, rb, label, fatherLabel"
                  << std::endl;
        return false;
    }
    return true;
}
 */

/*
bool contains2(vector<char> *character, char c) {
    //todo spostare da qualche altra parte
    for (int i = 0; i < character->size(); i++) {
        if (character->at(i) == c) {
            return true;
        }
    }

    return false;
}
 */

/*
void TreeParser::setAlphabet(char *inputFileName, TreeParser *treeParser) {
    //todo spostare da qualche altra parte

    string txt;
    ifstream file(inputFileName);

    if (file.is_open())
        while (file.good())
            getline(file, txt);
    file.close();

    vector<char> character = {};
    char c;
    for (int i = 0; i < txt.length(); i++) {
        c = txt[i];
        if (!contains2(&character, c)) {
            character.push_back(c);
        }
    }

    string alp = "$";

    for (int j = 0; j < character.size(); j++) {
        cst_t::char_type c = character.at(j);
        treeParser->alphabet.push_back(c);
    }

}
*/