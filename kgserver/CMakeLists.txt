cmake_minimum_required(VERSION 2.8)
project(KGServer)

add_definitions(-std=c++11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../bin")

find_package(Boost COMPONENTS log log_setup thread filesystem system program_options REQUIRED)

include_directories(${Boost_INCLUDE_DIR} /usr/local/include /usr/include ../include ../include/rapidjson) # all headers included here can be called by #include <...>
link_directories(/usr/local/lib /usr/lib)

set(SOURCE_FILES GraphProperty.cpp GraphProperty.h
        AbstractGraph.h AbstractTypedGraph.h
        TypedDirectedGraph.h TypedDirectedGraph.cpp
        Path.cpp Path.h
        AbstractParser.h
        jsonParser.cpp jsonParser.h
        SocketServer.cpp SocketServer.h
        opts.cpp opts.h
        Worker.cpp Worker.h)

add_executable(kgserver main.cpp ${SOURCE_FILES})
target_link_libraries(kgserver
        gtest gtest_main
        ${Boost_LIBRARIES}
)
