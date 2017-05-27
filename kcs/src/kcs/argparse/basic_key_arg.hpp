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
 * @file        kcs/argparse/basic_key_arg.hpp
 * @brief       basic_key_arg class header.
 * @author      Killian
 * @date        2016/03/10 - 16:26
 */

#ifndef KCS_ARGPARSE_BASIC_KEY_ARG_HPP
#define KCS_ARGPARSE_BASIC_KEY_ARG_HPP

#include <vector>

#include "arg_parser_exception.hpp"
#include "basic_arg_key.hpp"
#include "basic_base_arg.hpp"


namespace kcs {
namespace argparse {


/**
 * @brief       Class that represents arguments that have keys.
 */
template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAlloc = std::allocator<int>
>
class basic_key_arg : public virtual basic_base_arg<TpChar, TpCharTraits, TpAlloc>
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
    
    /** Class that represents a key for an argument. */
    using arg_key_type = basic_arg_key<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents flags container. */
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
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpArgKeyVector_ = vector_type<string_type>
    >
    basic_key_arg(
            TpString1_&& description,
            TpString2_&& error_id,
            arg_flags flags,
            TpArgKeyVector_&& keys
    )
            : base_arg_type(std::forward<TpString1_>(description),
                            std::forward<TpString2_>(error_id),
                            flags)
            , keys_vector_(std::forward<TpArgKeyVector_>(keys))
    {
        if (base_arg_type::error_id_is_empty() &&
            base_arg_type::flag_is_raised(arg_flags::USE_FIRST_KEY_IF_ERROR_ID_EMPTY))
        {
            base_arg_type::set_error_id(keys_vector_.front());
        }
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_key_arg(const basic_key_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_key_arg(basic_key_arg&& rhs) = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_key_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_key_arg& operator =(const basic_key_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_key_arg& operator =(basic_key_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            base_arg_type::operator =(std::move(rhs));
            keys_vector_ = std::move(rhs.keys_vector_);
        }
    
        return *this;
    }
    
    /**
     * @brief       Get the arguments keys.
     * @return      The arguments keys.
     */
    inline const vector_type<arg_key_type>& get_keys() const noexcept
    {
        return keys_vector_;
    }
    
    /**
     * @brief       Allows knowing whether an argument has a specified key.
     * @param       key : The key to check.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    bool check_key(const string_type& key) const noexcept override
    {
        return std::find(keys_vector_.begin(), keys_vector_.end(), key) != keys_vector_.end();
    }
    
    /**
     * @brief       Get first argument key.
     * @return      The first specified key is returned.
     * @throw       std::out_of_range : If the argument hasn't any key an exception is thrown.
     */
    const arg_key_type& get_front_key() const
    {
        if (keys_vector_.empty())
        {
            throw std::out_of_range("std::out_of_range: argument hasn't any key");
        }
        
        return keys_vector_.front();
    }
    
    /**
     * @brief       Get the key in the specified position.
     * @param       index : Position of the element.
     * @return      The element specified.
     * @throw       std::out_of_range : If there isn't any key in the specified position an
     *              exception is thrown.
     */
    const arg_key_type& get_key_at(std::size_t index) const
    {
        if (keys_vector_.size() <= index)
        {
            throw std::out_of_range("std::out_of_range: there isn't any key in the specified "
                                            "position");
        }
        
        return keys_vector_.at(index);
    }
    
    /**
     * @brief       Get the necessary length to print short arguments key in standard output.
     * @return      The necessary length to print short arguments key in standard output.
     */
    std::size_t get_short_keys_length() const noexcept override
    {
        std::size_t keys_length = 0;
        
        for (auto& x : keys_vector_)
        {
            if (!x.is_prefix_long())
            {
                kcs::lowlevel::try_addml(&keys_length, x.get_key().length(), 2);
            }
        }
        
        return keys_length;
    }
    
    /**
     * @brief       Get the necessary length to print long arguments key in standard output.
     * @return      The necessary length to print long arguments key in standard output.
     */
    std::size_t get_long_keys_length() const noexcept override
    {
        std::size_t keys_length = 0;
        
        for (auto& x : keys_vector_)
        {
            if (x.is_prefix_long())
            {
                kcs::lowlevel::try_addml(&keys_length, x.get_key().length(), 2);
            }
        }
        
        return keys_length;
    }
    
    /**
     * @brief       Print keys in standard output.
     */
    void print_keys() const
    {
        if (keys_vector_.empty())
        {
            return;
        }
        
        auto& os = kcs::iostream::get_cout<char_type>();
        std::size_t n_args_printed = 0;
        
        for (auto it = keys_vector_.cbegin(); it != keys_vector_.cend(); it++)
        {
            if (!it->is_prefix_long())
            {
                if (n_args_printed > 0)
                {
                    os << ", " << *it;
                }
                else
                {
                    os << *it;
                }
    
                kcs::lowlevel::try_addm(&n_args_printed, 1);
            }
        }
        
        if (n_args_printed < keys_vector_.size() && n_args_printed > 0)
        {
            os << ", ";
        }
        
        n_args_printed = 0;
        for (auto it = keys_vector_.cbegin(); it != keys_vector_.cend(); it++)
        {
            if (it->is_prefix_long())
            {
                if (n_args_printed > 0)
                {
                    os << ", " << *it;
                }
                else
                {
                    os << *it;
                }
    
                kcs::lowlevel::try_addm(&n_args_printed, 1);
            }
        }
    }
    
    /**
     * @brief       Print the argument information for help menu.
     * @param       max_description_line_length : The maximum arguments description length that will
     *              be printed in a single line.
     * @param       newline_indentation : The indentation used when a newline is found.
     * @param       keys_indentation : Indentation used to separate keys help descriptions during
     *              the print.
     * @param       short_id_length : The maximum length of the short keys.
     * @param       long_id_length : The maximum length of the long keys.
     */
    void print_help_text(
            std::size_t max_description_line_length,
            std::size_t newline_indentation,
            std::size_t keys_indentation,
            std::size_t short_id_length,
            std::size_t long_id_length
    ) const override
    {
        if (base_arg_type::description_is_empty())
        {
            return;
        }
        
        auto& os = kcs::iostream::get_cout<char_type>();
        std::size_t current_id_length = 0;
        std::size_t n_args_printed = 0;
        std::size_t i;
    
        for (i = keys_indentation; i > 0; i--)
        {
            os << (char_type)' ';
        }
        
        for (auto it = keys_vector_.cbegin(); it != keys_vector_.cend(); it++)
        {
            if (!it->is_prefix_long())
            {
                if (n_args_printed > 0)
                {
                    os << ", " << *it;
                    kcs::lowlevel::try_addml(&current_id_length, it->get_key().length(), 2);
                }
                else
                {
                    os << *it;
                    kcs::lowlevel::try_addm(&current_id_length, it->get_key().length());
                }
    
                kcs::lowlevel::try_addm(&n_args_printed, 1);
            }
        }
        
        if (n_args_printed < keys_vector_.size() && n_args_printed > 0)
        {
            os << ", ";
            kcs::lowlevel::try_addm(&current_id_length, 2);
        }
        
        if (current_id_length < short_id_length)
        {
            for (i = short_id_length - current_id_length; i > 0; i--)
            {
                os << (char_type)' ';
            }
        }
        
        n_args_printed = 0;
        current_id_length = 0;
        for (auto it = keys_vector_.cbegin(); it != keys_vector_.cend(); it++)
        {
            if (it->is_prefix_long())
            {
                if (n_args_printed > 0)
                {
                    os << ", " << *it;
                    kcs::lowlevel::try_addml(&current_id_length, it->get_key().length(), 2);
                }
                else
                {
                    os << *it;
                    kcs::lowlevel::try_addm(&current_id_length, it->get_key().length());
                }
    
                kcs::lowlevel::try_addm(&n_args_printed, 1);
            }
        }
        
        if (current_id_length < long_id_length)
        {
            for (i = long_id_length - current_id_length; i > 0; i--)
            {
                os << (char_type)' ';
            }
        }
    
        kcs::lowlevel::try_addml(&keys_indentation,
                                 short_id_length,
                                 long_id_length);
        kcs::lowlevel::try_addm(&newline_indentation,
                                keys_indentation);
        base_arg_type::print_help_text(max_description_line_length,
                                       newline_indentation,
                                       keys_indentation);
    }

protected:
    /**
     * @brief       Local copy assignment method.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     * @throw       std::bad_alloc : If the default allocator is used, an exception is thrown if the
     *              function needs to allocate storage and fails.
     */
    basic_key_arg& local_assignment(const basic_key_arg& rhs)
    {
        if (this != &rhs)
        {
            keys_vector_ = rhs.keys_vector_;
        }
        
        return *this;
    }
    
    /**
     * @brief       Local move assignment method.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_key_arg& local_assignment(basic_key_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            keys_vector_ = std::move(rhs.keys_vector_);
        }
        
        return *this;
    }

private:
    /** Argument keys collection. */
    vector_type<arg_key_type> keys_vector_;
};


/** Class that represents arguments that have keys with 8 bits characters. */
using key_arg = basic_key_arg<char>;


/** Class that represents arguments that have keys with 16 bits characters. */
using wkey_arg = basic_key_arg<wchar_t>;


}
}


#endif
