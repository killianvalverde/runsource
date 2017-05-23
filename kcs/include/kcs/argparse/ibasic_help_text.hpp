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
 * @file        kcs/argparse/ibasic_help_text.hpp
 * @brief       ibasic_help_text class header.
 * @author      Killian
 * @date        2016/03/12 - 20:35
 */

#ifndef KCS_ARGPARSE_IBASIC_HELP_TEXT_HPP
#define KCS_ARGPARSE_IBASIC_HELP_TEXT_HPP

#include <string>


namespace kcs {
namespace argparse {


/**
 * @brief       Interface that represents a text that can be printed in the help menu.
 */
template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAlloc = std::allocator<int>
>
class ibasic_help_text
{
public:
    /**
     * @brief       Print the argument information for help menu.
     */
    virtual void print_help_text() const = 0;

};


/** Interface that represents a text that can be printed in the help menu with 8 bits characters. */
using ihelp_text = ibasic_help_text<char>;


/** Interface that represents a text that can be printed in the help menu with 16 bits
 * characters. */
using iwhelp_text = ibasic_help_text<wchar_t>;


}
}


#endif
