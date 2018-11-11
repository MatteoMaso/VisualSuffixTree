//
// Created by root on 5/8/18.
//
#include <string>
#include <iostream>
#include <vector>

#ifndef VISUALSUFFIXTREE_UTILS_H
#define VISUALSUFFIXTREE_UTILS_H

//using namespace std;

class InputParser{
public:
    InputParser (int &argc, char **argv);

    /// @author iain
    const std::string& getCmdOption(const std::string &option) const;

    /// @author iain
    bool cmdOptionExists(const std::string &option) const;

private:
    std::vector <std::string> tokens;
};

bool fexists(const std::string& filename);

bool pexists(const std::string& pathname);

void getHomePath(char * homedir);

std::string toBinFormat(int numberOfBit, unsigned long n);

#include <sys/stat.h>

bool p_create(std::string path_name);

std::string CurrentTime();


#endif //VISUALSUFFIXTREE_UTILS_H
