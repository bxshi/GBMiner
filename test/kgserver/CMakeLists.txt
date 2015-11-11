cmake_minimum_required(VERSION 2.8)
project(KGServer_test)

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

include_directories(/usr/local/include /usr/include ../../kgserver ../../include)
link_directories(/usr/local/lib /usr/lib)

set(TEST_GRAPH_PROPERTY_SOURCE GraphPropertyTest.cpp ../../kgserver/GraphProperty.h ../../kgserver/GraphProperty.cpp)

add_executable(kgserver_test basic_test.cpp ${TEST_GRAPH_PROPERTY_SOURCE} main.cpp)

target_link_libraries(kgserver_test gtest gtest_main)