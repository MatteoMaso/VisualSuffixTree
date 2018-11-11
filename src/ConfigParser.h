//
// Created by root on 4/29/18.
//
#include <string>
#include "utils/Utils.hpp"
#include <map>

#ifndef VISUALSUFFIXTREE_CONDIFPARSER_H

#define VISUALSUFFIXTREE_CONDIFPARSER_H

class ConfigParser {
public:
    ConfigParser(std::string configFileName, std::map<std::string, std::string> * configParameter);
};

#endif //VISUALSUFFIXTREE_CONDIFPARSER_H
