set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(SOURCE_FILES
        kcs/argparse/arg_flags.hpp
        kcs/argparse/basic_arg_parser.hpp
        kcs/argparse/arg_parser_exception.hpp
        kcs/argparse/arg_parser_flags.hpp
        kcs/argparse/basic_arg_value.hpp
        kcs/argparse/arg_value_error_flags.hpp
        kcs/argparse/arg_value_types.hpp
        kcs/argparse/basic_foreign_arg.hpp
        kcs/argparse/basic_help_arg.hpp
        kcs/argparse/basic_help_text.hpp
        kcs/argparse/basic_key_arg.hpp
        kcs/argparse/basic_key_value_arg.hpp
        kcs/argparse/basic_version_arg.hpp
        kcs/lowlevel/flags_container.hpp
        kcs/lowlevel/enum_bitwise_operators.hpp
        kcs/stringutils/stringutils.hpp
        kcs/type_traits/type_traits.hpp
        kcs/argparse.hpp
        kcs/kcs.hpp
        kcs/lowlevel.hpp
        kcs/stringutils.hpp
        kcs/type_traits.hpp
        kcs/argparse/basic_base_arg.hpp
        kcs/argparse/basic_value_arg.hpp
        kcs/exception/exception_base.hpp
        kcs/exception.hpp
        kcs/type_casting/type_cast.hpp
        kcs/type_casting/bad_type_cast.hpp
        kcs/type_casting.hpp
        kcs/type_casting/c_string_to_integral_signed.hpp
        kcs/iostream.hpp
        kcs/iostream/stdio.hpp
        kcs/argparse/basic_arg_key.hpp
        kcs/argparse/arg_error_flags.hpp
        kcs/argparse/arg_parser_error_flags.hpp
        kcs/system/process.hpp
        kcs/system.hpp
        kcs/math.hpp
        kcs/math/math.hpp
        kcs/lowlevel/arithmetic_operations.hpp
        kcs/type_casting/c_string_to_integral_unsigned.hpp
        kcs/type_casting/basic_string_to_type.hpp
        kcs/type_casting/c_string_to_basic_string.hpp
        kcs/type_casting/c_string_to_floating_point.hpp
        kcs/type_casting/c_string_to_path.hpp
        kcs/iostream/stdio.cpp
        kcs/argparse/ibasic_help_text.hpp 
        kcs/iostream/basic_ostream.hpp 
        kcs/iostream/basic_ios_redirect.hpp 
        kcs/system/input_output.hpp 
        kcs/system/input_output.cpp
        kcs/system/filesystem.cpp
        kcs/system/system_exception.hpp
        kcs/system/filesystem.hpp
        kcs/system/process.cpp)

add_library(kcs STATIC ${SOURCE_FILES})

set_target_properties(kcs PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(kcs -lstdc++fs)
