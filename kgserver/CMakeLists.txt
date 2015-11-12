cmake_minimum_required(VERSION 2.8)
project(KGServer)

add_definitions(-std=c++11)

find_package(Boost COMPONENTS log log_setup thread filesystem system)

include_directories(/usr/local/include /usr/include ../include) # all headers included here can be called by #include <...>
link_directories(/usr/local/lib /usr/lib)

set(SOURCE_FILES GraphProperty.cpp GraphProperty.h
        AbstractGraph.h
        AbstractTypedGraph.h
        TypedDirectedGraph.h TypedDirectedGraph.cpp AbstractPath.h Path.cpp Path.h)

add_executable(kgserver main.cpp ${SOURCE_FILES})
target_link_libraries(kgserver
        gtest gtest_main
        ${Boost_LOG_SETUP_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
)
