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
 * @file        kcs/type_casting/bad_type_cast.hpp
 * @brief       bad_type_cast main header.
 * @author      Killian
 * @date        2016/08/19 - 15:34
 */

#ifndef KCS_TYPE_CASTING_BAD_TYPE_CAST_HPP
#define KCS_TYPE_CASTING_BAD_TYPE_CAST_HPP

#include "kcs/exception/exception_base.hpp"


namespace kcs {
namespace type_casting {


/**
 * @brief       Class that the type cast functions will use to throw exceptions.
 */
class bad_type_cast : public kcs::exception::exception_base
{
public:
    /**
     * @brief       Constructor with parameters.
     * @param       str : Message to return by the exception object.
     */
    inline bad_type_cast(const char* const str = nullptr) noexcept
            : exception_base(str)
    {
    }
};
    
    
}
}

#endif
