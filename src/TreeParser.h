//
// Created by root on 4/29/18.
//
#include <string>
#include "NodeInfo.h"

#ifndef VISUALSUFFIXTREE_TREEPARSER_H
#define VISUALSUFFIXTREE_TREEPARSER_H

using namespace std;
using namespace sdsl;

typedef cst_sct3<> cst_t;

typedef cst_bfs_iterator<cst_t> iterator1;

class node_type;

class TreeParser {

public:

    cst_t cst;                              //declare the suffix tree

    bool VERBOSE;

    TreeParser(char *inputFileName, char *outputFileName, map<string, string> *configParameter);

    void printBinFile(string &s, std::ofstream &bin_out);

    void printNode(NodeInfo *nodeInfo, std::ofstream *bin_out);

    string getEdge(cst_t *cst, iterator1 *it);

    vector<cst_t::char_type> alphabet;

    double p_pnorm_parameter;


private:
    long numberOfNode = 0;

    int tree_max_depth = 0;

    bool checkNumberOfBit(int nBit, NodeInfoStructure *nodeInfoStructure);

    void setAlphabet(char *inputFileName, TreeParser *treeParser);

    string print_node_info(cst_t *cst, iterator1 *it) {
        string msg = "\n\n\nNodeDepth: " + to_string(cst->node_depth(**it)) + " Depth: " + to_string(cst->depth(**it)) +
                     "-[" + to_string(cst->lb(**it)) +
                     "-" + to_string(cst->rb(**it)) +
                     "]";//<< "\nAll String length: " << allstring_length << " parent length: " << parent_strLength << "\nEdge: " << edge <<"\nEdge coded: " << e.edgeToString(&edge) << std::endl;

        return msg;
    }

    //todo
    ///* ogni tanto lancia arithmetic exeptio... sistemare probabilemente aggiungere controlli sulla divisione*/
    void print_percentage(long *counter, int *percentage, int *percentageOld, long *numberOfNode) {


        *percentageOld = *percentage;
        if(*numberOfNode < 1){
            std::cout << "ooooooo è minore di 1!!!" << std::endl;
        }
        *percentage = (*counter * 100) / *numberOfNode;


        if (*percentage != *percentageOld) {
            std::cout << *percentage << "%" << std::endl;
        }
        *counter = *counter + 1;
    }

    unsigned long nF( iterator1 *it){

        return cst.rb(**it) - cst.lb(**it) + 1;

    }

    float kl_divergence(iterator1 *it) {

        if (cst.node_depth(**it) == 0 || cst.is_leaf(**it)){
            return 0;
        }

        float kl = 0;

        float f_aWb = 0;        //frequenza figlio
        float f_aW = nF(it);    //frequenza nodo
        float f_Wb = 0;         //frequenza figlio suffix link
        float f_W = (cst.rb( cst.sl(**it))) - (cst.lb( cst.sl(**it))) + 1; //frequenza suffix link

        for (auto &child: cst.children(**it)) {

            f_aWb = (cst.rb(child)) - (cst.lb(child)) + 1;
            f_Wb = (cst.rb( cst.child(cst.sl(**it), cst.edge(child, cst.depth(**it) +1))  )) - (cst.lb( cst.child(cst.sl(**it), cst.edge(child, cst.depth(**it) +1)) )) + 1;

            kl += f_aWb * log(((f_aWb)/(f_aW))/((f_Wb)/(f_W)));

        }

        return kl;
    }

    float p_norm(iterator1 *it) {
        return nF(it) * p_normNoparam(it);
    }

    float p_normNoparam(iterator1 *it) {

        if (cst.node_depth(**it) == 0 || cst.is_leaf(**it)){
            return 0;
        }

        float p = 0;

        float f_aWb = 0; //frequenza figlio
        float f_aW = nF(it); //frequenza nodo
        float f_Wb = 0; //frequenza figlio suffix link
        float f_W = (cst.rb( cst.sl(**it))) - (cst.lb( cst.sl(**it))) + 1; //frequenza suffix link

        for (auto &child: cst.children(**it)) {

            f_aWb = (cst.rb(child)) - (cst.lb(child)) + 1;
            f_Wb = (cst.rb( cst.child(cst.sl(**it), cst.edge(child, cst.depth(**it) +1))  )) - (cst.lb( cst.child(cst.sl(**it), cst.edge(child, cst.depth(**it) +1)) )) + 1;

            p += pow( abs(((f_aWb)/(f_aW))-((f_Wb)/(f_W))) , p_pnorm_parameter);

        }

        return pow(p, 1/p_pnorm_parameter);
    }

    float entropy(iterator1 *it) {

        float f_Wb = 0; //frequenza figlio
        float f_W = nF(it); //frequenza nodo

        float h = 0;

        for (auto &child: cst.children(**it)) {

            f_Wb = (cst.rb(child)) - (cst.lb(child)) + 1;

            h += ((f_Wb)/(f_W)) * log((f_Wb)/(f_W));

        }

        return -h;
    }



    float entropySpecial(iterator1 *it) {

        float a = nF(it)*entropy(it);

        float f_aW = 0; //frequenza winer link

        unsigned long t;
        cst_t::char_type c;
        for (int i = 0; i < alphabet.size(); i++) {
            c = alphabet[i];
            t = cst.id(cst.wl(**it, c));

            if (t != numberOfNode) { // == number of node when there isn't a valid wl

                f_aW = (cst.rb(cst.wl(**it, c))) - (cst.lb(cst.wl(**it, c))) + 1;


                float f_Wb = 0; //frequenza figlio
                float f_W = cst.rb(cst.wl(**it, c)) - cst.lb(cst.wl(**it, c)) + 1; //frequenza nodo

                float h = 0;

                for (auto &child: cst.children(cst.wl(**it, c))) {

                    f_Wb = (cst.rb(child)) - (cst.lb(child)) + 1;

                    h += ((f_Wb)/(f_W)) * log((f_Wb)/(f_W));

                }



                a = a - f_aW * h;

            }
        }

        return a;

    }



};

#endif //VISUALSUFFIXTREE_TREEPARSER_H
