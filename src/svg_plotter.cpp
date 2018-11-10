//
// Created by matteolus on 04/11/18.
//

#include <sdsl/suffix_trees.hpp>

#include <iostream>
#include <string>

#include "Utils.hpp"

#include <bitset> //to use quickly bit representation
#include <vector>
#include <fstream>
#include <sys/stat.h>

#include "TreeParser.h"
#include "SvgCreator.h"
#include "ConfigParser.h"


#ifdef WINDOWS
#include <direct.h>
    #define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

char cCurrentPath[FILENAME_MAX];

using namespace std;
using namespace sdsl;


typedef cst_sct3<> cst_t;

vector<string> split(const char *str, char c1 = ' ', char c2 = 'w')
{
    vector<string> result;

    do
    {
        const char *begin = str;

        while((( *str != c1 ) && (*str != c2))  && *str)
            str++;

        result.push_back(string(begin, str));
    } while (0 != *str++);

    return result;
}


bool exist(const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

int main(int argc, char *argv[]) {

    //todo add check of the path and arg passing

    //todo add a logger

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        std::cout << "Error in reading current dir!... report on gitHub thanks" << std::endl;
        return errno;
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

    //std::cout <<  "The current working directory is " << cCurrentPath << "\n " << std::endl;

    //HUMAN CONFIGURATIONS
    map<string, string> configParameter;
    ConfigParser cfPars("../Settings/config.cfg", &configParameter); //Initialize the configurations Parameter

    string prog = argv[1];

    if ( argc != 2 ){
        std::cout << "Bad arguments, so please look at readMe file!" << std::endl;
        exit(-1);

    } else {

        vector<string> tmp = split(argv[1], '\\' , '/');
        std::string s = tmp.at(tmp.size() - 1).substr(0,tmp.at(tmp.size() - 1).size() - 4);  ;

        string binFile = cCurrentPath;
        //todo fix the relative path
        binFile.append("/Output/Binary_File/" + s + ".bin");
        char * binFilePointer = new char [binFile.length()+1];
        strcpy (binFilePointer, binFile.c_str());

        if (!exist(binFilePointer)){
            std::cout << "File: " << binFile << " not fount in Output/Binary_File, so create file first with -data program " << std::endl;
            exit(-1);
        }

        int version = 1;

        string svgFile = cCurrentPath;
        svgFile.append("/Output/Svg_Output/svg_" + s + "_" + to_string(version) + ".html");

        char * svgFilePointer = new char [svgFile.length()+1];
        strcpy (svgFilePointer, svgFile.c_str());

        bool nameFree = true;

        do{

            svgFile = cCurrentPath;
            svgFile.append("/Output/Svg_Output/svg_" + s + "_" + to_string(version) + ".html");

            svgFilePointer = new char [svgFile.length()+1];
            strcpy (svgFilePointer, svgFile.c_str());

            if(exist(svgFilePointer)){
                version++;
                nameFree = false;
            } else {
                nameFree = true;
            }


        }while(!nameFree);

        SvgCreator svgCreator(binFilePointer, svgFilePointer, &configParameter, argv[2]);
    }

}