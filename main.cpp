#include <sdsl/suffix_trees.hpp>
#include <iostream>
#include <string>
#include <bitset> //to use quickly bit representation
#include <vector>
#include <fstream>

#include "Include/TreeParser.h"
#include "Include/SvgCreator.h"

using namespace std;
using namespace sdsl;


typedef cst_sct3<> cst_t;


static const int parameters[10] = {10, 10, 16, 16};

int main(int argc, char *argv[]) {




    if (argc < 2) {
        cout << "insert -data nameInput for starting treeParser or -svg to create the SVG file" << std::endl;
        return 1;
    }

    char firstProgram[6] = "-data";
    char secondProgram[5] = "-svg";

    if (strcmp(argv[1], firstProgram) == 0) {

        TreeParser(argv[2], argv[3]);
    } else if (strcmp(argv[1], secondProgram) == 0) {
        SvgCreator svgCreator(argv[2]);

    } else {
        printf("Bad arguments!");
    }
}

