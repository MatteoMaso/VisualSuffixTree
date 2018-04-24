# VisualSuffixTree
A tool for representing suffix trees as flame graphs.

Installation
------------
First, install the [SDSL library](https://github.com/simongog/sdsl.git) into a known path. Then, set the corresponding SDSL installation directories in the `CMakeList.txt` file of the project:

```
include_directories(<SDSL_include_dir>)
link_directories(<SDSL_lib_dir>)
```

Finally, build the project from its root path:

```
cmake CMakeList.txt
make
```

Usage
-----
The main program contain two program

Program 1: TREE PARSER
----------------------
This program take a txt file as input and create a binary file with all the information reguarding the node
of the suffix tree built from the string.

    @user: VisualSuffixTree -data [input_string.txt] [output_file.bin]

Program 2: SVG CREATOR
----------------------
This program take as input a binary file created with the program 1 and create as output
an html file that represent a flameGraph of our string

    @user: VisualSuffixTree -svg [input_file.bin] [output_file.html]
