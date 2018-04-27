//
// Created by root on 4/27/18.
//
#include <string>

#ifndef VISUALSUFFIXTREE_CONFIGPARSER_H
#define VISUALSUFFIXTREE_CONFIGPARSER_H

using namespace std;

class ConfigParser {

public:
    //Gli passo il path in cui trovare il file di configurazione
    ConfigParser(string configFileName, map<string, string> *configParameter);

private:

};


#endif //VISUALSUFFIXTREE_CONFIGPARSER_H
