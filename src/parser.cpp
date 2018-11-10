//
// Created by matteolus on 04/11/18.
//

#include <sdsl/suffix_trees.hpp>

#include <iostream>

#include "Utils.hpp"

#include <bitset> //to use quickly bit representation
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <pwd.h>

#include "TreeParser.h"
#include "SvgCreator.h"
#include "ConfigParser.h"




/**
 * This is the entry point for the first parser. The executable is called "Parser"
 */

bool const VERBOSE_DEFAULT = true; //todo put a false
std::string OUTPUT_DEFAULT_PATH = "/Output"; //todo check if it's ok



//todo sistemare generale

//char cCurrentPath[FILENAME_MAX];



using namespace sdsl;



//PARSER PARAMETER
//TODO chek if we need to make more visible
bool VERBOSE;
std::string input_file_path;
std::string output_folder;
std::string output_name;


/**
 * This command print the parameters formatting that the program expect
 */
void printHelp();



int main(int argc, char **argv) {

    char * homedir;
    homedir = (char*)malloc(100 * sizeof(char));
    getHomePath(&homedir);


	//PARSE THE ARGUMENTS
    InputParser input =  InputParser(argc, argv);
	if(input.cmdOptionExists("-h") || input.cmdOptionExists("-help")){
		printHelp();
        return 0;
	}

	//VERBOSE
	input.cmdOptionExists("-v") ? VERBOSE = true : VERBOSE = VERBOSE_DEFAULT;

	//INPUT FILENAME
	const std::string &filename = input.getCmdOption("-in");
	if (!filename.empty()) {
		//The input file name
        if(fexists(filename)){
			input_file_path = filename;
		} else {
            std::cout << "ERROR: File input specified doesn't exist.\n" << filename << "\n\n" << std::endl;
            return -1;
		}
	} else {
		//We need at least the input file name
		std::cout << "ERROR: please insert the input file name. -in filename_path or run -h command\n" << std::endl;
	}

	//OUTPUT FOLDER
	const std::string &outputFolder = input.getCmdOption("-oF");
	if(!outputFolder.empty()) {
		if(pexists(outputFolder)){
            output_folder = outputFolder;
		}else{
		    std::cout << "ERROR: output path doesn't exist or the programm has not access. \t\ngiven path: " << outputFolder << "\n" << std::endl;
		    return -1;
		}
	} else {
		//Set the default output folder and print to the user
        output_folder = homedir + OUTPUT_DEFAULT_PATH;
        if(!pexists(output_folder)){
            //Create the output folder
            p_create(output_folder);
        }

        if(!pexists(output_folder)){
            std::cout << "ERROR: default output path doesn't exist or the programm has not access. \t\ngiven path: " << output_folder << "\n" << std::endl;
            return -1;
        }
	}

	//OUTPUT FILENAME
	const std::string &outputFileName = input.getCmdOption("-o");
	if(!filename.empty()){
        //check if it already exist print it's already exist
        if(fexists(outputFolder + outputFileName)){
            std::cout << "\nERROR: the file "<< outputFileName <<" already exist.\n" << std::endl;
            //todo (optional) add the option to override the file.
            return 0;
        }else{
            output_name = outputFileName;
        }

	}else{
		//Set the default output filename and print it and the output folder
		//todo to complete
        std::cout << "Choose a name and use it when the user don't pass the name TODO" << std::endl;
        return -1;
	}

	//PRINT THE PARAMETER USED BEFORE STARTING
    //TODO PRINT PARAMETER to complete
    std::cout << "\nPARSER START with parameters:\n"
                 "\tInput  File   Name: " << input_file_path << "\n"
                 "\tOutput File   Name: " << output_name  << "\n"
                 "\tOutput Folder Name: " << output_folder    << "\n"
                 "\nVerbose active:     " << VERBOSE << "\n"  << std::endl;

	//todo add a logger


	//HUMAN CONFIGURATIONS
	map<string, string> configParameter;
	ConfigParser cfPars("../Settings/config.cfg", &configParameter); //Initialize the configurations Parameter



	//vector<string> tmp = split(argv[1], '\\' , '/');
	//std::string s = tmp.at(tmp.size() - 1).substr(0,tmp.at(tmp.size() - 1).size() - 4);  ;

	//s += ".bin";

	//string outputFileName = cCurrentPath;
	//outputFileName += "/Output/Binary_File/";
	//outputFileName += s;

	//char * tab2 = new char [outputFileName.length()+1];
	//strcpy (tab2, outputFileName.c_str());

	//TreeParser(argv[1], tab2, &configParameter);

    //Free allocateed memory
    free(homedir); //fix the problem with this pointr
}

void printHelp(){

	std::cout << "\nVisual Suffix Tree\n"
				 "\nNAME"
				 "\n\tparser\n"
				 "\nSYNOPSIS"
				 "\n\tparser [OPTION]...\n"
				 "\nDESCRIPTION"
				 "\n\tDescription about the parser"
				 "\n"
				 "\nMandatory arguments."
				 "\n"
				 "\n\t-in"
				 "\n\t\tInput file name, insert the entire filename path"
				 "\n"
				 "\nOptional arguments."
				 "\n"
				 "\n\t-o"
				 "\n\t\toutput file name\n"
				 "\n\t-oF"
				 "\n\t\toutput file path, default: " << OUTPUT_DEFAULT_PATH <<
				 "\n"
				 "\n\t-h, -help"
				 "\n\t\tto print this message\n"
				 "\n\t-v"
				 "\n\t\tto active the verbose mode\n\n" << std::endl;

	return;
}