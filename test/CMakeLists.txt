cmake_minimum_required(VERSION 3.0)

project(runsource_test)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/kcs/include)

set(SOURCE_FILES
        main.cpp
        build_manger_test.cpp
        ../src/runsource_entry.cpp
        ../src/runsource_entry.hpp
        ../src/language.hpp
        )

add_executable(runsource_test ${SOURCE_FILES})

target_link_libraries(runsource_test gtest gtest_main)
target_link_libraries(runsource_test kcs)
