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
 * @file        kcs/iostream/stdio.hpp
 * @brief       stdio functions header.
 * @author      Killian
 * @date        2016/08/24 - 09:50
 */

#ifndef KCS_IOSTREAM_STDIO_HPP
#define KCS_IOSTREAM_STDIO_HPP

#include <iostream>


namespace kcs {
namespace iostream {


/**
 * @brief       Writes the C string pointed by format to the standard output (stdout). If format
 *              includes format specifiers (subsequences beginning with %), the additional arguments
 *              following format are formatted and inserted in the resulting string replacing their
 *              respective specifiers.
 * @param       format : C string that contains the text to be written to stdout. It can optionally
 *              contain embedded format specifiers that are replaced by the values specified in
 *              subsequent additional arguments and formatted as requested.
 * @return      On success, the total number of characters written is returned. If a writing error
 *              occurs, the error indicator (ferror) is set and a negative number is returned. If a
 *              multibyte character encoding error occurs while writing wide characters, errno is
 *              set to EILSEQ and a negative number is returned.
 */
int printf(const char* format, ...) noexcept;


/**
 * @brief       Writes the C string pointed by format to the standard output (stdout). If format
 *              includes format specifiers (subsequences beginning with %), the additional arguments
 *              following format are formatted and inserted in the resulting string replacing their
 *              respective specifiers.
 * @param       format : C string that contains the text to be written to stdout. It can optionally
 *              contain embedded format specifiers that are replaced by the values specified in
 *              subsequent additional arguments and formatted as requested.
 * @return      On success, the total number of characters written is returned. If a writing error
 *              occurs, the error indicator (ferror) is set and a negative number is returned. If a
 *              multibyte character encoding error occurs while writing wide characters, errno is
 *              set to EILSEQ and a negative number is returned.
 */
int printf(const wchar_t* format, ...) noexcept;


}
}


#endif
