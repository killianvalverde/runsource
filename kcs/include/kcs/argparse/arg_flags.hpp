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
 * @file        kcs/argparse/arg_flags.hpp
 * @brief       arg_flags enum header.
 * @author      Killian
 * @date        2016/03/10 - 15:56
 */

#ifndef KCS_ARGPARSE_ARG_FLAGS_HPP
#define KCS_ARGPARSE_ARG_FLAGS_HPP

#include "../lowlevel/enum_bitwise_operators.hpp"


namespace kcs {
namespace argparse {


/**
 * @brief       Contains all argument flags constants.
 */
enum class arg_flags : std::uint8_t
{
    /** Null flag. */
    NIL = 0x0,
    
    /** The argument allways has to be present in the program call. */
    ALLWAYS_REQUIRED = 0x1,
    
    /** The argument must appear just once in the program call. */
    APPEAR_JUST_ONCE = 0x2,
    
    /** The argument can be chained (-la == -l -a). The minimum and the maximum number of values for
     * an option has to be the same. */
    ALLOW_CHAIN = 0x4,
    
    /** The argument allows the equal operator (--tries=NUMBER). The minimum and the maximum number
     * of values for an option has to be one. */
    ALLOW_EQ = 0x8,
    
    /** The 'min' constant can be used to refer to the minimum value of an integral type. */
    ALLOW_MIN_CONSTANT = 0x10,
    
    /** The 'max' constant can be used to refer to the maximum value of an integral type. */
    ALLOW_MAX_CONSTANT = 0x20,
    
    /** The first key of an argument will be used to print the error id if the error id is empty. */
    USE_FIRST_KEY_IF_ERROR_ID_EMPTY = 0x40,
    
    /** Print the argument error id if there is an error with a path. */
    PRINT_ERROR_ID_ON_PATH_ERROR = 0x80,
    
    /** All argument flags. */
    ALL = 0xFF,
    
    /** The default flags used for the arguments. */
    DEFAULT_ARG_FLAGS = (
            APPEAR_JUST_ONCE |
            ALLOW_CHAIN |
            ALLOW_EQ |
            ALLOW_MIN_CONSTANT |
            ALLOW_MAX_CONSTANT |
            USE_FIRST_KEY_IF_ERROR_ID_EMPTY
    ),
    
    DEFAULT_FOREIGN_ARG_FLAGS = (
            APPEAR_JUST_ONCE |
            ALLOW_MIN_CONSTANT |
            ALLOW_MAX_CONSTANT |
            ALLWAYS_REQUIRED |
            USE_FIRST_KEY_IF_ERROR_ID_EMPTY
    )
};


/** Contains all argument flags constants. Alias of 'arg_flags' enumeration. */
using af_t = arg_flags;
    
    
}
}


/** @cond */
namespace kcs {
namespace lowlevel {
template<>
struct enum_bitwise_operators<kcs::argparse::arg_flags>
{
    static constexpr bool enable = true;
};
}
}
/** @endcond */


#endif
