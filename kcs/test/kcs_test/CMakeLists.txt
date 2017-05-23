include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_executable(kcs_test
        main.cpp
        argparse_test/arg_parser_exception_test.cpp
        argparse_test/base_arg_test.cpp
        argparse_test/arg_parser_test.cpp
        )

target_link_libraries(kcs_test gtest gtest_main)
target_link_libraries(kcs_test kcs)
