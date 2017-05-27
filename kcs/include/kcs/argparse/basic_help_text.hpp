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
 * @file        kcs/argparse/basic_help_text.hpp
 * @brief       basic_help_text class header.
 * @author      Killian
 * @date        2016/03/12 - 20:35
 */

#ifndef KCS_ARGPARSE_BASIC_HELP_TEXT_HPP
#define KCS_ARGPARSE_BASIC_HELP_TEXT_HPP

#include "../stringutils/stringutils.hpp"
#include "ibasic_help_text.hpp"


namespace kcs {
namespace argparse {


/**
 * @brief       Class that represents a text in the help information.
 */
template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAlloc = std::allocator<int>
>
class basic_help_text : public ibasic_help_text<TpChar, TpCharTraits, TpAlloc>
{
public:
    /** Fundamental type that represents character type used in the class. */
    using char_type = TpChar;
    
    /** class that defines essential properties of the characters used by basic_string objects */
    using char_traits_type = TpCharTraits;
    
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAlloc::template rebind<T>::other;
    
    /** Class that represents flags container */
    template<typename T>
    using flags_container_type = kcs::lowlevel::flags_container<T>;
    
    /** Class that represents sequences of characters. */
    using string_type = std::basic_string<char_type, char_traits_type, allocator_type<char_type>>;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       description : Argument description content.
     */
    template<
            typename TpString_,
            typename = std::enable_if_t<
                    !std::is_base_of<basic_help_text, std::decay_t<TpString_>>::value
            >
    >
    basic_help_text(TpString_&& description)
            : text_(std::forward<TpString_>(description))
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_help_text(const basic_help_text& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_help_text(basic_help_text&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_help_text() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_help_text& operator =(const basic_help_text& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_help_text& operator =(basic_help_text&& rhs) noexcept = default;
    
    
    /**
     * @brief       Print the argument information for help menu.
     * @param       max_description_line_length : The maximum arguments description length that will
     *              be printed in a single line.
     * @param       newline_indentation : The indentation used when a newline is found.
     * @param       current_line_length : The length of the current line.
     */
    void print_help_text(
            std::size_t max_description_line_length,
            std::size_t newline_indentation,
            std::size_t current_line_length
    ) const override
    {
        if (text_.empty())
        {
            return;
        }
    
        auto& os = kcs::iostream::get_cout<char_type>();
        std::size_t length_to_next;
        
        for (auto it = text_.cbegin(); it != text_.cend(); it++)
        {
            if (*it == (char_type)'\n')
            {
                current_line_length = newline_indentation;
                os << std::endl;
                for (std::size_t i = 0; i < newline_indentation; i++)
                {
                    os << (char_type)' ';
                }
            }
            else if (*it == (char_type)' ')
            {
                length_to_next = 1;
    
                auto aux_it = it;
                aux_it++;
                for (; aux_it != text_.cend() && *aux_it != ' '; aux_it++)
                {
                    length_to_next++;
                }
            
                kcs::lowlevel::try_addm(&length_to_next, current_line_length);
                if (length_to_next > max_description_line_length)
                {
                    current_line_length = newline_indentation;
                    os << std::endl;
                    for (std::size_t i = 0; i < newline_indentation; i++)
                    {
                        os << (char_type)' ';
                    }
                }
                else
                {
                    os << (char_type)' ';
                    current_line_length++;
                }
            }
            else
            {
                os << *it;
                current_line_length++;
            }
        }
    
        os << std::endl;
    }

private:
    /** Argument description content. */
    string_type text_;
};


/** Class that represents a text in the help information with 8 bits characters. */
using help_text = basic_help_text<char>;


/** Class that represents a text in the help information with 16 bits */
using whelp_text = basic_help_text<wchar_t>;



}
}


#endif
