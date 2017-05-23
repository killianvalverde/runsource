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
 * @file        kcs/argparse/basic_arg_value.hpp
 * @brief       basic_arg_value class header.
 * @author      Killian
 * @date        2015/12/31 - 03:15
 */

#ifndef KCS_ARGPARSE_BASIC_ARG_VALUE_HPP
#define KCS_ARGPARSE_BASIC_ARG_VALUE_HPP

#include <experimental/filesystem>
#include <regex>
#include <stdexcept>
#include <string>
#include <system_error>

#include "../filesystem/filesystem.hpp"
#include "../lowlevel/flags_container.hpp"
#include "../iostream/basic_ostream.hpp"
#include "../stringutils/stringutils.hpp"
#include "../system/system.hpp"
#include "../type_casting/type_cast.hpp"
#include "arg_error_flags.hpp"
#include "arg_flags.hpp"
#include "arg_value_error_flags.hpp"
#include "arg_value_types.hpp"
#include "basic_value_arg.hpp"


namespace kcs {
namespace argparse {


template<typename TpChar, typename TpCharTraits, typename TpAlloc>
class basic_value_arg;


/**
 * @brief       Class that represents a value for an argument.
 */
template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAlloc = std::allocator<int>
>
class basic_arg_value
{
    /** @relates    kcs::argparser::basic_arg_value */
    friend class kcs::argparse::basic_value_arg<TpChar, TpCharTraits, TpAlloc>;
    
public:
    /** Fundamental type that represents character type used in the class. */
    using char_type = TpChar;
    
    /** class that defines essential properties of the characters used by basic_string objects */
    using char_traits_type = TpCharTraits;
    
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAlloc::template rebind<T>::other;
    
    /** Class that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents flags container */
    template<typename T>
    using flags_container_type = kcs::lowlevel::flags_container<T>;
    
    /** Class that represent sequences of characters. */
    using string_type = std::basic_string<TpChar, TpCharTraits, allocator_type<char_type>>;
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       value : Argument value in string format.
     * @param       type : Type of the value.
     * @param       regex_to_match : Regex that the value has to match. If the string is empty the
     *              value will always match.
     * @param       composite_flags : The value of the composite flags.
     */
    template<
            typename TpString1_,
            typename TpString2_ = string_type,
            typename = std::enable_if_t<
                    !std::is_base_of<basic_arg_value, std::decay_t<TpString1_>>::value
            >
    >
    basic_arg_value(
            TpString1_&& value,
            arg_value_types type = arg_value_types::STRING,
            TpString2_&& regex_to_match = string_type(),
            arg_flags composite_flags = arg_flags::NULL_ARG_FLAGS
    )
            : value_(std::forward<TpString1_>(value))
            , type_(type)
            , regex_to_match_(std::forward<TpString2_>(regex_to_match))
            , error_flags_(arg_value_error_flags::NULL_ARG_VALUE_ERROR_FLAGS)
            , invalid_path_(false)
            , error_message_()
            , composite_flags_(composite_flags)
    {
        namespace stdfs = std::experimental::filesystem;
    
        if ((type_.flag_is_raised(arg_value_types::BOOL) &&
             !is_value_valid<bool>()) ||
            (type_.flag_is_raised(arg_value_types::DOUBLE) &&
             !is_value_valid<double>()) ||
            (type_.flag_is_raised(arg_value_types::FLOAT) &&
             !is_value_valid<float>()) ||
            (type_.flag_is_raised(arg_value_types::INT) &&
             !is_value_valid<int>()) ||
            (type_.flag_is_raised(arg_value_types::INT8) &&
             !is_value_valid<std::int8_t>()) ||
            (type_.flag_is_raised(arg_value_types::INT16) &&
             !is_value_valid<std::int16_t>()) ||
            (type_.flag_is_raised(arg_value_types::INT32) &&
             !is_value_valid<std::int32_t>()) ||
            (type_.flag_is_raised(arg_value_types::INT64) &&
             !is_value_valid<std::int64_t>()) ||
            (type_.flag_is_raised(arg_value_types::LONG) &&
             !is_value_valid<long>()) ||
            (type_.flag_is_raised(arg_value_types::LONG_DOUBLE) &&
             !is_value_valid<long double>()) ||
            (type_.flag_is_raised(arg_value_types::LONG_LONG) &&
             !is_value_valid<long long>()) ||
            (type_.flag_is_raised(arg_value_types::SHORT) &&
             !is_value_valid<short>()) ||
            (type_.flag_is_raised(arg_value_types::STRING) &&
             !is_value_valid<string_type>()) ||
            (type_.flag_is_raised(arg_value_types::UINT8) &&
             !is_value_valid<std::uint8_t>()) ||
            (type_.flag_is_raised(arg_value_types::UINT16) &&
             !is_value_valid<std::uint16_t>()) ||
            (type_.flag_is_raised(arg_value_types::UINT32) &&
             !is_value_valid<std::uint32_t>()) ||
            (type_.flag_is_raised(arg_value_types::UINT64) &&
             !is_value_valid<std::uint64_t>()) ||
            (type_.flag_is_raised(arg_value_types::UNSIGNED_INT) &&
             !is_value_valid<unsigned int>()) ||
            (type_.flag_is_raised(arg_value_types::UNSIGNED_LONG) &&
             !is_value_valid<unsigned long>()) ||
            (type_.flag_is_raised(arg_value_types::UNSIGNED_LONG_LONG) &&
             !is_value_valid<unsigned long long>()) ||
            (type_.flag_is_raised(arg_value_types::UNSIGNED_SHORT) &&
             !is_value_valid<unsigned short>()) ||
            (type_.get_value() >= arg_value_types::R_FILE &&
             !is_value_valid<stdfs::path>()))
        {
            error_flags_.raise_flag(arg_value_error_flags::WRONG_VALUE_ERROR);
        }
        
        try
        {
            if (!regex_to_match.empty())
            {
                if (!std::regex_match(value_, std::basic_regex<char_type>(regex_to_match)))
                {
                    error_flags_.raise_flag(arg_value_error_flags::REGEX_TO_MATCH_ERROR);
                    error_message_ =
                            kcs::type_casting::type_cast<string_type>("Invalid argument");
                }
            }
        }
        catch (const std::regex_error& re)
        {
        }
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_arg_value(const basic_arg_value& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_arg_value(basic_arg_value&& rhs) noexcept = default;
    
    /**
     * @brief       Destructor.
     */
    ~basic_arg_value() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_arg_value& operator =(const basic_arg_value& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_arg_value& operator =(basic_arg_value&& rhs) noexcept = default;
    
    /**
     * @brief       Get the raw object string.
     * @return      The raw object string.
     */
    inline const string_type& get_value() const noexcept
    {
        return value_;
    }
    
    /**
     * @brief       Allows knowing wether the value can be coverted to the specified type.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename T>
    std::enable_if_t<
            !kcs::type_traits::is_path<T>::value,
            bool
    >
    check_type() const
    {
        if (std::is_arithmetic<T>::value)
        {
            if (composite_flags_.flag_is_raised(arg_flags::ALLOW_MIN_CONSTANT) &&
                    kcs::stringutils::strcmp(value_, "min") == 0)
            {
                return true;
            }
            else if (composite_flags_.flag_is_raised(arg_flags::ALLOW_MAX_CONSTANT) &&
                    kcs::stringutils::strcmp(value_, "max") == 0)
            {
                return true;
            }
        }
        
        T aux;
        return kcs::type_casting::try_type_cast<T>(value_, aux);
    }
    
    /**
     * @brief       Allows knowing wether the value can be coverted to the specified type.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename T>
    std::enable_if_t<
            kcs::type_traits::is_path<T>::value,
            bool
    >
    check_type() const noexcept
    {
        namespace stdfs = std::experimental::filesystem;
    
        stdfs::path path(value_);
        bool success = true;
        std::error_condition error_condition;
    
        if ((type_.flag_is_raised(arg_value_types::R_FILE) &&
             !kcs::filesystem::can_current_process_read_file(path, error_condition)) ||
            (type_.flag_is_raised(arg_value_types::W_FILE) &&
             !kcs::filesystem::can_current_process_write_file(path, error_condition)) ||
            (type_.flag_is_raised(arg_value_types::WC_FILE) &&
             !kcs::filesystem::can_current_process_write_or_create_file(path, error_condition)) ||
            (type_.flag_is_raised(arg_value_types::X_FILE) &&
             !kcs::filesystem::can_current_process_execute_file(path, error_condition)) ||
            (type_.flag_is_raised(arg_value_types::R_DIR) &&
             !kcs::filesystem::can_current_process_read_dir(path, error_condition)) ||
            (type_.flag_is_raised(arg_value_types::W_DIR) &&
             !kcs::filesystem::can_current_process_write_dir(path, error_condition)) ||
            (type_.flag_is_raised(arg_value_types::X_DIR) &&
             !kcs::filesystem::can_current_process_execute_dir(path, error_condition)))
        {
            success = false;
        }
    
        return success;
    }
    
    /**
     * @brief       Get the value converted to tp.
     * @return      The value converted to tp.
     * @throw       kcs::type_casting::bad_type_cast : If no conversion could be performed, an
     *              exception is thrown.
     */
    template<typename T>
    std::enable_if_t<
            std::is_arithmetic<T>::value,
            T
    >
    type_cast() const
    {
        if (composite_flags_.flag_is_raised(arg_flags::ALLOW_MIN_CONSTANT) &&
            kcs::stringutils::strcmp(value_, "min") == 0)
        {
            return std::numeric_limits<T>::min();
        }
        else if (composite_flags_.flag_is_raised(arg_flags::ALLOW_MAX_CONSTANT) &&
                 kcs::stringutils::strcmp(value_, "max") == 0)
        {
            return std::numeric_limits<T>::max();
        }
        
        return kcs::type_casting::type_cast<T>(value_);
    }
    
    /**
     * @brief       Get the value converted to tp.
     * @return      The value converted to tp.
     * @throw       kcs::type_casting::bad_type_cast : If no conversion could be performed, an
     *              exception is thrown.
     */
    template<typename T>
    inline std::enable_if_t<
            !std::is_arithmetic<T>::value,
            T
    >
    type_cast() const
    {
        return kcs::type_casting::type_cast<T>(value_);
    }
    
    /**
     * @brief       Get the value converted to tp.
     * @param       nothrow_value : This parameter is only used to distinguish it from the first
     *              version with an overloaded version.
     * @return      If function was successful the value converted to tp is returned, otherwise
     *              false is returned.
     */
    template<typename T>
    std::enable_if_t<
            std::is_arithmetic<T>::value,
            T
    >
    type_cast(const std::nothrow_t& nothrow_value) const
    {
        if (composite_flags_.flag_is_raised(arg_flags::ALLOW_MIN_CONSTANT) &&
            kcs::stringutils::strcmp(value_, "min") == 0)
        {
            return std::numeric_limits<T>::min();
        }
        else if (composite_flags_.flag_is_raised(arg_flags::ALLOW_MAX_CONSTANT) &&
                 kcs::stringutils::strcmp(value_, "max") == 0)
        {
            return std::numeric_limits<T>::max();
        }
        
        T result = T();
        kcs::type_casting::try_type_cast<T>(value_, result);
        return result;
    }
    
    /**
     * @brief       Get the value converted to tp.
     * @param       nothrow_value : This parameter is only used to distinguish it from the first
     *              version with an overloaded version.
     * @return      If function was successful the value converted to tp is returned, otherwise
     *              false is returned.
     */
    template<typename T>
    std::enable_if_t<
            !std::is_arithmetic<T>::value,
            T
    >
    type_cast(const std::nothrow_t& nothrow_value) const
    {
        T res = T();
        kcs::type_casting::try_type_cast<T>(value_, res);
        return res;
    }
    
    /**
     * @brief       Try to get the value converted to tp.
     * @param       result : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename T>
    std::enable_if_t<
            std::is_arithmetic<T>::value,
            bool
    >
    try_type_cast(T& result) const noexcept
    {
        if (composite_flags_.flag_is_raised(arg_flags::ALLOW_MIN_CONSTANT) &&
                kcs::stringutils::strcmp(value_, "min") == 0)
        {
            result = std::numeric_limits<T>::min();
            return true;
        }
        else if (composite_flags_.flag_is_raised(arg_flags::ALLOW_MAX_CONSTANT) &&
                kcs::stringutils::strcmp(value_, "max") == 0)
        {
            result = std::numeric_limits<T>::max();
            return true;
        }
        
        return kcs::type_casting::try_type_cast<T>(value_, result);
    }
    
    /**
     * @brief       Try to get the value converted to tp.
     * @param       result : The object that will contain the result of the conversion.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename T>
    inline std::enable_if_t<
            !std::is_arithmetic<T>::value,
            bool
    >
    try_type_cast(T& result) const noexcept
    {
        return kcs::type_casting::try_type_cast<T>(value_, result);
    }
    
    /**
     * @brief       Allows knowing whether the argument value has errors.
     * @return      If there are errors true is returned, otherwise false is returned.
     */
    inline bool there_are_errors() const noexcept
    {
        return !error_flags_.is_empty();
    }

private:
    /**
     * @brief       Print argument value errors.
     * @param       program_name : The name of the program.
     * @param       error_id : The argument error id.
     */
    void print_errors(const string_type& program_name, const string_type& error_id) const
    {
        auto& os = kcs::iostream::get_cout<TpChar>();
        
        if (error_flags_.flag_is_raised(arg_value_error_flags::WRONG_VALUE_ERROR) ||
            error_flags_.flag_is_raised(arg_value_error_flags::REGEX_TO_MATCH_ERROR))
        {
            os << program_name << ": ";
            if (!error_id.empty())
            {
                os << error_id << ": ";
            }
            
            if (invalid_path_)
            {
                os << value_ << ": " << error_message_ << std::endl;
            }
            else
            {
                os << error_message_ << " '" << value_ << "'" << std::endl;
            }
        }
    }
    
    /**
     * @brief       Allows knowing wether the value can be coverted to the specified type.
     * @return      If function was successful true is returned, otherwise false is returned.
     * @note        If the construction of a tp object does not throw exceptions, the function does
     *              not throw exception.
     */
    template<typename T>
    inline std::enable_if_t<
            !kcs::type_traits::is_path<T>::value,
            bool
    >
    is_value_valid() noexcept(noexcept(T()))
    {
        if (std::is_arithmetic<T>::value)
        {
            if (composite_flags_.flag_is_raised(arg_flags::ALLOW_MIN_CONSTANT) &&
                    kcs::stringutils::strcmp(value_, "min") == 0)
            {
                return true;
            }
            else if (composite_flags_.flag_is_raised(arg_flags::ALLOW_MAX_CONSTANT) &&
                    kcs::stringutils::strcmp(value_, "max") == 0)
            {
                return true;
            }
        }
        
        T aux;
        bool success = kcs::type_casting::try_type_cast<T>(value_, aux);
        
        if (!success && std::is_arithmetic<T>::value)
        {
            error_message_ =
                    kcs::type_casting::type_cast<string_type>("Invalid number");
        }
        
        return success;
    }
    
    /**
     * @brief       Allows knowing wether the value is a valid path.
     * @return      If function was successful true is returned, otherwise false is returne.
     */
    template<typename T>
    std::enable_if_t<
            kcs::type_traits::is_path<T>::value,
            bool
    >
    is_value_valid() noexcept
    {
        namespace stdfs = std::experimental::filesystem;
    
        stdfs::path path(value_);
        bool success = true;
        std::error_condition error_condition;
    
        if ((type_.flag_is_raised(arg_value_types::R_FILE) &&
             !kcs::filesystem::can_current_process_read_file(path, error_condition)) ||
            (type_.flag_is_raised(arg_value_types::W_FILE) &&
             !kcs::filesystem::can_current_process_write_file(path, error_condition)) ||
            (type_.flag_is_raised(arg_value_types::WC_FILE) &&
             !kcs::filesystem::can_current_process_write_or_create_file(path, error_condition)) ||
            (type_.flag_is_raised(arg_value_types::X_FILE) &&
             !kcs::filesystem::can_current_process_execute_file(path, error_condition)) ||
            (type_.flag_is_raised(arg_value_types::R_DIR) &&
             !kcs::filesystem::can_current_process_read_dir(path, error_condition)) ||
            (type_.flag_is_raised(arg_value_types::W_DIR) &&
             !kcs::filesystem::can_current_process_write_dir(path, error_condition)) ||
            (type_.flag_is_raised(arg_value_types::X_DIR) &&
             !kcs::filesystem::can_current_process_execute_dir(path, error_condition)))
        {
            success = false;
        }
        
        if (!success)
        {
            invalid_path_ = true;
            
            if (error_condition.value() == EINVAL)
            {
                error_message_ =
                        kcs::type_casting::type_cast<string_type>("Not a regular file");
            }
            else
            {
                error_message_ =
                        kcs::type_casting::type_cast<string_type>(error_condition.message());
            }
        }
    
        return success;
    }

private:
    /** Argument value container. */
    string_type value_;
    
    /** Type of the value. */
    flags_container_type<arg_value_types> type_;
    
    /** Regex that the value has to match. If the string is empty the value will always match. */
    string_type regex_to_match_;
    
    /** Error flags that allows knowing wether there are errors. */
    flags_container_type<arg_value_error_flags> error_flags_;
    
    /** Indicates wether the path is invalid. */
    bool invalid_path_;
    
    /** Message to be displayed when there are errors. */
    string_type error_message_;
    
    /** Composite flags. */
    flags_container_type<arg_flags> composite_flags_;
};


/** Class that represents a value for an argument base on 8 bit characters. */
using arg_value = basic_arg_value<char>;


/** Class that represents a value for an argument base on 16 bit characters. */
using warg_value = basic_arg_value<wchar_t>;
    
    
}
}


#endif
