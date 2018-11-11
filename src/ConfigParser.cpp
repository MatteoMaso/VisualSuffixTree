//
// Created by root on 4/27/18.
//

#include <map>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "ConfigParser.h"

ConfigParser::ConfigParser(std::string configFileName, std::map<std::string, std::string> * configParameter){

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
            std::string name = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            std::pair<std::string, std::string> element = {name, value};
            configParameter->insert(element);
//            std::cout << name << " " << value << '\n';
        }
        cFile.close();
    } else {
        std::cout << "Unable to open config file." << '\n';
        exit(-1);
    }
}