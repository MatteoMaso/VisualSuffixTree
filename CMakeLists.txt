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

#used for header library file
include_directories($ENV{HOME}/include)
link_directories($ENV{HOME}/lib)

set(SOURCE_FILE main.cpp)

add_executable(VisualSuffixTree main.cpp src/SvgCreator.cpp Include/SvgCreator.h src/TreeParser.cpp Include/TreeParser.h  src/ConfigParser.cpp Include/ConfigParser.h src/NodeInfoStructure.cpp Include/NodeInfoStructure.h src/NodeInfo.cpp Include/NodeInfo.h src/Header.cpp Include/Header.h Include/Header.h Include/TreeParser.h Include/SvgCreator.h Include/NodeInfo.h Include/NodeInfoStructure.h Include/ConfigParser.h Include/BitIo.h Include/ObjNode.h src/ObjNode.cpp Include/Utils.h Include/SvgUtils.h src/SvgObject/StatusBar.cpp Include/SvgObject/StatusBar.h)

target_link_libraries(VisualSuffixTree sdsl divsufsort divsufsort64)
