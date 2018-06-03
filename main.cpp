#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset> //to use quickly bit representation
#include <vector>
#include <fstream>
#include <sys/stat.h>

#include "Include/TreeParser.h"
#include "Include/SvgCreator.h"
#include "Include/ConfigParser.h"


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

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        std::cout << "Error in reading current dir!... report on gitHub thanks" << std::endl;
        return errno;
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

    std::cout <<  "The current working directory is " << cCurrentPath << "\n " << std::endl;

    if (argc < 2) {
        cout << "insert -data nameInput for starting treeParser or -svg to create the SVG file" << std::endl;
        return 1;
    }

    //HUMAN CONFIGURATIONS
    map<string, string> configParameter;
    ConfigParser cfPars("./Settings/config.cfg", &configParameter); //Initialize the configurations Parameter

    string prog = argv[1];

    if (prog == "-data") {

        if ( argc != 3 ){
            std::cout << "Bad arguments in -data program, so please show readMe file!" << std::endl;
            exit(-1);

        } else {

            vector<string> tmp = split(argv[2], '\\' , '/');
            std::string s = tmp.at(tmp.size() - 1).substr(0,tmp.at(tmp.size() - 1).size() - 4);  ;

            s += ".bin";

//            std::cout << s << std::endl;

            string outputFileName = cCurrentPath;
            outputFileName += "/Output/Binary_File/";
            outputFileName += s;

            char * tab2 = new char [outputFileName.length()+1];
            strcpy (tab2, outputFileName.c_str());

//            std::cout << tab2 << std::endl;

            TreeParser(argv[2], tab2, &configParameter);
        }


    } else if (prog == "-svg") {

        if ( argc != 3 ){
            std::cout << "Bad arguments in -svg program, so please show readMe file!" << std::endl;
            exit(-1);

        } else {

            vector<string> tmp = split(argv[2], '\\' , '/');
            std::string s = tmp.at(tmp.size() - 1).substr(0,tmp.at(tmp.size() - 1).size() - 4);  ;

            string binFile = cCurrentPath;
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

    } else {
        std::cout << "Bad arguments! Chose -data or -svg" << std::endl;
        exit(-1);
    }
}


