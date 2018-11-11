# VisualSuffixTree

The purpose of this code is to provide a simple tool to represent a long sequence of character into a
suffix tree structure and then to produce a flame graph in which different properties can be evidenced
as the user prefer.

[VisualSuffixTree documentation](https://github.com/MatteoMaso/VisualSuffixTree/tree/develop/doc)

## Usage

### Prerequisites

##### You will need:

 * A modern C/C++ compiler
 * CMake 3.1+ installed

##### First, install the [SDSL library](https://github.com/simongog/sdsl.git) into default directory:

```
git clone https://github.com/simongog/sdsl-lite.git
cd sdsl-lite
./install.sh
```

### Building the Project

#### Git Clone

First we need to check out the git repo:

```
cd ${insert your workspace folder here}
git clone https://github.com/MatteoMaso/VisualSuffixTree.git
cd VisualSuffixTree
```

Now we shoud be in the project's top folder.
Build the project:
```
cmake CMakeLists.txt
make
```

## Running

In order to configure the output you have to set up the `config.txt` file. 

Given a file containing a string as a sequence of characters on a single line without headers, you first need to build an index that contains all the necessary information for drawing the suffix tree later. This is done by issuing the command:
```
./Plotter -in input_fileName [-v][-o nome_outputFile][-oF output_path][-h|-help]
example:  ./Plotter -in input_fileName
```
Given the index, you can create an SVG drawing of the suffix tree as follows:
```
./SVG_plotter -svg -in input_fileName [-v][-o nome_outputFile][-oF output_path][-h|-help]
example: ./SVG_plotter  -in input_fileName
```

## External library

[SDSL library](https://github.com/simongog/sdsl.git)

The Succinct Data Structure Library (SDSL) is a powerful and flexible C++11 library implementing succinct data structures. In total, the library contains the highlights of 40 research publications. Succinct data structures can represent an object (such as a bitvector or a tree) in space close to the information-theoretic lower bound of the object while supporting operations of the original object efficiently. The theoretical time complexity of an operation performed on the classical data structure and the equivalent succinct data structure are (most of the time) identical.

### License

Open sourced under MIT license, the terms of which can be read here — [MIT License](http://opensource.org/licenses/MIT).
