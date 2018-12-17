//
// Created by root on 5/8/18.
//
#ifndef VISUALSUFFIXTREE_UTILS_H
#define VISUALSUFFIXTREE_UTILS_H

#include <string>
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <cstring>

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

bool contains2(std::vector<char> *character, char c);

bool fexists(const std::string& filename);

bool pexists(const std::string& pathname);

void getHomePath(char * homedir);

std::string toBinFormat(int numberOfBit, unsigned long n);

bool p_create(std::string path_name);

std::string CurrentTime();

/**
 * This method is useful to extract the alphabet into a txt file
 * @param inputFileName complete name of a string txt file
 * @param alphabet it's a vector of char, each is a lecter contained in the file given
 * @return bool if ok false if some error happens
 */
bool getAlphabet(char * inputFileName, std::vector<char> * alphabet);


#endif //VISUALSUFFIXTREE_UTILS_H
