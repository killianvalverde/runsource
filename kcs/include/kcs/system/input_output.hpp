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

/**
 * @brief       Represents text attributes.
 */
enum class text_attribute
{
    /** Nil text attribute. */
    NIL,
    
    /** Default text attribute */
    DEFAULT,
    
    /** Black text. */
    BLACK,
    
    /** Red text. */
    RED,
    
    /** Green text. */
    GREEN,
    
    /** Brown text. */
    BROWN,
    
    /** Blue text. */
    BLUE,
    
    /** Purple text. */
    PURPLE,
    
    /** Cyan text. */
    CYAN,
    
    /** Light gray text. */
    LIGHT_GRAY,
    
    /** Dark gray text. */
    DARK_GRAY,
    
    /** Light red text. */
    LIGHT_RED,
    
    /** Light green text. */
    LIGHT_GREEN,
    
    /** Yellow text. */
    YELLOW,
    
    /** Light blue text. */
    LIGHT_BLUE,
    
    /** Light purple text. */
    LIGHT_PURPLE,
    
    /** Light cyan text. */
    LIGHT_CYAN,
    
    /** White text. */
    WHITE
};


/**
 * @brief       Print a message and pause de program execution until a key is pressed
 * @param       message : The message to print before the pause process. If a null pointer is used
 *              there won't be a message printed.
 * @throw       kcs::system::system_exception : If the system is not suported an exception in
 *              thrown.
 */
void pause(const char *message);


/**
 * @brief       Set a stream text attribute.
 * @param       stream : Stream in which set the attribute.
 * @param       attribute : Attribute to set.
 * @return      If function was successful 0 is returned, otherwise -1 is returned.
 * @throw       kcs::system::system_exception : If the system is not suported an exception in
 *              thrown.
 */
int set_stream_text_attribute(::FILE *stream, text_attribute attribute);


/**
 * @brief       Set a Ostream text attribute.
 * @param       os : Ostream in which set the attribute.
 * @param       attribute : Attribute to set.
 * @return      If function was successful 0 is returned, otherwise -1 is returned.
 * @throw       kcs::system::system_exception : If the system is not suported an exception in
 *              thrown.
 */
int set_ostream_text_attribute(std::ostream& os, text_attribute attribute);


}
}


#endif
