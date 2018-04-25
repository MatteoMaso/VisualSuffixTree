#the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

project(VisualSuffixTree)

set(CMAKE_CXX_STANDARD 11)

#used for header library file
include_directories(/opt/sdsl/include/sdsl)
link_directories(/opt/sdsl/lib)

set(SOURCE_FILE main.cpp)

add_executable(VisualSuffixTree main.cpp src/SvgCreator.cpp Include/SvgCreator.h src/TreeParser.cpp Include/TreeParser.h Include/BitIo.h Include/BitEncode.h Include/BitDecoder.h src/BitDecoder.cpp src/BitEncode.cpp)

target_link_libraries(VisualSuffixTree sdsl divsufsort divsufsort64)
