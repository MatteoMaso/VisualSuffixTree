#the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

project(VisualSuffixTree)

set(CMAKE_CXX_STANDARD 11)

#used for header library file
include_directories(/opt/sdsl/include/sdsl)
link_directories(/opt/sdsl/lib)

set(SOURCE_FILE main.cpp)

add_executable(VisualSuffixTree main.cpp src/SvgCreator.cpp Include/SvgCreator.h src/TreeParser.cpp Include/TreeParser.h  src/ConfigParser.cpp Include/ConfigParser.h src/NodeInfoStructure.cpp Include/NodeInfoStructure.h src/NodeInfo.cpp Include/NodeInfo.h src/Header.cpp Include/Header.h Include/Header.h Include/TreeParser.h Include/SvgCreator.h Include/NodeInfo.h Include/NodeInfoStructure.h Include/ConfigParser.h Include/BitIo.h Include/ObjNode.h src/ObjNode.cpp Include/Utils.h)

target_link_libraries(VisualSuffixTree sdsl divsufsort divsufsort64)
