cmake_minimum_required(VERSION 2.8)
project(KGClient)

add_definitions(-std=c++11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../bin")

find_package(Boost COMPONENTS log log_setup filesystem system program_options REQUIRED)

include_directories(${BOOST_INCLUDE_DIR} /usr/local/include /usr/include ../include ../include/rapidjson)
link_directories(/usr/local/lib /usr/lib)

set(SOURCE_FILES main.cpp Opts.cpp Opts.h)

add_executable(kgclient ${SOURCE_FILES})

target_link_libraries(kgclient ${Boost_LIBRARIES})
