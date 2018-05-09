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

        case 21:
            tmp = std::bitset<21>(n).to_string();
            break;

        case 22:
            tmp = std::bitset<22>(n).to_string();
            break;

        case 23:
            tmp = std::bitset<23>(n).to_string();
            break;

        case 24:
            tmp = std::bitset<24>(n).to_string();
            break;

        case 25:
            tmp = std::bitset<25>(n).to_string();
            break;

        case 26:
            tmp = std::bitset<26>(n).to_string();
            break;

        case 27:
            tmp = std::bitset<27>(n).to_string();
            break;

        case 28:
            tmp = std::bitset<28>(n).to_string();
            break;

        case 29:
            tmp = std::bitset<29>(n).to_string();
            break;

        case 30:
            tmp = std::bitset<30>(n).to_string();
            break;

        case 31:
            tmp = std::bitset<31>(n).to_string();
            break;

        case 32:
            tmp = std::bitset<32>(n).to_string();
            break;

        case 33:
            tmp = std::bitset<33>(n).to_string();
            break;

        case 34:
            tmp = std::bitset<34>(n).to_string();
            break;

        case 35:
            tmp = std::bitset<35>(n).to_string();
            break;

        case 36:
            tmp = std::bitset<36>(n).to_string();
            break;

        case 37:
            tmp = std::bitset<37>(n).to_string();
            break;

        case 38:
            tmp = std::bitset<38>(n).to_string();
            break;

        case 39:
            tmp = std::bitset<39>(n).to_string();
            break;

        case 40:
            tmp = std::bitset<40>(n).to_string();
            break;


        default:
        std::cout << "troppi bit nella rappresentazione! modificare toBinFormat in utils.h" << std::endl;
            break;
    }


    return tmp;
}


#endif //VISUALSUFFIXTREE_UTILS_H
