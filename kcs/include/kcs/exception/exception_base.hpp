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
 * @file        kcs/exception/exception_base.hpp
 * @brief       exception_base class header.
 * @author      Killian
 * @date        2016/08/19 11:39
 */

#ifndef KCS_EXCEPTION_EXCEPTION_BASE_HPP
#define KCS_EXCEPTION_EXCEPTION_BASE_HPP

#include <exception>
#include <string>


namespace kcs {
namespace exception {


/**
 * @brief       Base class for standard kcs exceptions. All objects thrown by components kcs are
 *              derived from this class. Therefore, all kcs exceptions can be caught by catching
 *              this type by reference.
 */
class exception_base : public std::exception
{
public:
    /**
     * @brief       Constructor with parameters.
     * @param       str : Message to return by the exception object.
     */
    inline explicit exception_base(const char* const str = nullptr) noexcept
            : message_(str)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    inline explicit exception_base(const exception_base& rhs) noexcept = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    inline explicit exception_base(exception_base&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    virtual ~exception_base() noexcept = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    exception_base& operator =(const exception_base& rhs) noexcept = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    exception_base& operator =(exception_base&& rhs) noexcept = default;
    
    /**
     * @brief       Get the message of the exception.
     * @return      The exception message.
     */
    inline virtual const char* what() const noexcept
    {
        return message_;
    }

protected:
    /** Message of the exception. */
    const char* message_;
};


}
}


#endif
