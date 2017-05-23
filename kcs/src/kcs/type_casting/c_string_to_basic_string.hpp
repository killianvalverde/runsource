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
 * @file        kcs/type_casting/c_string_to_basic_string.hpp
 * @brief       c_string_to_basic_string main header.
 * @author      Killian
 * @date        2016/08/19 - 15:39
 */

#ifndef KCS_TYPE_CASTING_C_STRING_TO_BASIC_STRING_HPP
#define KCS_TYPE_CASTING_C_STRING_TO_BASIC_STRING_HPP

#include <string>
#include <type_traits>

#include "../stringutils/stringutils.hpp"
#include "bad_type_cast.hpp"


namespace kcs {
namespace type_casting {


/**
 * @brief       Convert a C string into a std::basic_string.
 * @param       arg : The value to convert.
 * @return      The C string converted into a std::basic_string.
 * @throw       kcs::type_casting::bad_type_cast : If the conversion fail an exception is thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_basic_string<TpTarget>::value &&
                kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    TpTarget str;
    std::size_t len = kcs::stringutils::strlen(arg);
    
    str.assign(arg, arg + len);
    
    return str;
}


/**
 * @brief       Convert a C string into a std::basic_string.
 * @param       arg : The value to convert.
 * @param       result : The std::basic_string that will contain the result of the conversion.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_basic_string<TpTarget>::value &&
                kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value,
        bool
>
try_type_cast(const TpSource& arg, TpTarget& result) noexcept
{
    try
    {
        std::size_t length = kcs::stringutils::strlen(arg);
        
        if (length > 0)
        {
            result.assign(arg, arg + length);
            return true;
        }
    }
    catch (...)
    {
    }
    
    return false;
}


}
}


#endif
