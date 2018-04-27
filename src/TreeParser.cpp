#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset> //to use quickly bit representation
#include <vector>
#include <fstream>

#include "../Include/TreeParser.h"
#include "../Include/BitIo.h"
#include "../Include/BitEncode.h"
#include "../Include/ConfigParser.h"

using namespace std;
using namespace sdsl;

typedef cst_sct3<> cst_t;


TreeParser::TreeParser(char *inputFileName, char *outputFileName) {

    map<string, string> configParameter;
    ConfigParser cfPars("./Settings/config.cfg", &configParameter); //Initialize the configurations Parameter

    //Devo aggiornarlo ogni volta che aggiungo un nodo
    int nodeCounter = 0; //Contatore del numero di nodi così so quanto spazio occupa il file e quanto spazio potrei salvare

    cst_t cst;                              //declare the suffix tree
    construct(cst, inputFileName, 1);       //initialize the suffix tree

    typedef cst_bfs_iterator<cst_t> iterator;
    iterator begin = iterator(&cst, cst.root());
    iterator end = iterator(&cst, cst.root(), true, true);

    string nodeInfo;

    std::ofstream bin_out(outputFileName, std::ios::out | std::ios::binary);
    BitIo<16> bio;

    BitEncode e;

    //Create Header file
    string header = e.createHeader();
    printBinFile(header, bin_out);

    for (iterator it = begin; it != end; ++it) {

        //Resetto la stringa del nodo info
        nodeInfo = "";

        //Per ogni nodo stampo le sue proprità

        nodeInfo += e.nodeDepthToString(cst.node_depth(*it));   //Nodedepth
        nodeInfo += e.depthToString(cst.depth(*it));            //Depth
        nodeInfo += e.lbToString(cst.lb(*it));                  //Lb
        nodeInfo += e.rbToString(cst.rb(*it));                  //Rb


        int allstring_length = (int)cst.depth(*it); //Lunghezza dell suffisso dalla radice al nodo interessato
        int parent_strLength = (int)cst.depth(cst.parent(*it));

        string edge;

        if ((cst.node_depth(*it) == 0) ||
            (cst.node_depth(*it) == 1 && allstring_length == 1 && (cst.lb(*it) == cst.rb(*it)))) {
            edge = "$$$";
        } else {

            if (cst.lb(*it) == cst.rb(*it)) {
                //leaf
                edge = "";

                for (int i = parent_strLength + 1; i < allstring_length; i++) {
                    edge += cst.edge(*it, i);
                }

                edge += "$";

            } else {
                //internal node
                edge = "";

                for (int i = parent_strLength + 1; i <= allstring_length; i++) {
                    edge += cst.edge(*it, i);
                }
            }
        }


        nodeInfo += e.getEdgeLength(edge.size());  //Number of character into the edge
        nodeInfo += e.edgeToString(&edge);
////
////        std::cout << "NodeDepth: " << cst.node_depth(*it) << " Depth: " << cst.depth(*it) << "-[" << cst.lb(*it) << "-"
////                  << cst.rb(*it) << "]" << "\nAll String length: " << allstring_length << " parent length: " << parent_strLength << "\nEdge: " << edge <<"\nEdge coded: " << e.edgeToString(&edge) << std::endl;
////        //todo decommentare quando saro elaborare le stringhe del edge dall'SVG
//
//////        for (int k = 0; k < edge.length(); k++) {
//////            nodeInfo += std::bitset<bitRb>(charEncoding(edge[k], charCoding, inputLine)).to_string();
//////            std::cout << charEncoding(edge[k], charCoding, inputLine) << std::endl; //
//////        }
////
////        std::cout << "NodeInfo.sie(): " << nodeInfo.size() << std::endl; //
////        std::cout << "\n" << nodeInfo << std::endl;
//
        int len = nodeInfo.size() / 16;
        if (nodeInfo.size() % 16 != 0) len++;
        string length =  std::bitset<16>(len).to_string();

        string pi = length+nodeInfo;

        printBinFile(pi, bin_out);
    }

    bin_out.close();
};

void TreeParser::printBinFile(string &s, std::ofstream &bin_out) {

    BitIo<16> bio;
    int counter = s.length() / 16;

    if ((s.length() % 16) != 0){
        counter++;
    }

    for (int j = 0; j < counter; j++) {

        string temp;

        for (int k = 0 + j*16; k <= 15 + j*16; k++) {
            if (k >=  s.length()) {
                temp += "0";
            } else {
                temp += s[k];
            }
        }

        bio.push_back(std::bitset<16>(temp));
    }

    bin_out << bio;
};


string TreeParser::charEncoding(char &c, vector <string> &a, string &inputLine){

    for (int i = 0; i < inputLine.size(); i++) {
        char temp = inputLine[i];
        if ( c == temp ){
            return a[i];
        }
    }


    std::cout << "Errore in charEncoding, carattere: "<< c << " non trovato!" << std::endl; //
    exit(1);

};