Representing suffix trees as flame graphs
=========

Installation
------------
First, install the [SDSL library](https://github.com/simongog/sdsl.git) into a known path. Then, set the corresponding SDSL installation directories in the `CMakeList.txt` file of the project:
```
include_directories(<SDSL_include_dir>)
link_directories(<SDSL_lib_dir>)
```
Finally, build the project from its root path:
```
cmake CMakeLists.txt
make
```

Usage
-----
In order to configure properly the output you have to setting up the config.txt file

Given a file containing a string as a sequence of characters on a single line without headers, you first need to build an index that contains all the necessary information for drawing the suffix tree later. This is done by issuing the command:
```
./VisualSuffixTree -data [input_string.txt] [index.bin]
example:  -data ./Example/test/blue-whale-mt.txt ./Output/blue-whale-mt.bin
```
Given the index, you can create an SVG drawing of the suffix tree as follows:
```
./VisualSuffixTree -svg [index.bin] [drawing.html] [input_string.txt]
example: -svg ./Output/blue-whale-mt.bin ./Output/svg_blue-whale-mt.html ./Example/test/blue-whale-mt.txt
```
