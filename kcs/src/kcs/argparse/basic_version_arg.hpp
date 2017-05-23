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
 * @file       kcs/argparse/basic_version_arg.hpp
 * @brief      basic_version_arg class header.
 * @author     Killian
 * @date       2015/12/30 - 20:50
 */

#ifndef KCS_ARGPARSE_BASIC_VERSION_ARG_HPP
#define KCS_ARGPARSE_BASIC_VERSION_ARG_HPP

#include "basic_key_arg.hpp"


namespace kcs {
namespace argparse {


/**
 * @brief       Class that represents the option to get the version information.
 */
template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAlloc = std::allocator<int>
>
class basic_version_arg : public basic_key_arg<TpChar, TpCharTraits, TpAlloc>
{
public:
    /** Fundamental type that represents character type used in the class. */
    using char_type = TpChar;
    
    /** class that defines essential properties of the characters used by basic_string objects */
    using char_traits_types = TpCharTraits;
    
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAlloc::template rebind<T>::other;
    
    /** Class that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents a key for an argument. */
    using arg_key_type = basic_arg_key<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents flags container */
    template<typename T>
    using flags_container_type = kcs::lowlevel::flags_container<T>;
    
    /** Class that represent sequences of characters. */
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
     * @param       version_information : Argument version information content.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpString3_,
            typename TpArgKeyVector_ = vector_type<string_type>
    >
    basic_version_arg(
            TpString1_&& description,
            TpString2_&& error_id,
            arg_flags flags,
            TpArgKeyVector_&& keys,
            TpString3_&& version_information
    )
            : base_arg_type(std::forward<TpString1_>(description),
                            std::forward<TpString2_>(error_id),
                            flags)
            , key_arg_type(description,
                           error_id,
                           flags,
                           std::forward<TpArgKeyVector_>(keys))
            , version_information(std::forward<TpString3_>(version_information))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_version_arg(const basic_version_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_version_arg(basic_version_arg&& rhs) = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_version_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_version_arg& operator =(const basic_version_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_version_arg& operator =(basic_version_arg&& rhs) = default;
    
    /**
     * @brief       Print version information in standard output.
     */
    inline void print_version_information() const
    {
        kcs::iostream::get_cout<TpChar>() << version_information;
    }

private:
    /** Argument version information content. */
    string_type version_information;
};
    
    
}
}


#endif
