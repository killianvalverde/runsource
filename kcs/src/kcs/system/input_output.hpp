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
 * @file        kcs/system/input_output.hpp
 * @brief       input and output fonctions header.
 * @author      Killian
 * @date        2017/05/23 - 15:30
 */

#ifndef KCS_SYSTEM_INPUT_OUTPUT_HPP
#define KCS_SYSTEM_INPUT_OUTPUT_HPP

#include <iostream>


namespace kcs {
namespace system {


enum class text_attribute
{
    NIL,
    DEFAULT,
    BLACK,
    RED,
    GREEN,
    BROWN,
    BLUE,
    PURPLE,
    CYAN,
    LIGHT_GRAY,
    DARK_GRAY,
    LIGHT_RED,
    LIGHT_GREEN,
    YELLOW,
    LIGHT_BLUE,
    LIGHT_PURPLE,
    LIGHT_CYAN,
    WHITE
};


/**
 * @brief       Print a message and pause de program execution until a key is pressed
 * @param       message : The message to print before the pause process. If a null pointer is used
 *              there will be no message printed.
 */
void pause(const char *message);


/**
 * @brief       Set a stream text attribute.
 * @param       stream : Stream in which set the attribute.
 * @param       attribute : Attribute to set.
 * @return      If function was successful 0 is returned, otherwise -1 is returned.
 */
int set_stream_text_attribute(::FILE *stream, text_attribute attribute);


/**
 * @brief       Set a Ostream text attribute.
 * @param       os : Ostream in which set the attribute.
 * @param       attribute : Attribute to set.
 * @return      If function was successful 0 is returned, otherwise -1 is returned.
 */
int set_ostream_text_attribute(std::ostream& os, text_attribute attribute);
// todo : implement error management.


}
}


#endif
