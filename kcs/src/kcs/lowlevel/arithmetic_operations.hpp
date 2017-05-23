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
 * @file        kcs/lowlevel/arithmetic_operations.hpp
 * @brief       arithmetic_operations functions header.
 * @author      Killian
 * @date        2017/01/28 - 23:50
 */

#ifndef KCS_LOWLEVEL_ARITHMETIC_OPERATIONS_HPP
#define KCS_LOWLEVEL_ARITHMETIC_OPERATIONS_HPP

#include <limits>
#include <stdexcept>


namespace kcs {
namespace lowlevel {


/**
 * @brief       Fonction that allows to do an addition with overflow detection.
 * @param       target : Integral to increment.
 * @param       value : Value to add to the integral.
 * @throw       std::overflow_error : If the addition will produce an overflow an exception is
 *              thrown.
 */
template<typename TpTarget, typename TpValue>
inline TpTarget addo(
        TpTarget target,
        const TpValue& value
)
{
    constexpr TpTarget max_trg = std::numeric_limits<TpTarget>::max();
    
    if (target > max_trg - static_cast<TpTarget>(value))
    {
        throw std::overflow_error("std::overflow_error : overflow range error");
    }
    
    return target + static_cast<TpTarget>(value);
}


/**
 * @brief       Fonction that allows to do an addition with overflow detection.
 * @param       target : Integral to increment.
 * @param       values : Values to add to the integral.
 * @throw       std::overflow_error : If the addition will produce an overflow an exception is
 *              thrown.
 */
template<typename TpTarget, typename... TpValues>
inline TpTarget addol(
        TpTarget target,
        const TpValues&... values
)
{
    constexpr TpTarget max_trg = std::numeric_limits<TpTarget>::max();
    
    int dummy[sizeof...(TpValues)] = { (
        (target > max_trg - static_cast<TpTarget>(values)) ?
                throw std::overflow_error("std::overflow_error : overflow range error") :
                target += static_cast<TpTarget>(values), 0
    )... };
    
    return target;
}


/**
 * @brief       Fonction that allows to do an addition with overflow detection.
 * @param       target : Integral to increment.
 * @param       values : Value to add to the integral.
 * @return      If the function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpValue>
inline bool try_addo(
        TpTarget& target,
        const TpValue& values
) noexcept
{
    constexpr TpTarget max_trg = std::numeric_limits<TpTarget>::max();
    bool scs = true;
    
    if (target > max_trg - static_cast<TpTarget>(values))
    {
        scs = false;
    }
    
    target += static_cast<TpTarget>(values);
    
    return scs;
}


/**
 * @brief       Fonction that allows to do an addition with overflow detection.
 * @param       target : Integral to increment.
 * @param       values : Values to add to the integral.
 * @return      If the function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename... TpValues>
inline bool try_addol(
        TpTarget& target,
        const TpValues& ... values
) noexcept
{
    constexpr TpTarget max_trg = std::numeric_limits<TpTarget>::max();
    bool scs = true;
    
    int dummy[sizeof...(TpValues)] = { (
        ((target > max_trg - static_cast<TpTarget>(values)) ?
                scs = false : 0),
        target += static_cast<TpTarget>(values), 0
    )... };
    
    return scs;
}


/**
 * @brief       Fonction that allows to do an addition with overflow detection and in the overflow
 *              case the maximum value of the type will be set to target.
 * @param       target : Integral to increment.
 * @param       value : Values to add to the integral.
 */
template<typename TpTarget, typename TpValue>
inline TpTarget addm(
        TpTarget target,
        const TpValue& value
)
{
    constexpr TpTarget max_trg = std::numeric_limits<TpTarget>::max();
    
    if (target > max_trg - static_cast<TpTarget>(value))
    {
        target = max_trg;
    }
    else
    {
        target += static_cast<TpTarget>(value);
    }
    
    return target;
}


/**
 * @brief       Fonction that allows to do an addition with overflow detection and in the overflow
 *              case the maximum value of the type will be set to target.
 * @param       target : Integral to increment.
 * @param       values : Values to add to the integral.
 */
template<typename TpTarget, typename... TpValues>
inline TpTarget addml(
        TpTarget target,
        const TpValues& ... values
)
{
    constexpr TpTarget max_trg = std::numeric_limits<TpTarget>::max();
    
    int dummy[sizeof...(TpValues)] = { (
        (target > max_trg - static_cast<TpTarget>(values)) ?
                target = max_trg :
                target += static_cast<TpTarget>(values), 0
    )... };
    
    return target;
}


/**
 * @brief       Fonction that allows to do an addition with overflow detection and in the overflow
 *              case the maximum value of the type will be set to target.
 * @param       target : Integral to increment.
 * @param       value : Values to add to the integral.
 * @return      If the function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename TpValue>
inline bool try_addm(
        TpTarget& target,
        const TpValue& value
) noexcept
{
    constexpr TpTarget max_trg = std::numeric_limits<TpTarget>::max();
    bool scs = true;
    
    if (target > max_trg - static_cast<TpTarget>(value))
    {
        target = max_trg;
        scs = false;
    }
    else
    {
        target += static_cast<TpTarget>(value);
    }
    
    return scs;
}


/**
 * @brief       Fonction that allows to do an addition with overflow detection and in the overflow
 *              case the maximum value of the type will be set to target.
 * @param       target : Integral to increment.
 * @param       values : Values to add to the integral.
 * @return      If the function was successful true is returned, otherwise false is returned.
 */
template<typename TpTarget, typename... TpValues>
inline bool try_addml(
        TpTarget& target,
        const TpValues& ... values
) noexcept
{
    constexpr TpTarget max_trg = std::numeric_limits<TpTarget>::max();
    bool scs = true;
    
    int dummy[sizeof...(TpValues)] = { (
        (target > max_trg - static_cast<TpTarget>(values)) ?
                target = max_trg, scs = false :
                target += static_cast<TpTarget>(values), 0
    )... };
    
    return scs;
}
    
    
}
}


#endif
