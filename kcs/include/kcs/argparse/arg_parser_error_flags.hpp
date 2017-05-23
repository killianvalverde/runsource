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
 * @file        kcs/argparse/arg_parser_error_flags.hpp
 * @brief       arg_parser_error_flags enum header.
 * @author      Killian
 * @date        2016/08/07 - 01:08
 */

#ifndef KCS_ARGPARSE_ARG_PARSER_ERROR_FLAGS_HPP
#define KCS_ARGPARSE_ARG_PARSER_ERROR_FLAGS_HPP

#include "../lowlevel/enum_bitwise_operators.hpp"


namespace kcs {
namespace argparse {


/**
 * @brief       Contains all argument parser error flags constants.
 */
enum class arg_parser_error_flags : std::uint8_t
{
    /** Null flag. */
    NULL_ARG_PARSER_ERROR_FLAGS = 0x0,
    
    /** The argument parser has arguments errors. */
    ARGS_ERROR = 0x1,
    
    /** The argument parser has found arguments that aren't added in the parser and there aren't any
     * foreign argument to catch them. */
    UNRECOGNIZED_ARGS_ERROR = 0x2,
    
    /** All argument parser error flags. */
    FULL_ARG_PARSER_ERROR_FLAGS = 0x3
};


/** Contains all argument parser error flags constants. Alias of 'arg_parser_error_flags'
 *  enumeration. */
using apef_t = arg_parser_error_flags;


}
}


/** @cond */
namespace kcs {
namespace lowlevel {
template<>
struct enum_bitwise_operators<kcs::argparse::arg_parser_error_flags>
{
    static constexpr bool enable = true;
};
}
}
/** @endcond */


#endif
