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
 * @file        kcs/iostream/basic_ostream.hpp
 * @brief       basic_ostream related fonctions header.
 * @author      Killian
 * @date        2016/08/24 - 09:50
 */

#ifndef KCS_IOSTREAM_BASIC_OSTREAM_HPP
#define KCS_IOSTREAM_BASIC_OSTREAM_HPP

#include <iostream>


namespace kcs {
namespace iostream {


/**
 * @brief       Get the current 'std::basic_ostream' object used to print TpChar in standard output.
 * @return      The current 'std::basic_ostream' object used to print TpChar in standard output.
 */
template<typename TpChar>
inline std::basic_ostream<TpChar>& get_cout() noexcept;


/**
 * @brief       Get the current 'std::ostream' object used to print in standard output.
 * @return      The current 'std::ostream' object used to print in standard output.
 */
template<>
inline std::ostream& get_cout<char>() noexcept
{
    return std::cout;
}


/**
 * @brief       Get the current 'std::wostream' object used to print in standard output.
 * @return      The current 'std::wostream' object used to print in standard output.
 */
template<>
inline std::wostream& get_cout<wchar_t>() noexcept
{
    return std::wcout;
}


}
}


#endif
