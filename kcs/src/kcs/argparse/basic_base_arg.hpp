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
 * @file        kcs/argparse/basic_base_arg.hpp
 * @brief       basic_base_arg class header.
 * @author      Killian
 * @date        2015/12/28 - 23:17
 */

#ifndef KCS_ARGPARSE_BASIC_COMMAND_ARG_HPP
#define KCS_ARGPARSE_BASIC_COMMAND_ARG_HPP

#include "../iostream/basic_ostream.hpp"
#include "../lowlevel/arithmetic_operations.hpp"
#include "../system/input_output.hpp"
#include "arg_error_flags.hpp"
#include "arg_flags.hpp"
#include "ibasic_help_text.hpp"


namespace kcs {
namespace argparse {


/**
 * @brief       Class that represents the base of the arguments hierarchy.
 */
template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAlloc = std::allocator<int>
>
class basic_base_arg : public ibasic_help_text<TpChar, TpCharTraits, TpAlloc>
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
    using string_type = std::basic_string<TpChar, TpCharTraits, allocator_type<char_type>>;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       description : Argument description content.
     * @param       error_id : The id used to reference an arguments during the error display.
     * @param       flags : Flags that dictates the argument behavior.
     */
    template<typename TpString1_, typename TpString2_>
    basic_base_arg(
            TpString1_&& description,
            TpString2_&& error_id,
            arg_flags flags
    )
            : description_(std::forward<TpString1_>(description))
            , error_id_(std::forward<TpString2_>(error_id))
            , parsed_(false)
            , found_(false)
            , flags_(flags)
            , error_flags_(arg_error_flags::NIL)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_base_arg(const basic_base_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_base_arg(basic_base_arg&& rhs) noexcept
            : description_(std::move(rhs.description_))
            , error_id_(std::move(rhs.error_id_))
            , parsed_(std::move(rhs.parsed_))
            , found_(std::move(rhs.found_))
            , flags_(std::move(rhs.flags_))
            , error_flags_(std::move(rhs.error_flags_))
    {
        rhs.parsed_ = false;
        rhs.found_ = false;
    }
    
    /**
     * @brief       Destructor.
     */
    virtual ~basic_base_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_base_arg& operator =(const basic_base_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_base_arg& operator =(basic_base_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            description_ = std::move(rhs.description_);
            error_id_ = std::move(rhs.error_id_);
            std::swap(parsed_, rhs.parsed_);
            std::swap(found_, rhs.found_);
            flags_ = std::move(rhs.flags_);
            error_flags_ = std::move(rhs.error_flags_);
        }
        
        return *this;
    }
    
    /**
     * @brief       Get a value that allows knowing wether the argument has been parsed.
     * @return      A value that allows knowing wether the argument has been parsed.
     */
    inline bool was_parsed() const noexcept
    {
        return parsed_;
    }
    
    /**
     * @brief       Get a value that specifies whether the argument has been found in the program
     *              call.
     * @return      The value that specifies whether the argument has been found in the program
     *              call.
     */
    inline bool was_found() const noexcept
    {
        return found_;
    }
    
    /**
     * @brief       Set a value that specifies whether the argument has been parsed.
     * @param       parsed : The value that specifies whether the argument has been parsed.
     */
    inline void set_parsed(bool parsed) noexcept
    {
        parsed_ = parsed;
    }
    
    /**
     * @brief       Set a value that specifies whether the argument has been found in the program
     *              call.
     * @param       found : The value that specifies whether the argument has been found in the
     *              program call.
     */
    void set_found(bool found) noexcept
    {
        if (found && found_ && flags_.flag_is_raised(arg_flags::APPEAR_JUST_ONCE))
        {
            error_flags_.raise_flag(arg_error_flags::APPEAR_JUST_ONCE_ERROR);
        }
        else
        {
            found_ = found;
            error_flags_.erase_flag(arg_error_flags::APPEAR_JUST_ONCE_ERROR);
        }
    }
    
    /**
     * @brief       Allows knowing wether the argument description is empty.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    inline bool description_is_empty() const noexcept
    {
        return description_.empty();
    }
    
    /**
     * @brief       Allows knowing wether the argument error id is empty.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    inline bool error_id_is_empty() const noexcept
    {
        return error_id_.empty();
    }
    
    /**
     * @brief       Allows knowing wether an argument flag is set.
     * @param       flag : The flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool flag_is_raised(arg_flags flag) const noexcept
    {
        return flags_.flag_is_raised(flag);
    }
    
    /**
     * @brief       Erase a argument flag.
     * @param       flag : Flag to erase.
     */
    inline void erase_flag(arg_flags flag) noexcept
    {
        flags_.erase_flag(flag);
    }
    
    /**
     * @brief       Allows knowing wether an argument flag is set.
     * @param       flag : The flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool error_flag_is_raised(arg_error_flags flag) const noexcept
    {
        return error_flags_.flag_is_raised(flag);
    }
    
    /**
     * @brief       Raise a argument error flag.
     * @param       flag : Error flag to raise.
     */
    inline void raise_error_flag(arg_error_flags flag) noexcept
    {
        error_flags_.raise_flag(flag);
    }
    
    /**
     * @brief       Erase a argument error flag.
     * @param       flag : Error flag to erase.
     */
    inline void erase_error_flag(arg_error_flags flag) noexcept
    {
        error_flags_.erase_flag(flag);
    }
    
    /**
     * @brief       Erase all argument flags.
     */
    inline void clear_error_flags() noexcept
    {
        error_flags_.clear();
    }
    
    /**
     * @brief       Set errors flags if required.
     */
    virtual void update_error_flags() noexcept
    {
        if (flags_.flag_is_raised(arg_flags::ALLWAYS_REQUIRED) && !found_ && parsed_)
        {
            error_flags_.raise_flag(arg_error_flags::ALLWAYS_REQUIRED_ERROR);
        }
        else
        {
            error_flags_.erase_flag(arg_error_flags::ALLWAYS_REQUIRED_ERROR);
        }
    }
    
    /**
     * @brief       Allows knowing wether there are errors flags.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool there_are_errors() const noexcept
    {
        return !error_flags_.is_empty();
    }
    
    /**
     * @brief       Print the argument information for help menu.
     * @param       max_line_length : The maximum description length that will be printed in a
     *              single line.
     * @param       newline_indentation : The indentation used when a newline is found in a
     *              description.
     * @param       current_line_length : The length of the current line.
     */
    void print_help_text(
            std::size_t max_line_length,
            std::size_t newline_indentation,
            std::size_t current_line_length
    ) const override
    {
        if (description_.empty())
        {
            return;
        }
    
        auto& os = kcs::iostream::get_cout<char_type>();
        std::size_t length_to_next;
    
        for (auto it = description_.cbegin(); it != description_.cend(); it++)
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
                for (; aux_it != description_.cend() && *aux_it != ' '; aux_it++)
                {
                    length_to_next++;
                }
            
                kcs::lowlevel::try_addm(&length_to_next, current_line_length);
                if (length_to_next > max_line_length)
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
    
    /**
     * @brief       Print argument errors in standard output.
     * @param       program_name : The name of the program.
     * @param       colors_enable : If it is true, colors will be used during the print.
     */
    virtual void print_errors(const string_type& program_name, bool colors_enable) const
    {
        auto& os = kcs::iostream::get_cout<char_type>();
        
        if (error_flag_is_raised(arg_error_flags::ALLWAYS_REQUIRED_ERROR))
        {
            os << program_name << ": ";
            if (!get_error_id().empty())
            {
                if (colors_enable)
                {
                    kcs::system::set_ostream_text_attribute(
                            os, kcs::system::text_attribute::LIGHT_RED);
                    os << get_error_id() << ": ";
                    kcs::system::set_ostream_text_attribute(
                            os, kcs::system::text_attribute::DEFAULT);
                }
                else
                {
                    os << get_error_id() << ": ";
                }
            }
            os << "Option is allways required" << std::endl;
        }
        if (error_flag_is_raised(arg_error_flags::APPEAR_JUST_ONCE_ERROR))
        {
            os << program_name << ": ";
            if (!get_error_id().empty())
            {
                if (colors_enable)
                {
                    kcs::system::set_ostream_text_attribute(
                            os, kcs::system::text_attribute::LIGHT_RED);
                    os << get_error_id() << ": ";
                    kcs::system::set_ostream_text_attribute(
                            os, kcs::system::text_attribute::DEFAULT);
                }
                else
                {
                    os << get_error_id() << ": ";
                }
            }
            os << "Option has appeared more than once" << std::endl;
        }
    }
    
    /**
     * @brief       Allows throwing an error to the user using the arguent and its syntax. This
     *              method has been made for particular occasions in which an argument error can't
     *              be detected by the argument parser.
     * @param       error_message : The message to print in the standard output.
     * @param       program_name : The name of the program.
     * @param       colors_enable : If it is true, colors will be used during the print.
     */
    void print_error_message(
            const string_type& error_message,
            const string_type& program_name,
            bool colors_enable
    ) const
    {
        auto& os = kcs::iostream::get_cout<char_type>();
    
        os << program_name << ": ";
        if (!get_error_id().empty())
        {
            if (colors_enable)
            {
                kcs::system::set_ostream_text_attribute(
                        os, kcs::system::text_attribute::LIGHT_RED);
                os << get_error_id() << ": ";
                kcs::system::set_ostream_text_attribute(
                        os, kcs::system::text_attribute::DEFAULT);
            }
            else
            {
                os << get_error_id() << ": ";
            }
        }
        os << error_message << std::endl;
    }
    
    /**
     * @brief       Equal operator.
     * @param       rhs : The object to compare.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool operator ==(const basic_base_arg& rhs) const noexcept
    {
        return description_ == rhs.description_ &&
               error_id_ == rhs.error_id_ &&
               parsed_ == rhs.parsed_ &&
               found_ == rhs.found_ &&
               flags_ == rhs.flags_ &&
               error_flags_ == rhs.error_flags_;
    }
    
    /**
     * @brief       Not equal operator.
     * @param       rhs : The object to compare.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool operator !=(const basic_base_arg& rhs) const noexcept
    {
        return !(rhs == *this);
    }
    
    /**
     * @brief       Allows knowing whether an argument has a specified key.
     * @param       key : The key to check.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    virtual bool check_key(const string_type& key) const noexcept /*= 0;*/
    {
        return false;
    }
    
    /**
     * @brief       Get the necessary length to print short arguments keys in standard output.
     * @return      The necessary length to print short arguments keys in standard output.
     */
    virtual std::size_t get_short_keys_length() const noexcept /*= 0;*/
    {
        return 0;
    }
    
    /**
     * @brief       Get the necessary length to print long arguments keys in standard output.
     * @return      The necessary length to print long arguments keys in standard output.
     */
    virtual std::size_t get_long_keys_length() const noexcept /*= 0;*/
    {
        return 0;
    }
    
    /**
     * @brief       Print the argument information for help menu.
     * @param       args_indentation : Indentation used to print arguments help description.
     * @param       max_line_length : The maximum description length that will be printed in a
     *              single line.
     * @param       newline_indentation : The indentation used when a newline is found in a
     *              description.
     * @param       short_id_length : The maximum length of the short keys.
     * @param       long_id_length : The maximum length of the long keys.
     */
    virtual void print_help_text(
            std::size_t args_indentation,
            std::size_t max_line_length,
            std::size_t newline_indentation,
            std::size_t short_id_length,
            std::size_t long_id_length
    ) const /*= 0;*/
    {
    }

protected:
    /**
     * @brief       Get the argument error id.
     * @return      The argument error id.
     */
    inline const string_type& get_error_id() const noexcept
    {
        return error_id_;
    }
    
    /**
     * @brief       Get the arguement flags.
     * @return      The argument flags.
     */
    inline const flags_container_type<arg_flags>& get_flags() const noexcept
    {
        return flags_;
    }
    
    /**
     * @brief       Set the argument error id.
     * @param       error_id : The argument error id.
     */
    template<typename TpString_>
    void set_error_id(TpString_&& error_id)
    {
        error_id_ = std::forward<TpString_>(error_id);
    }

private:
    /** Argument description content. */
    string_type description_;
    
    /** The id used to reference an arguments during the error display. */
    string_type error_id_;
    
    /** Allows knowing wether the argument has been parsed. */
    bool parsed_;
    
    /** Specifies whether the argument has been found in the program call. */
    bool found_;
    
    /** Flags that dictates the argument behavior. */
    flags_container_type<arg_flags> flags_;
    
    /** Error flags that allows knowing wether there are errors. */
    flags_container_type<arg_error_flags> error_flags_;
};


/** Class that represents the base of the arguments hierarchy with 8 bits characters. */
using base_arg = basic_base_arg<char>;


/** Class that represents the base of the arguments hierarchy with 16 bits characters. */
using wbase_arg = basic_base_arg<wchar_t>;
    
    
}
}


#endif
