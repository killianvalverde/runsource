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
 * @file        kcs/type_casting/c_string_to_integral_unsigned.hpp
 * @brief       c_string_to_integral_unsigned main header.
 * @author      Killian
 * @date        2016/08/19 - 18:02
 */

#ifndef KCS_TYPE_CASTING_C_STRING_TO_INTEGRAL_UNSIGNED_HPP
#define KCS_TYPE_CASTING_C_STRING_TO_INTEGRAL_UNSIGNED_HPP

#include <cctype>
#include <limits>
#include <type_traits>

#include "../type_traits/type_traits.hpp"
#include "bad_type_cast.hpp"


namespace kcs {
namespace type_casting {


/**
 * @brief       Convert a C string into a integral unsigned type.
 * @param       arg : The value to convert.
 * @return      The C string converted into a integral unsigned type.
 * @throw       kcs::type_casting::bad_type_cast : If the conversion fail an exception is thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                std::is_integral<TpTarget>::value &&
                std::is_unsigned<TpTarget>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    using unsigned_type = TpTarget;
    using char_type = std::remove_const_t<std::remove_pointer_t<std::decay_t<TpSource>>>;
    using string_pointer_type = const char_type*;
    
    constexpr unsigned_type max_unsigned = std::numeric_limits<unsigned_type>::max();
    
    string_pointer_type str = arg;
    unsigned_type result = 0;
    char_type sign;
    char_type digit;
    
    if (str == nullptr)
    {
        throw bad_type_cast("kcs::type_casting::bad_type_cast: source value could not be "
                                    "interpreted as target");
    }
    
    while (isspace(*str))
    {
        str++;
    }
    
    sign = *str;
    if (sign == '+')
    {
        str++;
    }
    
    if (*str == '\0')
    {
        throw bad_type_cast("kcs::type_casting::bad_type_cast: source value could not be "
                                    "interpreted as target");
    }
    
    while ((digit = *str) != '\0')
    {
        if (!isdigit(digit))
        {
            throw bad_type_cast("kcs::type_casting::bad_type_cast: source value could not be "
                                        "interpreted as target");
        }
        
        digit -= '0';
        
        if ((result > max_unsigned / 10) ||
            (result == max_unsigned / 10 && digit > (max_unsigned % 10)))
        {
            throw bad_type_cast("kcs::type_casting::bad_type_cast: overflow range error");
        }
        
        result = result * 10 + digit;
        str++;
    }
    
    return result;
}


/**
 * @brief       Convert a C string into a integral unsigned type.
 * @param       arg : The value to convert.
 * @param       result : The integral unsigned type that will contain the result of the conversion.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                std::is_integral<TpTarget>::value &&
                std::is_unsigned<TpTarget>::value,
        bool
>
try_type_cast(const TpSource& arg, TpTarget& result) noexcept
{
    using unsigned_type = TpTarget;
    using char_type = std::remove_const_t<std::remove_pointer_t<std::decay_t<TpSource>>>;
    using string_pointer_type = const char_type*;
    
    constexpr unsigned_type max_unsigned = std::numeric_limits<unsigned_type>::max();
    
    result = 0;
    string_pointer_type str = arg;
    unsigned_type result_builder = 0;
    char_type sign;
    char_type digit;
    
    if (str == nullptr)
    {
        return false;
    }
    
    while (isspace(*str))
    {
        str++;
    }
    
    sign = *str;
    if (sign == '+')
    {
        str++;
    }
    
    if (*str == '\0')
    {
        return false;
    }
    
    while ((digit = *str) != '\0')
    {
        if (!isdigit(digit))
        {
            return false;
        }
        
        digit -= '0';
        
        if ((result_builder > max_unsigned / 10) ||
            (result_builder == max_unsigned / 10 && digit > (max_unsigned % 10)))
        {
            return false;
        }
        
        result_builder = result_builder * 10 + digit;
        str++;
    }
    
    result = result_builder;
    return true;
}


}
}


#endif
