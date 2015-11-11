cmake_minimum_required(VERSION 2.8)
project(KGServer)

add_definitions(-std=c++11)

include_directories(/usr/local/include /usr/include ../include)
link_directories(/usr/local/lib /usr/lib)

set(SOURCE_FILES GraphProperty.cpp GraphProperty.h AbstractGraph.h AbstractGraph.cpp)

add_executable(kgserver main.cpp ${SOURCE_FILES})