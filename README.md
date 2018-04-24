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
Given a file containing a string as a sequence of characters on a single line without headers, you first need to build an index that contains all the necessary information for drawing the suffix tree later. This is done by issuing the command:
```
./VisualSuffixTree -data [string.txt] [index.bin]
```
Given the index, you can create an SVG drawing of the suffix tree as follows:
```
./VisualSuffixTree -svg [index.bin] [drawing.html]
```
