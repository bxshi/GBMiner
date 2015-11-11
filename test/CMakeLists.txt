cmake_minimum_required(VERSION 2.8)
project(KGMiner_test)

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif (APPLE)

find_package(Boost COMPONENTS log log_setup thread filesystem system)

include_directories(/usr/local/include /usr/include ../include ../kgserver) # all headers included here can be called by #include <...>
link_directories(/usr/local/lib /usr/lib)

add_executable(kgminer_test LoggerTest.cpp main.cpp)

target_link_libraries(kgminer_test
        gtest gtest_main
        ${Boost_LOG_SETUP_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
)

add_subdirectory(kgserver)