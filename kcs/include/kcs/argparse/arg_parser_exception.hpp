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
 * @file        kcs/argparse/arg_parser_exception.hpp
 * @brief       arg_parser_exception class header.
 * @author      Killian
 * @date        2016/01/01 - 02:24
 */

#ifndef KCS_ARGPARSE_ARG_PARSER_EXCEPTION_HPP
#define KCS_ARGPARSE_ARG_PARSER_EXCEPTION_HPP

#include "kcs/exception/exception_base.hpp"


namespace kcs {
namespace argparse {


/**
 * @brief       argparse_exception is the class that the arguement parser will use to throw
 *              exceptions.
 */
class arg_parser_exception : public kcs::exception::exception_base
{
public:
    /**
     * @brief       Constructor with parameters.
     * @param       str : Message to return by the exception object.
     */
    inline arg_parser_exception(const char* const str = nullptr) noexcept
            : exception_base(str)
    {
    }
};


}
}


#endif
