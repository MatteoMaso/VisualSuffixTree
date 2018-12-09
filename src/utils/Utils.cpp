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



//Method for getAlphabet
bool contains2(std::vector<char> *character, char c) {
    for (int i = 0; i < character->size(); i++) {
        if (character->at(i) == c) {
            return true;
        }
    }
    return false;
}

/*
 * Extracts a vector with the different character  find in the file
 */
bool getAlphabet(char *inputFileName, std::vector<char> * alphabet) {

    string txt;
    ifstream file(inputFileName);

    if (file.is_open())
        while (file.good())
            getline(file, txt);
    file.close();

    char c;
    for (int i = 0; i < txt.length(); i++) {
        c = txt[i];
        if (!contains2(alphabet, c)) {
            alphabet->push_back(c);
        }
    }

    return true; //todo (add some control)
    //check if the file exist and something like this
    //check if there's no letter inside or something like this
}
