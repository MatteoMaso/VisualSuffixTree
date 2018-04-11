#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sdsl;

typedef cst_sct3<> cst_t;

/**
 *
 * @param inputFileName file that contains the strings that you want to analyze
 */
void treeParser(char* inputFileName);

/**
 * This method take an input file well formatted and create a flameGraph as output
 * @param inputFileName formatted file to convert in FlameGraph SVG
 */
void createSvg(char* inputFileName);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "insert -data nameInput for starting treeParser or -svg to create the SVG file" << std::endl;
        return 1;
    }

    char firstProgram[6]  = "-data";
    char secondProgram[5] = "-svg";

    if (strcmp(argv[1], firstProgram) == 0){
        treeParser(argv[2]);
    } else if (strcmp(argv[1], secondProgram) == 0){
        createSvg(argv[2]);
    } else {
        printf("Bad arguments!");
    }
}


void createSvg(char* inputFileName){
    //BEGIN SVG CREATOR
//    todo to implement
};

/**
 * FORMAT OUTPUT FILE
 * ogni parametro è separato da ":"
 *
 * PARAM1: node_depth
 * PARAM2: depth
 * PARAM3:
 * PARAM4:
 */
void treeParser(char* inputFileName) {

    //BEGIN TREE PARSER

    cst_t cst;                              //declare the suffix tree
    construct(cst, inputFileName, 1);       //initialize the suffix tree

    typedef cst_bfs_iterator<cst_t> iterator;
    iterator begin = iterator(&cst, cst.root());
    iterator end   = iterator(&cst, cst.root(), true, true);

    int str_length = 0;

    for (iterator it = begin; it != end; ++it) {

        str_length = cst.depth(*it);

        string edge = "";

        if( (cst.node_depth(*it) == 0)||(cst.node_depth(*it) == 1 && str_length == 1 && (cst.lb(*it) == cst.rb(*it))) ){
            edge = "-";
        } else {

            if (cst.lb(*it) == cst.rb(*it)) {

                edge = "leaft:         ";

                for (int i = 1; i < str_length - 1; i++) {
                    edge += cst.edge(*it, i);
                }

                edge += "#";
                edge += "#";

            } else {

                edge = "internal_node: ";

                for (int i = 1; i <= str_length; i++) {
                    edge += cst.edge(*it, i);
                }
            }
        }

        std::cout << "NodeDepth: " << cst.node_depth(*it) << " Depth: " << cst.depth(*it) << "-[" << cst.lb(*it) << "," << cst.rb(*it) << "]" << edge << std::endl;
    }
};
