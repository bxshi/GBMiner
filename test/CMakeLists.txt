cmake_minimum_required(VERSION 2.8)
project(KGMiner_test)

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

include_directories(/usr/local/include /usr/include ../include) # all headers included here can be called by #include <...>
link_directories(/usr/local/include /usr/include)

add_executable(kgminer_test LoggerTest.cpp main.cpp)

add_subdirectory(kgserver)