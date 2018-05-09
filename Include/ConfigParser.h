//
// Created by root on 4/29/18.
//
#include <string>

#ifndef VISUALSUFFIXTREE_CONDIFPARSER_H

#define VISUALSUFFIXTREE_CONDIFPARSER_H

using namespace std;
class ConfigParser {
public:
    ConfigParser(string configFileName, map<string, string> * configParameter);
};

#endif //VISUALSUFFIXTREE_CONDIFPARSER_H
