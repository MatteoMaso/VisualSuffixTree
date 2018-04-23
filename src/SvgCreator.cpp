#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset>       //to use quickly bit representation
#include <vector>
#include <fstream>

#include "../Include/SvgCreator.h"

using namespace std;

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

string partitioner(string s, int from, int to);

string getHeader(string fileName);

SvgCreator::SvgCreator(char *inputFileName) {

        //BEGIN SVG CREATOR
        std::ofstream svg_out("./Output/svg.html", std::ios::out | std::ios::binary);

        const int bitNodeDepth = 10; //fino a 1024
        const int bitDepth = 10;
        const int bitLb = 8;
        const int bitRb = 8;
        const int bitCharRepresentation = 2;


        std::ifstream bin_in(inputFileName , std::ios::binary);
        if (!bin_in.is_open()){
            std::cout << "I'm not able to open file: " << inputFileName << std::endl;
        }

        BitIo<16> bio2;
        bin_in >> bio2;

        int nodeInfoDim = 36;
        int nodeInfoLength = 0;
        if ((nodeInfoDim % 16)  == 0){
            nodeInfoLength = nodeInfoDim / 16;
        } else {
            nodeInfoLength = nodeInfoDim / 16 + 1;
        }


        svg_out << getHeader("headerSvg.txt"); //Insert the header SVG into the file

        int a, b, c;
        int x, y, i, j, z, x0, y0, H, w;

        x0 = 10;
        y0 = 40;
        H = 15;

        //TODO ADD warning se non trova il file test.bin oppure se è sbagliato o vuoto

        while (!bio2.empty()) {

            string nodeInfo = "";

            for (int i = 0; i < nodeInfoLength; ++i) {
                nodeInfo += bio2.pop_front().to_string();
            }

            a = stoi(partitioner(nodeInfo, 0, 9), nullptr, 2);
            b = stoi(partitioner(nodeInfo, 20, 27), nullptr, 2);
            c = stoi(partitioner(nodeInfo, 28, 35), nullptr, 2);


            x = x0 + b;
            H = 15;
            y = y0 + (a*H);
            w = (c-b) +1 ;

            std::cout << "\nBitNode depth: " << a << " [" << b << "-" << c << "]\n" << std::endl;



//        std::cout << "\n<g class=\"func_g\" onmouseover=\"s(this)\" onmouseout=\"c()\" onclick=\"zoom(this)\">\n"
//                "<title>DEPTH IS: " << z << "</title><rect x=\""<< x <<"\" y=\"" << y <<"\" width=\"980\" height=\"15.0\" fill=\"rgb(225,0,0)\" rx=\"2\" ry=\"2\" />\n""</g>" << std::endl;

            string temp =  "\n<g class=\"func_g\" onmouseover=\"s(this)\" onmouseout=\"c()\" onclick=\"zoom(this)\">\n""<title>DEPTH IS: ";
            temp+= to_string(z);
            temp+= "</title><rect x=\"";
            temp+= to_string(x);
            temp+= "\" y=\"";
            temp+= to_string(y);
            temp+= "\" width=\"";
            temp+= to_string(w);
            temp+= "\" ""height=\"15.0\" fill=\"rgb(225,0,0)\" rx=\"2\" ry=\"2\" />\n""</g>";

            char str[temp.length()];
            strcpy(str, temp.c_str());

            svg_out << str;
        }


        char svgEnd[] = {"</svg>"}; //Close the SVG File
        svg_out << svgEnd;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    string c2 = "ciao";
//    char str[c2.length()];
//    strcpy(str, c2.c_str());

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        bin_in.close(); //Close the input file
        svg_out.close(); //chiudo il file on output
};

string partitioner(string s, int from, int to){

    string a = "";

    for (int i = from; i <= to; ++i) {
        a += s[i];
    }

    return a;
}

string getHeader(string fileName){

    string txt;
    string temp;
    ifstream file;
    file.open(fileName);

    if (file.is_open()){
//        std::cout << "File open" << std::endl;

        while (!file.eof()) {
            getline(file, temp);
            txt += temp + "\n";
        }

    } else {
        std::cout << "I'm not able to read the header!" << std::endl;
    }

    file.close();
    return txt;
}
