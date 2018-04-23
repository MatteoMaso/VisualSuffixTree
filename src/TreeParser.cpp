#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset> //to use quickly bit representation
#include <vector>
#include <fstream>

#include "../Include/TreeParser.h"
#include "../Include/BitIo.h"
#include "../Include/BitEncode.h"

using namespace std;
using namespace sdsl;

typedef cst_sct3<> cst_t;

void printBinFile(string &s, std::ofstream &bin_out);

string charEncoding(char &c, vector <string> &a, string &inputLine);


TreeParser::TreeParser(char *inputFileName, char *outputFileName) {

    std::cout << "Insert the alphabet: ABC or ARFS ... etc" << std::endl; //
    string inputLine;
    getline(cin, inputLine);
    cout << inputLine.length() << endl;

    vector <string> charCoding; //Vuoto per ora

    //TODO RENDERLO PARAMETRICO
    if (inputLine.length() <= 2){
        //1 bit
        charCoding.emplace_back("0");
        charCoding.emplace_back("1");

    } else if ( inputLine.length() > 2 && inputLine.length() <= 4){
//        2 bit
        charCoding.emplace_back("00");
        charCoding.emplace_back("01");
        charCoding.emplace_back("10");
        charCoding.emplace_back("11");
    } else if ( inputLine.length() > 4 && inputLine.length() <= 8){
        //3 bit
        charCoding.emplace_back("000");
        charCoding.emplace_back("001");
        charCoding.emplace_back("010");
        charCoding.emplace_back("011");
        charCoding.emplace_back("100");
        charCoding.emplace_back("101");
        charCoding.emplace_back("110");
        charCoding.emplace_back("111");
    } else {
        std::cout << "To much alphabet character! Todo implement" << std::endl; //
        exit(1);
    }



    for (int j = 0; j < inputLine.length(); j++) {
        std::cout << "Char: " << inputLine[j] << " Encoded: " << charCoding[j] << std::endl; //
    }



    const int bitCharacter = 2;


    //Devo aggiornarlo ogni volta che aggiungo un nodo
    int nodeCounter = 0; //Contatore del numero di nodi così so quanto spazio occupa il file e quanto spazio potrei salvare

    cst_t cst;                              //declare the suffix tree
    construct(cst, inputFileName, 1);       //initialize the suffix tree

    typedef cst_bfs_iterator<cst_t> iterator;
    iterator begin = iterator(&cst, cst.root());
    iterator end = iterator(&cst, cst.root(), true, true);

    int str_length = 0;

    string nodeInfo;

    std::ofstream bin_out(outputFileName, std::ios::out | std::ios::binary);
    BitIo<16> bio;

    BitEncode e;

    for (iterator it = begin; it != end; ++it) {

        //Resetto la stringa del nodo info
        nodeInfo = "";

        //Per ogni nodo stampo le sue proprità

        nodeInfo += e.nodeDepthToString(cst.node_depth(*it));   //Nodedepth
        nodeInfo += e.depthToString(cst.depth(*it));            //Depth
        nodeInfo += e.lbToString(cst.lb(*it));                  //Lb
        nodeInfo += e.rbToString(cst.rb(*it));                  //Rb


        str_length = (int)cst.depth(*it);



        //TODO DA STAMPARE SOLO IL RAMO E NON TUTTA LA STRINGA
        string edge;

        if ((cst.node_depth(*it) == 0) ||
            (cst.node_depth(*it) == 1 && str_length == 1 && (cst.lb(*it) == cst.rb(*it)))) {
            edge = "-";
        } else {

            if (cst.lb(*it) == cst.rb(*it)) {
                //leaf
                edge = "";

                for (int i = 1; i < str_length - 1; i++) {
                    edge += cst.edge(*it, i);
                }

                edge += "#";
                edge += "#";

            } else {
                //internal node
                edge = "";

                for (int i = 1; i <= str_length; i++) {
                    edge += cst.edge(*it, i);
                }
            }
        }

        std::cout << "NodeDepth: " << cst.node_depth(*it) << " Depth: " << cst.depth(*it) << "-[" << cst.lb(*it) << "-"
                  << cst.rb(*it) << "]" << std::endl;


        //TODO DEVO STAMPARE PRIMA IL NUMERO DI CARATTERI DELL'EDGE COSI POI IN LETTURA SO QUANTI LEGGERNE
//        nodeInfo += std::bitset<bitRb>(edge.length()).to_string();

        //todo togliere gli hashtag o tenerne conto di default

        //todo decommentare quando saro elaborare le stringhe del edge dall'SVG
//        for (int k = 0; k < edge.length(); k++) {
//            nodeInfo += std::bitset<bitRb>(charEncoding(edge[k], charCoding, inputLine)).to_string();
////            std::cout << charEncoding(edge[k], charCoding, inputLine) << std::endl; //
//        }

        printBinFile(nodeInfo, bin_out);

    }

    bin_out.close();
};

void printBinFile(string &s, std::ofstream &bin_out) {

    BitIo<16> bio;

    for (int j = 0; j <= s.length() / 16; j++) {

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


string charEncoding(char &c, vector <string> &a, string &inputLine){

    for (int i = 0; i < inputLine.size(); i++) {
        char temp = inputLine[i];
        if ( c == temp ){
            return a[i];
        }
    }


    std::cout << "Errore in charEncoding, carattere: "<< c << " non trovato!" << std::endl; //
    exit(1);

};