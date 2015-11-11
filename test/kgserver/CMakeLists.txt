cmake_minimum_required(VERSION 2.8)
project(KGServer_test)

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

find_package(Boost COMPONENTS log log_setup thread filesystem system)

include_directories(/usr/local/include /usr/include ../../kgserver ../../include)
link_directories(/usr/local/lib /usr/lib)

set(SOURCE_FILES GraphPropertyTest.cpp ../../kgserver/GraphProperty.h ../../kgserver/GraphProperty.cpp
                    TypedDirectedGraphTest.cpp ../../kgserver/TypedDirectedGraph.h ../../kgserver/TypedDirectedGraph.cpp ../../kgserver/AbstractTypedGraph.h
                 ../../kgserver/AbstractGraph.h)

add_executable(kgserver_test basic_test.cpp ${SOURCE_FILES} main.cpp)

target_link_libraries(kgserver_test gtest gtest_main
        ${Boost_LOG_SETUP_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
)