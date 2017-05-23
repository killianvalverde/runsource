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
 * @file       kcs/lowlevel/enum_bitwise_operators.hpp
 * @brief      enum_bitwise_operators functions header.
 * @author     Killian
 * @date       2016/08/05 - 14:09
 */

#ifndef KCS_LOWLEVEL_ENUM_BITWISE_OPERATORS_HPP
#define KCS_LOWLEVEL_ENUM_BITWISE_OPERATORS_HPP

#include <iostream>
#include <type_traits>

#include "../type_traits/type_traits.hpp"


namespace kcs {
namespace lowlevel {


/** @cond */
template<typename T>
struct enum_bitwise_operators
{
    static constexpr bool enable = false;
    static constexpr bool enable_with_rhs_integrals = false;
    static constexpr bool enable_with_lhs_integrals = false;
    static constexpr bool enable_basic_ostream_stdout = false;
};


template<typename T>
std::enable_if_t<
        enum_bitwise_operators<T>::enable,
        T
>
operator |(const T& lhs, const T& rhs)
{
    typedef std::underlying_type_t<T> underlying_type;
    return static_cast<T>(static_cast<underlying_type>(lhs) | static_cast<underlying_type>(rhs));
}


template<typename TpEnum, typename TpIntegral>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_rhs_integrals,
        TpEnum
>
operator |(const TpEnum& lhs, const TpIntegral& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return static_cast<TpEnum>(static_cast<underlying_type>(lhs) |
            static_cast<underlying_type>(rhs));
}


template<typename TpIntegral, typename TpEnum>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_lhs_integrals,
        TpEnum
>
operator |(const TpIntegral& lhs, const TpEnum& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return static_cast<TpEnum>(static_cast<underlying_type>(lhs) |
            static_cast<underlying_type>(rhs));
}


template<typename T>
std::enable_if_t<
        enum_bitwise_operators<T>::enable,
        T
>
operator &(const T& lhs, const T& rhs)
{
    typedef std::underlying_type_t<T> underlying_type;
    return static_cast<T>(static_cast<underlying_type>(lhs) & static_cast<underlying_type>(rhs));
}


template<typename TpEnum, typename TpIntegral>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_rhs_integrals,
        TpEnum
>
operator &(const TpEnum& lhs, const TpIntegral& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return static_cast<TpEnum>(static_cast<underlying_type>(lhs) &
            static_cast<underlying_type>(rhs));
}


template<typename TpIntegral, typename TpEnum>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_lhs_integrals,
        TpEnum
>
operator &(const TpIntegral& lhs, const TpEnum& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return static_cast<TpEnum>(static_cast<underlying_type>(lhs) &
            static_cast<underlying_type>(rhs));
}


template<typename T>
std::enable_if_t<
        enum_bitwise_operators<T>::enable,
        T
>
operator ^(const T& lhs, const T& rhs)
{
    typedef std::underlying_type_t<T> underlying_type;
    return static_cast<T>(static_cast<underlying_type>(lhs) ^ static_cast<underlying_type>(rhs));
}


template<typename TpEnum, typename TpIntegral>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_rhs_integrals,
        TpEnum
>
operator ^(const TpEnum& lhs, const TpIntegral& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return static_cast<TpEnum>(static_cast<underlying_type>(lhs) ^
            static_cast<underlying_type>(rhs));
}


template<typename TpIntegral, typename TpEnum>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_lhs_integrals,
        TpEnum
>
operator ^(const TpIntegral& lhs, const TpEnum& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return static_cast<TpEnum>(static_cast<underlying_type>(lhs) ^
            static_cast<underlying_type>(rhs));
}


template<typename T>
std::enable_if_t<
        enum_bitwise_operators<T>::enable,
        T
>
operator <<(const T& lhs, const T& rhs)
{
    typedef std::underlying_type_t<T> underlying_type;
    return static_cast<T>(static_cast<underlying_type>(lhs) << static_cast<underlying_type>(rhs));
}


template<typename TpEnum, typename TpIntegral>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_rhs_integrals,
        TpEnum
>
operator <<(const TpEnum& lhs, const TpIntegral& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return static_cast<TpEnum>(static_cast<underlying_type>(lhs) <<
            static_cast<underlying_type>(rhs));
}


template<typename TpIntegral, typename TpEnum>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_lhs_integrals,
        TpEnum
>
operator <<(const TpIntegral& lhs, const TpEnum& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return static_cast<TpEnum>(static_cast<underlying_type>(lhs) <<
            static_cast<underlying_type>(rhs));
}


template<typename T>
std::enable_if_t<
        enum_bitwise_operators<T>::enable,
        T
>
operator >>(const T& lhs, const T& rhs)
{
    typedef std::underlying_type_t<T> underlying_type;
    return static_cast<T>(static_cast<underlying_type>(lhs) >> static_cast<underlying_type>(rhs));
}


template<typename TpEnum, typename TpIntegral>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_rhs_integrals,
        TpEnum
>
operator >>(const TpEnum& lhs, const TpIntegral& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return static_cast<TpEnum>(static_cast<underlying_type>(lhs) >>
            static_cast<underlying_type>(rhs));
}


template<typename TpIntegral, typename TpEnum>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_lhs_integrals,
        TpEnum
>
operator >>(const TpIntegral& lhs, const TpEnum& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return static_cast<TpEnum>(static_cast<underlying_type>(lhs) >>
            static_cast<underlying_type>(rhs));
}


template<typename T>
std::enable_if_t<
        enum_bitwise_operators<T>::enable,
        T
>
operator ~(const T& lhs)
{
    typedef std::underlying_type_t<T> underlying_type;
    return static_cast<T>(~static_cast<underlying_type>(lhs));
}


template<typename T>
std::enable_if_t<
        enum_bitwise_operators<T>::enable,
        T&
>
operator |=(T& lhs, const T& rhs)
{
    typedef std::underlying_type_t<T> underlying_type;
    return (lhs = static_cast<T>(static_cast<underlying_type>(lhs) |
            static_cast<underlying_type>(rhs)));
}


template<typename TpEnum, typename TpIntegral>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_rhs_integrals,
        TpEnum&
>
operator |=(TpEnum& lhs, const TpIntegral& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return (lhs = static_cast<TpEnum>(static_cast<underlying_type>(lhs) |
            static_cast<underlying_type>(rhs)));
}


template<typename TpIntegral, typename TpEnum>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_lhs_integrals,
        TpIntegral&
>
operator |=(TpIntegral& lhs, const TpEnum& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return (lhs = static_cast<TpIntegral>(static_cast<underlying_type>(lhs) |
            static_cast<underlying_type>(rhs)));
}


template<typename T>
std::enable_if_t<
        enum_bitwise_operators<T>::enable,
        T&
>
operator &=(T& lhs, const T& rhs)
{
    typedef std::underlying_type_t<T> underlying_type;
    return (lhs = static_cast<T>(static_cast<underlying_type>(lhs) &
            static_cast<underlying_type>(rhs)));
}


template<typename TpEnum, typename TpIntegral>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_rhs_integrals,
        TpEnum&
>
operator &=(TpEnum& lhs, const TpIntegral& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return (lhs = static_cast<TpEnum>(static_cast<underlying_type>(lhs) &
            static_cast<underlying_type>(rhs)));
}


template<typename TpIntegral, typename TpEnum>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_lhs_integrals,
        TpIntegral&
>
operator &=(TpIntegral& lhs, const TpEnum& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return (lhs = static_cast<TpIntegral>(static_cast<underlying_type>(lhs) &
            static_cast<underlying_type>(rhs)));
}


template<typename T>
std::enable_if_t<
        enum_bitwise_operators<T>::enable,
        T&
>
operator ^=(T& lhs, const T& rhs)
{
    typedef std::underlying_type_t<T> underlying_type;
    return (lhs = static_cast<T>(static_cast<underlying_type>(lhs) ^
            static_cast<underlying_type>(rhs)));
}


template<typename TpEnum, typename TpIntegral>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_rhs_integrals,
        TpEnum&
>
operator ^=(TpEnum& lhs, const TpIntegral& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return (lhs = static_cast<TpEnum>(static_cast<underlying_type>(lhs) ^
            static_cast<underlying_type>(rhs)));
}


template<typename TpIntegral, typename TpEnum>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_lhs_integrals,
        TpIntegral&
>
operator ^=(TpIntegral& lhs, const TpEnum& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return (lhs = static_cast<TpIntegral>(static_cast<underlying_type>(lhs) ^
            static_cast<underlying_type>(rhs)));
}


template<typename T>
std::enable_if_t<
        enum_bitwise_operators<T>::enable,
        T&
>
operator <<=(T& lhs, const T& rhs)
{
    typedef std::underlying_type_t<T> underlying_type;
    return (lhs = static_cast<T>(static_cast<underlying_type>(lhs) << static_cast<underlying_type>(rhs)));
}


template<typename TpEnum, typename TpIntegral>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_rhs_integrals,
        TpEnum&
>
operator <<=(TpEnum& lhs, const TpIntegral& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return (lhs = static_cast<TpEnum>(static_cast<underlying_type>(lhs) <<
            static_cast<underlying_type>(rhs)));
}


template<typename TpIntegral, typename TpEnum>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_lhs_integrals,
        TpIntegral&
>
operator <<=(TpIntegral& lhs, const TpEnum& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return (lhs = static_cast<TpIntegral>(static_cast<underlying_type>(lhs) <<
            static_cast<underlying_type>(rhs)));
}


template<typename T>
std::enable_if_t<
        enum_bitwise_operators<T>::enable,
        T&
>
operator >>=(T& lhs, const T& rhs)
{
    typedef std::underlying_type_t<T> underlying_type;
    return (lhs = static_cast<T>(static_cast<underlying_type>(lhs) >> static_cast<underlying_type>(rhs)));
}


template<typename TpEnum, typename TpIntegral>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_rhs_integrals,
        TpEnum&
>
operator >>=(TpEnum& lhs, const TpIntegral& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return (lhs = static_cast<TpEnum>(static_cast<underlying_type>(lhs) >>
            static_cast<underlying_type>(rhs)));
}


template<typename TpIntegral, typename TpEnum>
std::enable_if_t<
        enum_bitwise_operators<TpEnum>::enable_with_lhs_integrals,
        TpIntegral&
>
operator >>=(TpIntegral& lhs, const TpEnum& rhs)
{
    typedef std::underlying_type_t<TpEnum> underlying_type;
    return (lhs = static_cast<TpIntegral>(static_cast<underlying_type>(lhs) >>
            static_cast<underlying_type>(rhs)));
}


template<typename TpChar, typename TpCharTraits, typename TpBitmask>
std::enable_if_t<
        enum_bitwise_operators<TpBitmask>::enable_basic_ostream_stdout,
        std::basic_ostream<TpChar>&
>
operator <<(
        std::basic_ostream<TpChar, TpCharTraits>& os,
        const TpBitmask& rhs
)
{
    typedef std::underlying_type_t<TpBitmask> underlying_type;
    std::uint32_t n_bits = sizeof(TpBitmask) * 8U;
    
    for (std::uint32_t i = 0U, cont = 0U; i < n_bits; i++)
    {
        os << (static_cast<underlying_type>(rhs) >> (n_bits - 1U - i) & 1U);
        
        if (cont == 7U && i + 1U < n_bits)
        {
            os << " ";
            cont = 0U;
        }
        
        else
            cont++;
    }
    
    return os;
}
/** @endcond */


}
}


using kcs::lowlevel::operator |;
using kcs::lowlevel::operator &;
using kcs::lowlevel::operator ^;
using kcs::lowlevel::operator <<;
using kcs::lowlevel::operator >>;
using kcs::lowlevel::operator ~;
using kcs::lowlevel::operator |=;
using kcs::lowlevel::operator &=;
using kcs::lowlevel::operator ^=;
using kcs::lowlevel::operator <<=;
using kcs::lowlevel::operator >>=;


#endif
