cmake_minimum_required(VERSION 3.0)

project(runsource)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(SOURCE_FILES
        src/main.cpp
        src/program.cpp
        src/program.hpp
        src/language.hpp
        src/tool_chain.hpp 
        src/cpp_standard.hpp 
        src/c_standard.hpp
        )

add_executable(runsource ${SOURCE_FILES})
target_link_libraries(runsource -lkboost -lstdc++fs)
