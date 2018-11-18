//
// Created by matteolus on 04/11/18.
//
#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <bitset> //to use quickly bit representation
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <pwd.h>

#include "TreeParser.h"
#include "SvgCreator.h"
#include "ConfigParser.h"
#include "utils/Utils.hpp"
#include "logger/Logger.h"


//for the levelDB attempt
#include <cassert>
#include "../leveldb/include/leveldb/db.h"

/**
 * This is the entry point for the first parser. The executable is called "Parser"
 */

std::string OUTPUT_DEFAULT_PATH = "/Output";

/**
 * This command print the parameters formatting that the program expect
 */
void printHelp();

void levelDB_test();

int main(int argc, char **argv) {
    levelDB_test();

    char * homedir = (char*)malloc(100 * sizeof(char));
    char * input_file_path = (char*)malloc(256 * sizeof(char));
    char * output_folder = (char*)malloc(256 * sizeof(char));
    char * output_filename_path = (char*)malloc(256 * sizeof(char));
    getHomePath(homedir);

	//PARSE THE ARGUMENTS
    InputParser input =  InputParser(argc, argv);
	if(input.cmdOptionExists("-h") || input.cmdOptionExists("-help")){
		printHelp();
        return 0;
	}

	//VERBOSE
    LOGGER->SetVerbose(input.cmdOptionExists("-v"));

	//INPUT FILENAME
	const std::string &filename = input.getCmdOption("-in");
	if (!filename.empty()) {
		//The input file name
        LOGGER->Log("-in parameter found");
        if(fexists(filename)){
			input_file_path = (char *)filename.c_str();
            LOGGER->Log("-in value: %s", input_file_path);
		} else {
            std::cerr  << "ERROR: File input specified doesn't exist.\n" << filename << "\n\n" << std::endl;
            LOGGER->Log("-in file: %s, doesn't exist", input_file_path);
            return -1;
		}
	} else {
		//We need at least the input file name
        LOGGER->Log("-in parameter not found");
		std::cerr << "ERROR: please insert the input file name. -in filename_path or run -h command\n" << std::endl;
	}

	//OUTPUT FOLDER
	const std::string &outputFolder = input.getCmdOption("-oF");
	if(!outputFolder.empty()) {
        LOGGER->Log("-oF parameter found");
	    if(pexists(outputFolder)){
	        output_folder = (char *)outputFolder.c_str();
            LOGGER->Log("-oF value: %s", output_folder);
		}else{
            LOGGER->Log("-oF %s, doesn't exist", outputFolder);
		    std::cerr << "ERROR: output path doesn't exist or the programm has not access. \t\ngiven path: " << outputFolder << "\n" << std::endl;
		    return -1;
		}
	} else {
        LOGGER->Log("-oF parameter not found");
		//Set the default output folder and print to the user
        getHomePath(homedir);
        LOGGER->Log("Home dir: %s", homedir);
        strcpy(output_folder, homedir); //puth the home dir into output_folder
        strcat(output_folder, OUTPUT_DEFAULT_PATH.c_str()); //Append the other string

        if(!pexists(output_folder)){
            //Create the output folder
            p_create(output_folder);
            LOGGER->Log("-oF Create directory: %s", output_folder);
        }else{
            LOGGER->Log("-oF default output directory already exist, %s", output_folder);
            //std::cout << "Path already exist: " << output_folder << std::endl;
        }

        if(!pexists(output_folder)){
            LOGGER->Log("ERROR: default output path doesn't exist or the programm has not access. \t\ngiven path: %s", output_folder);
            std::cerr << "ERROR: default output path doesn't exist or the programm has not access. \t\ngiven path: " <<  output_folder << std::endl;
            return -1;
        }
	}

	//OUTPUT FILENAME
	const std::string &outputFileName = input.getCmdOption("-o");
	if(!filename.empty()){
        LOGGER->Log("-o parameter found");
	    //check if it already exist print it's already exist
        strcpy(output_filename_path, output_folder);
        strcat(output_filename_path, "/");
        strcat(output_filename_path, outputFileName.c_str());

        if(fexists(output_filename_path)){
            LOGGER->Log("ERROR: the file %s already exist!", output_filename_path);
            std::cerr <<  "ERROR: the file "<< output_filename_path << " already exist!" << std::endl;
            //todo (optional) add the option to override the file.
            //return 0; //todo remove the commentand let return 0, it was just for test
        }else{
            strcpy(output_filename_path, output_folder);
            strcat(output_filename_path, "/");
            strcat(output_filename_path, outputFileName.c_str());

            LOGGER->Log("-o output filename_path: %s", output_filename_path);
        }

	}else{
        LOGGER->Log("-o parameter not found, plese provide a valid name.");
		//Set the default output filename and print it and the output folder
		//todo to complete
        //std::cout << "Choose a name and use it when the user don't pass the name TODO" << std::endl;
        return -1;
	}

	//PRINT THE PARAMETER USED BEFORE STARTING
    //TODO PRINT PARAMETER to complete
    std::cout << "\nPARSER START with parameters:\n"
                 "\tInput  File   Name: " << input_file_path << "\n"
                 "\tOutput File   Name: " << output_filename_path  << "\n"
                 "\tOutput Folder Name: " << output_folder    << "\n" << std::endl;


	//HUMAN CONFIGURATIONS
	map<string, string> configParameter; //fix this things
    const std::string &config_path = input.getCmdOption("-c");

	//ConfigParser cfPars("/root/Desktop/Progetti/visualSuffixTree/VisualSuffixTree/Settings/config.cfg", &configParameter); //Initialize the configurations Parameter
	ConfigParser cfPars(config_path, &configParameter); //Initialize the configurations Parameter

    TreeParser(input_file_path, output_filename_path, &configParameter);

    //Free allocated memory
    free(homedir); //fix the problem with this pointr
    free(input_file_path);
    free(output_folder);
    free(output_filename_path);
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
				 "\n\t\tto active the verbose mode.\n\n" << std::endl;
	return;
}

void levelDB_test() {

    leveldb::DB* db;
    leveldb::Options options;

    options.create_if_missing = true;
    leveldb::Status status = leveldb::DB::Open(options, "/root/lDB/", &db);
    assert(status.ok());

    leveldb::Status s;
    std::string value = "100ghghghg";
    std::string key = "ciao";
    //leveldb::Status s = db->Get(leveldb::ReadOptions(), key1, &value);
    if (s.ok()) s = db->Put(leveldb::WriteOptions(), key, value);
    std::string document;
    db->Get(leveldb::ReadOptions(), "ciao", &document);
    std::cout << document << std::endl;
    delete db;
}
