cmake_minimum_required(VERSION 2.8)
project(KGServer_include)

add_definitions(-std=c++11)

include_directories(/usr/local/include /usr/include)
link_directories(/usr/local/lib /usr/lib)

set(SOURCE_FILES Logger.h main.cpp)

add_executable(KGServer_include ${SOURCE_FILES})