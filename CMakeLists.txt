cmake_minimum_required(VERSION 2.8)
project(KGMiner)

add_definitions(-std=c++11)

add_subdirectory(kgserver)
add_subdirectory(kgclient)
add_subdirectory(test)