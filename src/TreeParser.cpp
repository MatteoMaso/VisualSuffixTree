#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset> //to use quickly bit representation
#include <vector>
#include <fstream>

#include "../Include/TreeParser.h"

using namespace std;
using namespace sdsl;

typedef cst_sct3<> cst_t;

/**
 * Class for binary file manipulation
 */
template<std::size_t N>
class BitIo {
public:

    void push_back(const std::bitset<N> &bs) {
        std::vector<Byte> result((N + 7) >> 3);
        for (int j = 0; j < int(N); ++j) {
            result[j >> 3] |= (bs[j] << (j & 7));
        }
        for (const Byte &byte : result) {
            bytes.push_back(byte);
        }
        num_bytes += NUM_BYTES_PER_BITSET;
    }

    std::bitset<N> pop_front() {
        std::bitset<N> result;
        for (int j = 0; j < int(N); ++j) {
            result[j] = ((bytes[(j >> 3) + offset] >> (j & 7)) & 1);
        }
        offset += NUM_BYTES_PER_BITSET;
        num_bytes -= NUM_BYTES_PER_BITSET;
        return result;
    }

    bool empty() {
        return num_bytes < NUM_BYTES_PER_BITSET;
    }

    void clear() {
        bytes.clear();
        num_bytes = 0;
    }

    std::size_t size() {
        return num_bytes;
    }

private:

    using Byte = unsigned char;
    static constexpr std::size_t NUM_BYTES_PER_BITSET = N / 8;

    template<std::size_t T>
    friend std::ostream &operator<<(std::ostream &os, const BitIo<T> &bio);

    template<std::size_t T>
    friend std::istream &operator>>(std::istream &is, BitIo<T> &bio);

    std::istream &read_file(std::istream &is) {
        bytes.clear();

        std::streampos current_pos, file_size;
        current_pos = is.tellg();
        is.seekg(0, std::ios::end);
        file_size = is.tellg() - current_pos;
        is.seekg(current_pos, std::ios::beg);

        bytes.resize(file_size);
        is.read((char *) &bytes[0], file_size);

        num_bytes += file_size;

        return is;
    }

    std::vector<Byte> bytes;
    std::size_t offset = 0;
    std::size_t num_bytes = 0;
};

template<std::size_t N>
std::ostream &operator<<(std::ostream &os, const BitIo<N> &bio) {
    for (const auto &byte : bio.bytes) {
        os << byte;
    }
    return os;
}

template<std::size_t N>
std::istream &operator>>(std::istream &is, BitIo<N> &bio) {
    if (!is) {
        is.setstate(std::ios::failbit);
    }
    bio.read_file(is);
    return is;
}


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
        charCoding.push_back("0");
        charCoding.push_back("1");

    } else if ( inputLine.length() > 2 && inputLine.length() <= 4){
//        2 bit
        charCoding.push_back("00");
        charCoding.push_back("01");
        charCoding.push_back("10");
        charCoding.push_back("11");
    } else if ( inputLine.length() > 4 && inputLine.length() <= 8){
        //3 bit
        charCoding.push_back("000");
        charCoding.push_back("001");
        charCoding.push_back("010");
        charCoding.push_back("011");
        charCoding.push_back("100");
        charCoding.push_back("101");
        charCoding.push_back("110");
        charCoding.push_back("111");
    } else {
        std::cout << "To much alphabet character! Todo implement" << std::endl; //
        exit(1);
    }



    for (int j = 0; j < inputLine.length(); j++) {
        std::cout << "Char: " << inputLine[j] << " Encoded: " << charCoding[j] << std::endl; //
    }



    //Lunghezza dei parametri, poi dovrò settarli da interfaccia e dovranno essere comuni anche al file che crea l'svg per essere in grado di leggerlo
    const int bitNodeDepth = 10; //fino a 1024
    const int bitDepth = 10;
    const int bitLb = 8;
    const int bitRb = 8;

    const int bitCharacter = 2;

    //Devo aggiornarlo ogni volta che aggiungo un nodo
    int nodeCounter = 0; //Contatore del numero di nodi così so quanto spazio occupa il file e quanto spazio potrei salvare

    cst_t cst;                              //declare the suffix tree
    construct(cst, inputFileName, 1);       //initialize the suffix tree

    typedef cst_bfs_iterator<cst_t> iterator;
    iterator begin = iterator(&cst, cst.root());
    iterator end = iterator(&cst, cst.root(), true, true);

    int str_length = 0;

    string nodeInfo = "";

    std::ofstream bin_out(outputFileName, std::ios::out | std::ios::binary);
    BitIo<16> bio;

    for (iterator it = begin; it != end; ++it) {

        //Resetto la stringa del nodo info
        nodeInfo = "";

        //Per ogni nodo stampo le sue proprità
        //Print Node_depth  [0-?]
        unsigned long i = cst.node_depth(*it);
        nodeInfo += std::bitset<bitNodeDepth>(cst.node_depth(*it)).to_string();


        nodeInfo += std::bitset<bitDepth>(cst.depth(*it)).to_string();

        nodeInfo += std::bitset<bitLb>(cst.lb(*it)).to_string();

        nodeInfo += std::bitset<bitRb>(cst.rb(*it)).to_string();


        str_length = cst.depth(*it);



        //TODO DA STAMPARE SOLO IL RAMO E NON TUTTA LA STRINGA
        string edge = "";

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

//        std::cout << "NodeDepth: " << cst.node_depth(*it) << " Depth: " << cst.depth(*it) << "-[" << cst.lb(*it) << ","
//                  << cst.rb(*it) << "]" << edge << std::endl;
//

        //TODO DEVO STAMPARE PRIMA IL NUMERO DI CARATTERI DELL'EDGE COSI POI IN LETTURA SO QUANTI LEGGERNE
//        nodeInfo += std::bitset<bitRb>(edge.length()).to_string();

        //todo togliere gli hashtag o tenerne conto di default

        //todo decommentare quando saro elaborare le stringhe del edge dall'SVG
        for (int k = 0; k < edge.length(); k++) {
            nodeInfo += std::bitset<bitRb>(charEncoding(edge[k], charCoding, inputLine)).to_string();
//            std::cout << charEncoding(edge[k], charCoding, inputLine) << std::endl; //
        }



        printBinFile(nodeInfo, bin_out);

    }

    bin_out.close();
};

void printBinFile(string &s, std::ofstream &bin_out) {

    BitIo<16> bio;

    for (int j = 0; j <= s.length() / 16; j++) {

        string temp = "";

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