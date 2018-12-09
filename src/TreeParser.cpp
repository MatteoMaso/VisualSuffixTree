#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset> //to use quickly bit representation
#include <vector>
#include <fstream>

#include "TreeParser.h"
#include "ConfigParser.h"
#include "nodeMap/NodesMap.h"
#include <iostream>

typedef cst_sct3<> cst_t;

TreeParser::TreeParser(char * inputFileName, char * outputFileName, map<string, string> * configParameter) {

    //new alphabet, i should check if the getAlphabet return true otherwise some error happened
    auto * new_alphabet = new vector<char>;
    getAlphabet(inputFileName, new_alphabet);
    std::cout << *new_alphabet << std::endl;

    //SUFFIX TREE STRUCTURE
    construct(cst, inputFileName, 1);                       //initialize the suffix tree
    typedef cst_bfs_iterator<cst_t> iterator;
    iterator begin = iterator(&cst, cst.root());
    iterator end = iterator(&cst, cst.root(), true, true);

    std::cout << "finish tree constructions" << std::endl;

    p_pnorm_parameter = stod(configParameter->at("STATISTIC_PNORM_PARAMETER")); //P-parameter user set;


    numberOfNode = cst.id(cst.root());
    std::cout << "Number of Node: " << numberOfNode << std::endl;


    csa_wt<> csa = cst.csa;

    NodesMap my_map = NodesMap(outputFileName, "w");

    for (iterator1 it = begin; it != end; ++it) {

        //create the node in this way is memory expensive
        //maybe I could have one unique node, add the element and in each loop save just his
        //serialization version.. todo think about
        NodeNew *  nodeNew1 = new NodeNew(); //todo (optional) non creare inside , it depends

        //Add element
        if(cst.id(*it) != 0){
            std::cout << "set index: " << cst.id(*it) << std::endl;
            nodeNew1->set_index((cst.id(*it)));
            nodeNew1->setDepth(cst.depth(*it));
            nodeNew1->setNodeDepth(cst.node_depth(*it));
            nodeNew1->setLb(cst.lb(*it));
            nodeNew1->setRb(cst.rb(*it));
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
            map<int, nodeNew::index > wl;
            for (int i = 0; i < new_alphabet->size(); i++) {
                const cst_t::char_type c = new_alphabet->at(i);
                nodeNew::index t = cst.id(cst.wl(*it, c));
                !!!capire come mai non si salva la mappa di wl!!!
                if (t != numberOfNode) { // == number of node when there isn't a valid wl
                    pair<int, unsigned long> pair = {i, t};
                    wl.insert(pair);
                    std::cout << "add wl" << std::endl;
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



    }


    //my_map.showContent();

    std::cout << "Tree max depth: " << tree_max_depth << std::endl;
};