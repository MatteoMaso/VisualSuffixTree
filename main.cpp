#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset> //to use quickly bit representation
#include <vector>

using namespace std;
using namespace sdsl;

typedef cst_sct3<> cst_t;

/**
 *
 * @param inputFileName file that contains the strings that you want to analyze
 */
void treeParser(char *inputFileName);

/**
 * This method take an input file well formatted and create a flameGraph as output
 * @param inputFileName formatted file to convert in FlameGraph SVG
 */
void createSvg(char *inputFileName);

void printBinFile(string &s, std::ofstream &bin_out) ;

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


int main(int argc, char *argv[]) {

//    std::ofstream bin_out("./bf.bin", std::ios::out | std::ios::binary);
//
//    BitIo<16> bio;
//
//    bio.push_back(std::bitset<16>("1001011010010110"));
//    bio.push_back(std::bitset<16>("0000000011111111"));
//    bio.push_back(std::bitset<16>("1111111100000000"));
//    bio.push_back(std::bitset<16>("0011001111001100"));
//
//    bin_out << bio;
//    bin_out.close(); // bf.bin is 8 bytes
//
//    std::ifstream bin_in("./bf.bin", std::ios::binary);
//
//    BitIo<16> bio2;
//    bin_in >> bio2;
//
//    while (!bio2.empty()) {
//        std::cout << bio2.pop_front() << std::endl; // Prints the 4 16-bit bitsets in correct order.
//    }


    if (argc < 2) {
        cout << "insert -data nameInput for starting treeParser or -svg to create the SVG file" << std::endl;
        return 1;
    }

    char firstProgram[6] = "-data";
    char secondProgram[5] = "-svg";

    if (strcmp(argv[1], firstProgram) == 0) {
        treeParser(argv[2]);
    } else if (strcmp(argv[1], secondProgram) == 0) {
        createSvg(argv[2]);
    } else {
        printf("Bad arguments!");
    }
}

string partitioner(string s, int from, int to){
    string a = "";

    for (int i = from; i <= to; ++i) {
        a += s[i];
    }

    return a;
}


void createSvg(char *inputFileName) {
    //BEGIN SVG CREATOR

    const int bitNodeDepth = 10; //fino a 1024
    const int bitDepth = 10;
    const int bitLb = 8;
    const int bitRb = 8;

    //Read input file
    //    std::ifstream bin_in(inputFileName, std::ios::binary); //usare questo per passaggio parametri
    std::ifstream bin_in("./Output/test.bin", std::ios::binary);

    BitIo<16> bio2;
    bin_in >> bio2;

    int nodeInfoDim = 36;
    int nodeInfoLength = 0;
    if ((nodeInfoDim % 16)  == 0){
        nodeInfoLength = nodeInfoDim / 16;
    } else {
        nodeInfoLength = nodeInfoDim / 16 + 1;
    }

    std::cout << nodeInfoLength << std::endl; // Prints the 4 16-bit bitsets in correct order.

    while (!bio2.empty()) {

        string nodeInfo = "";

        for (int i = 0; i < nodeInfoLength; ++i) {
            nodeInfo += bio2.pop_front().to_string();
        }

        string str = partitioner(nodeInfo, 0, 9);
        int x = stoi(partitioner(nodeInfo, 0, 9), nullptr, 2);


        std::cout << "NodeDepth: " << stoi(partitioner(nodeInfo, 0, 9), nullptr, 2) << " Depth: " << stoi(partitioner(nodeInfo, 10, 19), nullptr, 2) << "-[" << stoi(partitioner(nodeInfo, 20, 27), nullptr, 2) << ","
                  << stoi(partitioner(nodeInfo, 28, 35), nullptr, 2) << "]" <<  std::endl;

        std::cout << nodeInfo << std::endl; //

    }

};

/**
 * FORMAT OUTPUT FILE
 */
void treeParser(char *inputFileName) {

    //Lunghezza dei parametri, poi dovrò settarli da interfaccia e dovranno essere comuni anche al file che crea l'svg per essere in grado di leggerlo
    const int bitNodeDepth = 10; //fino a 1024
    const int bitDepth = 10;
    const int bitLb = 8;
    const int bitRb = 8;

    const int bitCharacter = 2;

    //Devo aggiornarlo ogni volta che aggiungo un nodo
    int nodeCounter = 0; //Contatore del numero di nodi così so quanto spazio occupa il file e quanto spazio potrei salvare

    //Constuct 01: 8 bits initialized to zero
    bitset<8> bitset1(string("11111100"));

    cst_t cst;                              //declare the suffix tree
    construct(cst, inputFileName, 1);       //initialize the suffix tree

    typedef cst_bfs_iterator<cst_t> iterator;
    iterator begin = iterator(&cst, cst.root());
    iterator end = iterator(&cst, cst.root(), true, true);

    int str_length = 0;

    string nodeInfo = "";

    std::ofstream bin_out("./Output/test.bin", std::ios::out | std::ios::binary);
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

/*        std::cout << "\nNode info " << nodeInfo << std::endl;


        string edge = "";

        if ((cst.node_depth(*it) == 0) ||
            (cst.node_depth(*it) == 1 && str_length == 1 && (cst.lb(*it) == cst.rb(*it)))) {
            edge = "-";
        } else {

            if (cst.lb(*it) == cst.rb(*it)) {

                edge = "leaf:         ";

                for (int i = 1; i < str_length - 1; i++) {
                    edge += cst.edge(*it, i);
                }

                edge += "#";
                edge += "#";

            } else {

                edge = "internal_node: ";

                for (int i = 1; i <= str_length; i++) {
                    edge += cst.edge(*it, i);
                }
            }
        }

        std::cout << "NodeDepth: " << cst.node_depth(*it) << " Depth: " << cst.depth(*it) << "-[" << cst.lb(*it) << ","
                  << cst.rb(*it) << "]" << edge << std::endl;*/

        printBinFile(nodeInfo, bin_out);

    }

    bin_out.close(); // bf.bin is 8 bytes
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

}
