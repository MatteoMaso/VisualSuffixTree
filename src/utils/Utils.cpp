//
// Created by root on 11/10/18.
//
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <bitset>
#include "Utils.hpp"
#include "../logger/Logger.h"


InputParser::InputParser (int &argc, char **argv){
    for (int i=1; i < argc; ++i)
        this->tokens.push_back(std::string(argv[i]));
}

/// @author iain
const std::string& InputParser::getCmdOption(const std::string &option) const{
    std::vector<std::string>::const_iterator itr;
    itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != this->tokens.end() && ++itr != this->tokens.end()){
        return *itr;
    }
    static const std::string empty_string("");
    return empty_string;
}
    /// @author iain
bool InputParser::cmdOptionExists(const std::string &option) const{
    return std::find(this->tokens.begin(), this->tokens.end(), option)
        != this->tokens.end();
}


bool fexists(const std::string& filename){
    std::ifstream ifile(filename.c_str());
    return (bool)ifile;
}


bool pexists(const std::string& pathname){
    struct stat info;

    char * tab2 = new char [pathname.length()+1];
    strcpy (tab2, pathname.c_str());

    if( stat( tab2, &info ) != 0 )
        //printf( "cannot access %s\n", pathname );
        return false;
    else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows
        //printf( "%s is a directory\n", pathname );
        return true;
    else
        //printf( "%s is no directory\n", pathname );
        return false;
}

void getHomePath(char * homedir){

    struct passwd *pw = getpwuid(getuid());
    strcpy(homedir, (pw->pw_dir));
}

bool p_create(std::string path_name){
    //std::cout << path_name << " creation of this path" << std::endl;
    mkdir(path_name.c_str(), 777);
    return true;
}

std::string CurrentTime() {
    time_t rawtime;
    struct tm * ptm;

    //time ( &rawtime );
    //ptm = gmtime ( &rawtime );

    return "Time"; //todo from clion doesn't work with time ... from terminal it works
}

//old methods...
std::string toBinFormat(int numberOfBit, unsigned long n) {

    //Todo cercare un metodo più, usando gli shift probabilmente

    std::string tmp;

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


bool contains2(std::vector<char> *character, char c) {
    for (int i = 0; i < character->size(); i++) {
        if (character->at(i) == c) {
            return true;
        }
    }
    return false;
}

bool getAlphabet(char *inputFileName, std::vector<char> * alphabet) {

    string txt;
    ifstream file(inputFileName);

    if (file.is_open())
        while (file.good())
            getline(file, txt);
    file.close();

    //vector<char> character = {};
    char c;
    for (int i = 0; i < txt.length(); i++) {
        c = txt[i];
        if (!contains2(alphabet, c)) {
            alphabet->push_back(c);
        }
    }


    /*
    for (int j = 0; j < alphabet->size(); j++) {
        cst_t::char_type c = alphabet->at(j);
        treeParser->alphabet.push_back(c);
    }
    */
    return true; //todo (add some control)
    //check if the file exist and something like this
    //check if there's no letter inside or something like this
}
