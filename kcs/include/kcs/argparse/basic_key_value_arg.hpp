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
 * @file        kcs/argparse/basic_key_value_arg.hpp
 * @brief       basic_key_value_arg class header.
 * @author      Killian
 * @date        2016/03/11 - 08:12
 */

#ifndef KCS_ARGPARSE_BASIC_KEY_VALUE_ARG_HPP
#define KCS_ARGPARSE_BASIC_KEY_VALUE_ARG_HPP

#include "basic_key_arg.hpp"
#include "basic_value_arg.hpp"


namespace kcs {
namespace argparse {


/**
 * @brief       Class that represents arguments that have keys and values.
 */
template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAlloc = std::allocator<int>
>
class basic_key_value_arg
        : public basic_key_arg<TpChar, TpCharTraits, TpAlloc>
        , public basic_value_arg<TpChar, TpCharTraits, TpAlloc>
{
public:
    /** Fundamental type that represents character type used in the class. */
    using char_type = TpChar;
    
    /** class that defines essential properties of the characters used by basic_string objects */
    using char_traits_type = TpCharTraits;
    
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAlloc::template rebind<T>::other;
    
    /** Class that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents a value for an argument. */
    using arg_value_type = basic_arg_value<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents a key for an argument. */
    using arg_key_type = basic_arg_key<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents flags container */
    template<typename T>
    using flags_container_type = kcs::lowlevel::flags_container<T>;
    
    /** Class that represents sequences of characters. */
    using string_type = std::basic_string<TpChar, TpCharTraits, allocator_type<char_type>>;
    
    /** Class that represents a sequence container. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       description : Argument description content.
     * @param       error_id : The id used to reference an arguments during the error display.
     * @param       flags : Flags that dictates the argument behavior.
     * @param       keys : Argument keys collection.
     * @param       min_values : Minimum number of values for an option.
     * @param       max_values : Maximum number of values for an option.
     * @param       values_types : Collection that has the values types that must have the values.
     * @param       regex_collection : Regular expressions that the values have to match. If the
     *              vector is empty all values will match.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpArgKeyVector_ = vector_type<string_type>,
            typename TpStringVector_ = vector_type<string_type>,
            typename TpArgValueTypesVector_ = vector_type<arg_value_types>
    >
    basic_key_value_arg(
            TpString1_&& description,
            TpString2_&& error_id,
            arg_flags flags,
            TpArgKeyVector_&& keys,
            std::size_t min_values,
            std::size_t max_values,
            TpArgValueTypesVector_&& values_types,
            TpStringVector_&& regex_collection
    )
            : base_arg_type(std::forward<TpString1_>(description),
                            std::forward<TpString2_>(error_id),
                            flags)
            , key_arg_type(description,
                           error_id,
                           flags,
                           std::forward<TpArgKeyVector_>(keys))
            , value_arg_type(description,
                             error_id,
                             flags,
                             min_values,
                             max_values,
                             std::forward<TpArgValueTypesVector_>(values_types),
                             std::forward<TpStringVector_>(regex_collection))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_key_value_arg(const basic_key_value_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_key_value_arg(basic_key_value_arg&& rhs) = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_key_value_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_key_value_arg& operator =(const basic_key_value_arg& rhs)
    {
        if (this != &rhs)
        {
            base_arg_type::operator =(rhs);
            key_arg_type::local_assignment(rhs);
            value_arg_type::local_assignment(rhs);
        }
        
        return *this;
    }
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_key_value_arg& operator =(basic_key_value_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            base_arg_type::operator =(std::move(rhs));
            key_arg_type::local_assignment(std::move(rhs));
            value_arg_type::local_assignment(std::move(rhs));
        }
        
        return *this;
    }
    
    /**
     * @brief       Print argument errors in standard output.
     * @param       program_name : The name of the program.
     * @param       colors_enable : If it is true, colors will be used during the print.
     */
    void print_errors(const string_type& program_name, bool colors_enable) const override
    {
        value_arg_type::print_errors(program_name, colors_enable);
    }
};


/** Class that represents arguments that have keys and values with 8 bits characters. */
using key_value_arg = basic_key_value_arg<char>;


/** Class that represents arguments that have keys and values with 16 bits characters. */
using wkey_value_arg = basic_key_value_arg<wchar_t>;


}
}


#endif
