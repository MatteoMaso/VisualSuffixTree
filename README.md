Representing suffix trees as flame graphs
=========

Installation
------------
First, install the [SDSL library](https://github.com/simongog/sdsl.git) into ./usr/local
command: ./install.sh ./usr/local
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
./VisualSuffixTree -data [input_string.txt]
example:  ./VisualSuffixTree -data ./Example/input_example.txt
```
Given the index, you can create an SVG drawing of the suffix tree as follows:
```
./VisualSuffixTree -svg [input_string.txt]
example: ./VisualSuffixTree -svg ./Example/input_example.txt
```
