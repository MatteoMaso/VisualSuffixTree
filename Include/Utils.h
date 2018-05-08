//
// Created by root on 5/8/18.
//
#include <string>
#include <iostream>


#ifndef VISUALSUFFIXTREE_UTILS_H
#define VISUALSUFFIXTREE_UTILS_H

using namespace std;

string toBinFormat(int numberOfBit, unsigned long n) {

    //Todo cercare un metodo più, usando gli shift probabilmente

    string tmp;

    switch (numberOfBit) {
        case 1:
            tmp = std::bitset<1>(n).to_string();
            break;

        case 2:
            tmp = std::bitset<2>(n).to_string();
            break;

        case 3:
            tmp = std::bitset<3>(n).to_string();
            break;

        case 4:
            tmp = std::bitset<4>(n).to_string();
            break;

        case 5:
            tmp = std::bitset<5>(n).to_string();
            break;

        case 6:
            tmp = std::bitset<6>(n).to_string();
            break;

        case 7:
            tmp = std::bitset<7>(n).to_string();
            break;

        case 8:
            tmp = std::bitset<8>(n).to_string();
            break;

        case 9:
            tmp = std::bitset<9>(n).to_string();
            break;

        case 10:
            tmp = std::bitset<10>(n).to_string();
            break;

        case 11:
            tmp = std::bitset<11>(n).to_string();
            break;

        case 12:
            tmp = std::bitset<12>(n).to_string();
            break;

        case 13:
            tmp = std::bitset<13>(n).to_string();
            break;

        case 14:
            tmp = std::bitset<14>(n).to_string();
            break;

        case 15:
            tmp = std::bitset<15>(n).to_string();
            break;

        case 16:
            tmp = std::bitset<16>(n).to_string();
            break;

        case 17:
            tmp = std::bitset<17>(n).to_string();
            break;

        case 18:
            tmp = std::bitset<18>(n).to_string();
            break;

        case 19:
            tmp = std::bitset<19>(n).to_string();
            break;

        case 20:
            tmp = std::bitset<20>(n).to_string();
            break;


        default:
        std::cout << "troppi bit nella rappresentazione! modificare toBinFormat in utils.h" << std::endl;
            break;
    }


    return tmp;
}


#endif //VISUALSUFFIXTREE_UTILS_H
