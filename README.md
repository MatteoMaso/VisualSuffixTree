# VisualSuffixTree

The purpose of this code is to provide a simple tool to represent a long sequence of character into a 
suffix tree structure and then to produce a flame graph in which different properties can be evidenced 
as the user prefer. 

[VisualSuffixTree documentation](https://github.com/MatteoMaso/VisualSuffixTree/tree/master/doc)

## Usage

### Prerequisites

You will need:

 * A modern C/C++ compiler
 * CMake 3.1+ installed

### Building the Project

#### Git Clone

First we need to check out the git repo:

```
$ cd ${insert your workspace folder here}
$ git clone https://github.com/MatteoMaso/VisualSuffixTree.git
$ cd VisualSuffixTree
```

Now we shoud be in the project's top folder.

## Installing
first, install the [SDSL library](https://github.com/simongog/sdsl.git) into default directory:
```
./install.sh
```
Finally, build the project from its root path:
```
cmake CMakeLists.txt
make
```

## Running

In order to configure the output you have to set up the `config.txt` file. 

Given a file containing a string as a sequence of characters on a single line without headers, you first need to build an index that contains all the necessary information for drawing the suffix tree later. This is done by issuing the command:
```
./VisualSuffixTree -data [input_string.txt]
example:  ./VisualSuffixTree -data ./Example/input_example.txt
```
Given the index, you can create an SVG drawing of the suffix tree as follows:
```
./VisualSuffixTree -svg [input_string.txt]
example: ./VisualSuffixTree -svg ./Example/input_example.txt
```

## External library
add the simong library with the link

add LevelDB library when i'll use it

### License

Open sourced under MIT license, the terms of which can be read here — [MIT License](http://opensource.org/licenses/MIT).
