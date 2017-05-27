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
 * @file        kcs/argparse/basic_foreign_arg.hpp
 * @brief       basic_foreign_arg class header.
 * @author      Killian
 * @date        2016/03/11 - 14:20
 */

#ifndef KCS_ARGPARSE_BASIC_FOREIGN_ARG_HPP
#define KCS_ARGPARSE_BASIC_FOREIGN_ARG_HPP

#include "basic_value_arg.hpp"


namespace kcs {
namespace argparse {


/**
 * @brief       Class that represents arguments without keys.
 */
template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAlloc = std::allocator<int>
>
class basic_foreign_arg : public basic_value_arg<TpChar, TpCharTraits, TpAlloc>
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
    
    /** Class that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents a value for an argument. */
    using arg_value_type = basic_arg_value<TpChar, TpCharTraits, TpAlloc>;
    
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
     * @param       min_values : Minimum number of values for an option.
     * @param       max_values : Maximum number of values for an option.
     * @param       values_types : Collection that has the values types that must have the values.
     * @param       regex_collection : Regular expressions that the values have to match. If the
     *              string is empty all values will match.
     * @param       usage_key : The key used to reference an arguments during the help usage
     *              sentence display.
     * @param       help_key : The key used to reference an arguments during the help display.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpString3_,
            typename TpString4_,
            typename TpArgValueTypesVector_ = vector_type<arg_value_types>,
            typename TpStringVector_ = vector_type<string_type>
    >
    basic_foreign_arg(
            TpString1_&& description,
            TpString2_&& error_id,
            arg_flags flags,
            std::size_t min_values,
            std::size_t max_values,
            TpArgValueTypesVector_&& values_types,
            TpStringVector_&& regex_collection,
            TpString3_&& usage_key,
            TpString4_&& help_key
    )
            : base_arg_type(std::forward<TpString1_>(description),
                            std::forward<TpString2_>(error_id),
                            flags)
            , value_arg_type(description,
                             error_id,
                             flags,
                             min_values,
                             max_values,
                             std::forward<TpArgValueTypesVector_>(values_types),
                             std::forward<TpStringVector_>(regex_collection))
            , usage_key_(std::forward<TpString3_>(usage_key))
            , help_key_(std::forward<TpString4_>(help_key))
    {
        if (base_arg_type::error_id_is_empty() &&
            base_arg_type::flag_is_raised(arg_flags::USE_FIRST_KEY_IF_ERROR_ID_EMPTY))
        {
            base_arg_type::set_error_id(help_key_);
        }
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_foreign_arg(const basic_foreign_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_foreign_arg(basic_foreign_arg&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_foreign_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_foreign_arg& operator =(const basic_foreign_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_foreign_arg& operator =(basic_foreign_arg&& rhs) noexcept = default;
    
    /**
     * @brief       Get the usage key of the argument.
     * @return      The usage key of the argument.
     */
    inline const string_type& get_usage_key() const noexcept
    {
        return usage_key_;
    }
    
    /**
     * @brief       Get the help key of the argument.
     * @return      The help key of the argument.
     */
    inline const string_type& get_help_key() const noexcept
    {
        return help_key_;
    }
    
    /**
     * @brief       Allows knowing whether an foreign argument has a specified key.
     * @param       key : The key to check.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    bool check_key(const string_type& key) const noexcept override
    {
        return usage_key_ == key || help_key_ == key;
    }
    
    /**
     * @brief       Get the necessary length to print the help keys in standard output.
     * @return      The necessary length to print the help keys in standard output.
     */
    inline std::size_t get_short_keys_length() const noexcept override
    {
        return lowlevel::addm(help_key_.length(), 2);
    }
    
    /**
     * @brief       Only used for polymorphic propose.
     * @return      0 is returned always.
     */
    inline std::size_t get_long_keys_length() const noexcept override
    {
        return 0;
    }
    
    /**
     * @brief       Print the argument usage key in standard output for usage sentence.
     */
    inline void print_usage_key() const
    {
        auto& os = kcs::iostream::get_cout<char_type>();
        
        os << " [" << usage_key_ << "]";
        if (value_arg_type::get_max_values() > 1)
        {
            os << "...";
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
        std::size_t current_id_length = kcs::lowlevel::addm(help_key_.length(), 2);
        std::size_t total_id_length = kcs::lowlevel::addm(short_id_length, long_id_length);
        std::size_t i;
    
        for (i = keys_indentation; i > 0; i--)
        {
            os << (char_type)' ';
        }
        
        os << help_key_ << "  ";
        
        if (current_id_length < total_id_length)
        {
            for (i = total_id_length - current_id_length; i > 0; i--)
            {
                os << (char_type)' ';
            }
        }
    
        kcs::lowlevel::try_addm(&keys_indentation,
                                total_id_length);
        kcs::lowlevel::try_addm(&newline_indentation,
                                keys_indentation);
        base_arg_type::print_help_text(max_description_line_length,
                                       newline_indentation,
                                       keys_indentation);
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

private:
    /** The key used to reference an arguments during the help usage sentence display. */
    string_type usage_key_;
    
    /** The key used to reference an arguments during the help display. */
    string_type help_key_;
};


/** Class that represents arguments without keys with 8 bits characters.  */
using foreign_arg = basic_foreign_arg<char>;


/** Class that represents arguments without keys with 16 bits characters.  */
using wforeign_arg = basic_foreign_arg<wchar_t>;


}
}


#endif
