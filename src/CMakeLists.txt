set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

include_directories(${CMAKE_SOURCE_DIR}/kcs/include)

set(SOURCE_FILES
        main.cpp
        runsource_entry.cpp
        runsource_entry.hpp
        language.hpp
        tool_chain.hpp cpp_standard.hpp c_standard.hpp)

add_executable(runsource ${SOURCE_FILES})
target_link_libraries(runsource kcs -lstdc++fs)
