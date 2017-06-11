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
 * @file        kcs/type_casting/type_cast.hpp
 * @brief       type_cast main header.
 * @author      Killian
 * @date        2016/08/19 - 15:29
 */

#ifndef KCS_TYPE_CASTING_TYPE_CAST_HPP
#define KCS_TYPE_CASTING_TYPE_CAST_HPP

#include "bad_type_cast.hpp"
#include "basic_string_to_type.hpp"
#include "c_string_to_basic_string.hpp"
#include "c_string_to_floating_point.hpp"
#include "c_string_to_integral_signed.hpp"
#include "c_string_to_integral_unsigned.hpp"
#include "c_string_to_path.hpp"


namespace kcs {
namespace type_casting {


/**
 * @brief       Convert a C string into a integral unsigned type.
 * @param       arg : The value to convert.
 * @param       default_value : The value returned if the conversion fails.
 * @return      If function was successful the C string converted into a integral unsigned type is
 *              returned, otherwise the default value specified is returned.
 */
template<typename TpTarget, typename TpSource>
TpTarget type_cast(const TpSource& arg, TpTarget&& default_value)
{
    TpTarget target;
    
    if (try_type_cast(arg, target))
    {
        return target;
    }
    
    return default_value;
}


}
}


#endif
