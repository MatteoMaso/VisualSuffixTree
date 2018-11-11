//
// Created by root on 4/27/18.
//

#include <map>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "ConfigParser.h"

ConfigParser::ConfigParser(string configFileName, map<string, string> *configParameter){

    std::cout << configFileName << std::endl;
    std::ifstream cFile (configFileName);

    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            line.erase(remove_if(line.begin(), line.end(), ::isspace),
                       line.end());
            if(line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            string name = line.substr(0, delimiterPos);
            string value = line.substr(delimiterPos + 1);
            pair<string, string> element = {name, value};
            configParameter->insert(element);
//            std::cout << name << " " << value << '\n';
        }
        cFile.close();
    } else {
        std::cout << "Unable to open config file." << '\n';
        exit(-1);
    }
}