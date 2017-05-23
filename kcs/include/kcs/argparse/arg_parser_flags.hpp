/* Copyright (C) 2017 Killian Poulaud.
   This file is part of KCS.

   KCS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   KCS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with KCS. If not, see <http://www.gnu.org/licenses/>. */

/**
 * @file        kcs/argparse/arg_parser_flags.hpp
 * @brief       arg_parser_flags enum header.
 * @author      Killian
 * @date        2016/08/07 - 01:08
 */

#ifndef KCS_ARGPARSE_ARG_PARSER_FLAGS_HPP
#define KCS_ARGPARSE_ARG_PARSER_FLAGS_HPP

#include "../lowlevel/enum_bitwise_operators.hpp"


namespace kcs {
namespace argparse {


/**
 * @brief       Contains all argument parser flags constants.
 */
enum class arg_parser_flags : std::uint16_t
{
    /** Null flag. */
    NULL_ARG_PARSER_FLAGS = 0x0,
    
    /** Print the help information when a help argument is found. */
    PRINT_HELP = 0x1,
    
    /** Print the version information when a version argument is found. */
    PRINT_VERSION = 0x2,
    
    /** If there are errors the errors will be printed in the standard output. */
    PRINT_ERRORS = 0x4,
    
    /** If there are arguments errors the 'try program_name --help' sentece will appear. */
    PRINT_TRY_HELP_ON_ERRORS = 0x8,
    
    /** If there are arguments errors the help information will be printed in the standard
     * output. */
    PRINT_HELP_ON_ERRORS = 0x10,
    
    /** The common 'usage' sentence will be printed in the beginning of the help information. */
    PRINT_USAGE_ON_PRINT_HELP = 0x20,
    
    /** The common 'Options' sentence will be printed in the beginning of the help information.  */
    PRINT_OPTIONS_ON_PRINT_HELP = 0x40,
    
    /** When the help information will be printed, the arguments keys or help ids will be printed
     * before the description. */
    PRINT_ARGS_ID_ON_PRINT_HELP = 0x80,
    
    /** The foreign arguments values cannot be added if the value contains one of the prefixes. */
    FOREIGN_VALUE_ARG_CANNOT_BE_ADDED_WITH_PREFIX = 0x100,
    
    /** Exit the program when the help information is printed. */
    EXIT_ON_PRINT_HELP = 0x200,
    
    /** Exit the program when the version information is printed. */
    EXIT_ON_PRINT_VERSION = 0x400,
    
    /** Exit the program when the args errors are printed. */
    EXIT_ON_PRINT_ARGS_ERRORS = 0x800,
    
    /** All argument parser flags. */
    FULL_ARG_PARSER_FLAGS = 0xFFF,
    
    /** The default flags used by the argument parser. */
    DEFAULT_ARG_PARSER_FLAGS = (
            PRINT_HELP |
            PRINT_VERSION |
            PRINT_ERRORS |
            PRINT_TRY_HELP_ON_ERRORS |
            PRINT_USAGE_ON_PRINT_HELP |
            PRINT_ARGS_ID_ON_PRINT_HELP |
            FOREIGN_VALUE_ARG_CANNOT_BE_ADDED_WITH_PREFIX |
            EXIT_ON_PRINT_HELP |
            EXIT_ON_PRINT_VERSION |
            EXIT_ON_PRINT_ARGS_ERRORS
    )
};


/** Contains all argument parser flags constants. Alias of 'arg_parser_flags' enumeration. */
using apf_t = arg_parser_flags;
    
    
}
}


/** @cond */
namespace kcs {
namespace lowlevel {
template<>
struct enum_bitwise_operators<kcs::argparse::arg_parser_flags>
{
    static constexpr bool enable = true;
};
}
}
/** @endcond */


#endif
