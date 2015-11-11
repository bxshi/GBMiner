cmake_minimum_required(VERSION 2.8)
project(KGServer)

add_definitions(-std=c++11)

include_directories(/usr/local/include /usr/include)
link_directories(/usr/local/include /usr/include)

add_executable(kgserver main.cpp)