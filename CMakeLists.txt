## The minimum version of CMake required ##
cmake_minimum_required(VERSION 3.10)
SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

## Project information ##
project(VisualSuffixTree)
set(PROJECT_VENDOR "Matteo Maso")
set(PROJECT_CONTACT "matteo.maso.93@gmail.com")
set(PROJECT_URL "https://github.com/MatteoMaso/VisualSuffixTree")
set(PROJECT_DESCRIPTION "VisualSuffixTree: Visual tool for Suffix Tree visualization")

set(CMAKE_CXX_STANDARD 11)

#Used for the sdsl library
include_directories($ENV{HOME}/include)
link_directories($ENV{HOME}/lib)


#add_executable(VisualSuffixTree src/main.cpp src/SvgCreator.cpp src/SvgCreator.h src/TreeParser.cpp src/TreeParser.h src/ConfigParser.cpp src/ConfigParser.h src/NodeInfoStructure.cpp src/NodeInfoStructure.h src/NodeInfo.cpp src/NodeInfo.h src/Header.cpp src/Header.h src/Header.h src/TreeParser.h src/SvgCreator.h src/NodeInfo.h src/NodeInfoStructure.h src/ConfigParser.h src/BitIo.h src/ObjNode.h src/ObjNode.cpp src/Utils.h src/SvgUtils.h src/SvgObject/StatusBar.cpp src/SvgObject/StatusBar.h)
add_executable(Parser src/tree_parser.cpp src/SvgCreator.cpp src/SvgCreator.h src/TreeParser.cpp src/TreeParser.h src/ConfigParser.cpp src/ConfigParser.h src/NodeInfoStructure.cpp src/NodeInfoStructure.h src/NodeInfo.cpp src/NodeInfo.h src/Header.cpp src/Header.h src/Header.h src/TreeParser.h src/SvgCreator.h src/NodeInfo.h src/NodeInfoStructure.h src/ConfigParser.h src/BitIo.h src/ObjNode.h src/ObjNode.cpp src/utils/Utils.hpp src/SvgUtils.h src/SvgObject/StatusBar.cpp src/SvgObject/StatusBar.h src/utils/Utils.cpp src/logger/Logger.cpp src/logger/Logger.h src/nodeMap/NodesMap.cpp src/nodeMap/NodesMap.h src/node/Node.cpp src/node/Node.h)
add_executable(SVG_plotter src/svg_plotter src/SvgCreator.cpp src/SvgCreator.h src/TreeParser.cpp src/TreeParser.h src/ConfigParser.cpp src/ConfigParser.h src/NodeInfoStructure.cpp src/NodeInfoStructure.h src/NodeInfo.cpp src/NodeInfo.h src/Header.cpp src/Header.h src/Header.h src/TreeParser.h src/SvgCreator.h src/NodeInfo.h src/NodeInfoStructure.h src/ConfigParser.h src/BitIo.h src/ObjNode.h src/ObjNode.cpp src/utils/Utils.hpp src/SvgUtils.h src/SvgObject/StatusBar.cpp src/SvgObject/StatusBar.h src/utils/Utils.cpp src/logger/Logger.cpp src/logger/Logger.h src/node/Node.cpp src/node/Node.h)

#target_link_libraries(VisualSuffixTree sdsl divsufsort divsufsort64)
target_link_libraries(Parser sdsl divsufsort divsufsort64)
target_link_libraries(SVG_plotter sdsl divsufsort divsufsort64)
