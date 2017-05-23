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
 * @file        kcs/type_casting/c_string_to_floating_point.hpp
 * @brief       c_string_to_floating_point main header.
 * @author      Killian
 * @date        2016/08/19 - 18:02
 */

#ifndef KCS_TYPE_CASTING_C_STRING_TO_FLOATING_POINT_HPP
#define KCS_TYPE_CASTING_C_STRING_TO_FLOATING_POINT_HPP

#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <type_traits>

#include "../type_traits/type_traits.hpp"
#include "bad_type_cast.hpp"


namespace kcs {
namespace type_casting {


/**
 * @brief       Convert a C string into a float.
 * @param       arg : The value to convert.
 * @return      The C string converted into a float.
 * @throw       kcs::type_casting::bad_type_cast : If the conversion fail an exception is thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                kcs::type_traits::is_char<std::decay_t<std::remove_pointer_t<TpSource>>>::value &&
                kcs::type_traits::is_float<TpTarget>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    using floating_point_type = TpTarget;
    
    errno = 0;
    char* endptr;
    floating_point_type res = strtof(arg, &endptr);
    
    if (endptr == arg || *endptr != '\0')
    {
        throw bad_type_cast("kcs::type_casting::bad_type_cast: source value could not be "
                                    "interpreted as target");
    }
    
    if (res == HUGE_VALF && errno == ERANGE)
    {
        throw bad_type_cast("kcs::type_casting::bad_type_cast: overflow range error");
    }
    
    if (errno == ERANGE)
    {
        throw bad_type_cast("kcs::type_casting::bad_type_cast: underflow range error");
    }
    
    return res;
}


/**
 * @brief       Convert a string into a float.
 * @param       arg : The value to convert.
 * @return      The string converted into a float.
 * @throw       kcs::type_casting::bad_type_cast : If the conversion fail an exception is thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                !kcs::type_traits::is_char<std::decay_t<std::remove_pointer_t<TpSource>>>::value &&
                kcs::type_traits::is_float<TpTarget>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    return type_cast<TpTarget>(type_cast<std::string>(arg).c_str());
}


/**
 * @brief       Convert a C string into a float.
 * @param       arg : The value to convert.
 * @param       result : The float that will contain the result of the conversion.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                kcs::type_traits::is_char<std::decay_t<std::remove_pointer_t<TpSource>>>::value &&
                kcs::type_traits::is_float<TpTarget>::value,
        bool
>
try_type_cast(const TpSource& arg, TpTarget& result)
{
    using floating_point_type = TpTarget;
    
    errno = 0;
    char* endptr;
    result = strtof(arg, &endptr);
    
    if (endptr == arg || *endptr != '\0')
    {
        return false;
    }
    
    if (result == HUGE_VALF && errno == ERANGE)
    {
        return false;
    }
    
    if (errno == ERANGE)
    {
        return false;
    }
    
    return true;
}


/**
 * @brief       Convert a string into a float.
 * @param       arg : The value to convert.
 * @param       result : The float that will contain the result of the conversion.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                !kcs::type_traits::is_char<std::decay_t<std::remove_pointer_t<TpSource>>>::value &&
                kcs::type_traits::is_float<TpTarget>::value,
        bool
>
try_type_cast(const TpSource& arg, TpTarget& result)
{
    return try_type_cast<TpTarget>(type_cast<std::string>(arg).c_str(), result);
}


/**
 * @brief       Convert a C string into a double.
 * @param       arg : The value to convert.
 * @return      The C string converted into a double.
 * @throw       kcs::type_casting::bad_type_cast : If the conversion fail an exception is thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                kcs::type_traits::is_char<std::decay_t<std::remove_pointer_t<TpSource>>>::value &&
                kcs::type_traits::is_double<TpTarget>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    using floating_point_type = TpTarget;
    
    errno = 0;
    char* endptr;
    floating_point_type res = strtod(arg, &endptr);
    
    if (endptr == arg || *endptr != '\0')
    {
        throw bad_type_cast("kcs::type_casting::bad_type_cast: source value could not be "
                                    "interpreted as target");
    }
    
    if (res == HUGE_VAL && errno == ERANGE)
    {
        throw bad_type_cast("kcs::type_casting::bad_type_cast: overflow range error");
    }
    
    if (errno == ERANGE)
    {
        throw bad_type_cast("kcs::type_casting::bad_type_cast: underflow range error");
    }
    
    return res;
}


/**
 * @brief       Convert a string into a double.
 * @param       arg : The value to convert.
 * @return      The string converted into a double.
 * @throw       kcs::type_casting::bad_type_cast : If the conversion fail an exception is thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                !kcs::type_traits::is_char<std::decay_t<std::remove_pointer_t<TpSource>>>::value &&
                kcs::type_traits::is_double<TpTarget>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    return type_cast<TpTarget>(type_cast<std::string>(arg).c_str());
}


/**
 * @brief       Convert a C string into a double.
 * @param       arg : The value to convert.
 * @param       result : The double that will contain the result of the conversion.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                kcs::type_traits::is_char<std::decay_t<std::remove_pointer_t<TpSource>>>::value &&
                kcs::type_traits::is_double<TpTarget>::value,
        bool
>
try_type_cast(const TpSource& arg, TpTarget& result)
{
    using floating_point_type = TpTarget;
    
    errno = 0;
    char* endptr;
    result = strtod(arg, &endptr);
    
    if (endptr == arg || *endptr != '\0')
    {
        return false;
    }
    
    if (result == HUGE_VAL && errno == ERANGE)
    {
        return false;
    }
    
    if (errno == ERANGE)
    {
        return false;
    }
    
    return true;
}


/**
 * @brief       Convert a string into a double.
 * @param       arg : The value to convert.
 * @param       result : The double that will contain the result of the conversion.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                !kcs::type_traits::is_char<std::decay_t<std::remove_pointer_t<TpSource>>>::value &&
                kcs::type_traits::is_double<TpTarget>::value,
        bool
>
try_type_cast(const TpSource& arg, TpTarget& result)
{
    return try_type_cast<TpTarget>(type_cast<std::string>(arg).c_str(), result);
}


/**
 * @brief       Convert a C string into a long double.
 * @param       arg : The value to convert.
 * @return      The C string converted into a long double.
 * @throw       kcs::type_casting::bad_type_cast : If the conversion fail an exception is thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                kcs::type_traits::is_char<std::decay_t<std::remove_pointer_t<TpSource>>>::value &&
                kcs::type_traits::is_long_double<TpTarget>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    using floating_point_type = TpTarget;
    
    errno = 0;
    char* endptr;
    floating_point_type res = strtold(arg, &endptr);
    
    if (endptr == arg || *endptr != '\0')
    {
        throw bad_type_cast("kcs::type_casting::bad_type_cast: source value could not be "
                                    "interpreted as target");
    }
    
    if (res == HUGE_VALL && errno == ERANGE)
    {
        throw bad_type_cast("kcs::type_casting::bad_type_cast: overflow range error");
    }
    
    if (errno == ERANGE)
    {
        throw bad_type_cast("kcs::type_casting::bad_type_cast: underflow range error");
    }
    
    return res;
}


/**
 * @brief       Convert a string into a long double.
 * @param       arg : The value to convert.
 * @return      The string converted into a long double.
 * @throw       kcs::type_casting::bad_type_cast : If the conversion fail an exception is thrown.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                !kcs::type_traits::is_char<std::decay_t<std::remove_pointer_t<TpSource>>>::value &&
                kcs::type_traits::is_long_double<TpTarget>::value,
        TpTarget
>
type_cast(const TpSource& arg)
{
    return type_cast<TpTarget>(type_cast<std::string>(arg).c_str());
}


/**
 * @brief       Convert a C string into a long double.
 * @param       arg : The value to convert.
 * @param       result : The long double that will contain the result of the conversion.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                kcs::type_traits::is_char<std::decay_t<std::remove_pointer_t<TpSource>>>::value &&
                kcs::type_traits::is_long_double<TpTarget>::value,
        bool
>
try_type_cast(const TpSource& arg, TpTarget& result)
{
    using floating_point_type = TpTarget;
    
    errno = 0;
    char* endptr;
    result = strtold(arg, &endptr);
    
    if (endptr == arg || *endptr != '\0')
    {
        return false;
    }
    
    if (result == HUGE_VALL && errno == ERANGE)
    {
        return false;
    }
    
    if (errno == ERANGE)
    {
        return false;
    }
    
    return true;
}


/**
 * @brief       Convert a C string into a long double.
 * @param       arg : The value to convert.
 * @param       result : The long double that will contain the result of the conversion.
 * @return      If function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpSource>
std::enable_if_t<
        kcs::type_traits::is_character_pointer<std::decay_t<TpSource>>::value &&
                !kcs::type_traits::is_char<std::decay_t<std::remove_pointer_t<TpSource>>>::value &&
                kcs::type_traits::is_long_double<TpTarget>::value,
        bool
>
try_type_cast(const TpSource& arg, TpTarget& result)
{
    return try_type_cast<TpTarget>(type_cast<std::string>(arg).c_str(), result);
}


}
}


#endif

