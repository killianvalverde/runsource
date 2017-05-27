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
 * @file        kcs/argparse/basic_arg_key.hpp
 * @brief       basic_arg_key class header.
 * @author      Killian
 * @date        2017/01/05 - 20:57
 */

#ifndef KCS_ARGPARSE_BASIC_ARG_KEY_HPP
#define KCS_ARGPARSE_BASIC_ARG_KEY_HPP

#include <string>


namespace kcs {
namespace argparse {


/**
 * @brief       Class that represents a key for an argument.
 */
template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAlloc = std::allocator<int>
>
class basic_arg_key
{
public:
    /** Fundamental type that represents character type used in the class. */
    using char_type = TpChar;
    
    /** class that defines essential properties of the characters used by basic_string objects */
    using char_traits_type = TpCharTraits;
    
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAlloc::template rebind<T>::other;
    
    /** Class that represent sequences of characters. */
    using string_type = std::basic_string<TpChar, TpCharTraits, allocator_type<char_type>>;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       key : Key string to represent.
     * @param       is_prefix_long : Allows knowing wether the key prefix is a long prefix.
     */
    template<typename TpString_>
    basic_arg_key(TpString_&& key, bool is_prefix_long)
            : key_(std::forward<TpString_>(key))
            , is_prefix_long_(is_prefix_long)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_arg_key(const basic_arg_key& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_arg_key(basic_arg_key&& rhs) noexcept
            : key_(std::move(rhs.key_))
            , is_prefix_long_(rhs.is_prefix_long_)
    {
        rhs.is_prefix_long_ = false;
    }
    
    /**
     * @brief       Destructor.
     */
    virtual ~basic_arg_key() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_arg_key& operator =(const basic_arg_key& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_arg_key& operator =(basic_arg_key&& rhs) noexcept
    {
        if (this != &rhs)
        {
            key_ = std::move(rhs.key_);
            std::swap(is_prefix_long_, rhs.is_prefix_long_);
        }
        
        return *this;
    }
    
    /**
     * @brief       Allows knowing wether two objects are equal.
     * @param       rhs : Object to compare.
     * @return      If the objets are equal true is returned, otherwise false is returned.
     */
    inline bool operator ==(const basic_arg_key& rhs) const noexcept
    {
        return (key_ == rhs.key_);
    }
    
    /**
     * @brief       Allows knowing wether two objects are equal.
     * @param       key : String to compare.
     * @return      If the objets are equal true is returned, otherwise false is returned.
     */
    inline bool operator ==(const string_type& key) const noexcept
    {
        return (key_ == key);
    }
    
    /**
     * @brief       Allows knowing wether two objects are different.
     * @param       rhs : Object to compare.
     * @return      If the objets are different true is returned, otherwise false is returned.
     */
    inline bool operator !=(const basic_arg_key& rhs) const noexcept
    {
        return (key_ != rhs.key_);
    }
    
    /**
     * @brief       Allows knowing wether two objects are different.
     * @param       key : String to compare.
     * @return      If the objets are different true is returned, otherwise false is returned.
     */
    inline bool operator !=(const string_type& key) const noexcept
    {
        return (key_ != key);
    }
    
    /**
     * @brief       Allows implicit conversion to string type.
     * @return      A const reference to the string equivalent of the object.
     */
    inline operator const string_type&() const noexcept
    {
        return key_;
    }
    
    /**
     * @brief       Get the key in a character string.
     * @return      The key in a character stirng.
     */
    inline const string_type& get_key() const noexcept
    {
        return key_;
    }
    
    /**
     * @brief       Allows knowing wether the key prefix is a long prefix.
     * @return      A value that allows knowing wether the key prefix is a long prefix.
     */
    inline bool is_prefix_long() const noexcept
    {
        return is_prefix_long_;
    }

private:
    /** The character string that represents the key. */
    string_type key_;
    
    /** The value that allows knowing wether the key prefix is a long prefix. */
    bool is_prefix_long_;
};


/**
 * @brief       Allows to use an objects to print in standard output the value that constains the
 *              key.
 * @param       os : The object who call the function.
 * @param       key : The key to print in standard output.
 * @return      The object who call the function.
 * @throw       std::basic_ostream::failure : It throws an exception if the resulting error state
 *              flag of the 'std::basic_ostream' object is not goodbit and member exceptions was set
 *              to throw for that state.
 */
template<typename TpChar, typename TpCharTraits>
std::basic_ostream<TpChar, TpCharTraits>& operator <<(
        std::basic_ostream<TpChar, TpCharTraits>& os,
        const basic_arg_key<TpChar>& key
)
{
    os << key.get_key();
    
    return os;
}


/** Class that represents a key for an argument with 8 bits characters. */
using arg_key = basic_arg_key<char>;


/** Class that represents a key for an argument with 16 bits characters. */
using warg_key = basic_arg_key<wchar_t>;


}
}


#endif
