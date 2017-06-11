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
 *  @file       kcs/argparse/basic_arg_parser.hpp
 *  @brief      basic_arg_parser class header.
 *  @author     Killian
 *  @date       2015/12/31 - 04:51
 */

#ifndef KCS_ARGPARSE_BASIC_ARG_PARSER_HPP
#define KCS_ARGPARSE_BASIC_ARG_PARSER_HPP

#include <unordered_map>
#include <unordered_set>

#include "../lowlevel/flags_container.hpp"
#include "../stringutils/stringutils.hpp"
#include "arg_parser_error_flags.hpp"
#include "arg_parser_flags.hpp"
#include "basic_foreign_arg.hpp"
#include "basic_help_arg.hpp"
#include "basic_help_text.hpp"
#include "basic_key_value_arg.hpp"
#include "basic_version_arg.hpp"


namespace kcs {
namespace argparse {


/**
 * @brief       Class used to parse arguments.
 */
template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAlloc = std::allocator<int>
>
class basic_arg_parser
{
public:
    /** Fundamental type that represents character type used in the class. */
    using char_type = TpChar;
    
    /** class that defines essential properties of the characters used by basic_string objects */
    using char_traits_type = TpCharTraits;
    
    /** Class that represents the allocator used in the class. */
    template<typename T>
    using allocator_type = typename TpAlloc::template rebind<T>::other;
    
    /** Class that represents a text in the help information.*/
    using ihelp_text_type = ibasic_help_text<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents a text in the help information.*/
    using help_text_type = basic_help_text<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents the base of the arguments hierarchy. */
    using base_arg_type = basic_base_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents arguments that have keys. */
    using key_arg_type = basic_key_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents arguments that have values. */
    using value_arg_type = basic_value_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents the option to get the help information. */
    using help_arg_type = basic_help_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents the option to get the version information. */
    using version_arg_type = basic_version_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents arguments that have keys and values. */
    using key_value_arg_type = basic_key_value_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents arguments without keys. */
    using foreign_arg_type = basic_foreign_arg<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents a key for an argument. */
    using arg_key_type = basic_arg_key<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents a value for an argument. */
    using arg_value_type = basic_arg_value<TpChar, TpCharTraits, TpAlloc>;
    
    /** Class that represents a bit field */
    template<typename T>
    using flags_container_type = kcs::lowlevel::flags_container<T>;
    
    /** Class that represent sequences of characters. */
    using string_type = std::basic_string<TpChar, TpCharTraits, allocator_type<char_type>>;
    
    /** Class that represents a sequence container. */
    template<typename T>
    using vector_type = std::vector<T, allocator_type<T>>;
    
    /** Class that represents associative containers that store elements formed by the combination
     *  of a key value and a mapped value. */
    template<typename TpKey_, typename TpValue_>
    using unordered_map_type = std::unordered_map<
            TpKey_, TpValue_, std::hash<TpKey_>,
            std::equal_to<TpKey_>,
            allocator_type<std::pair<const TpKey_, TpValue_>>
    >;
    
    /** Class that represents containers that store unique elements in no particular order, and
     *  which allow for fast retrieval of individual elements based on their value. */
    template<typename TpKey_>
    using unordered_set_type = std::unordered_set<
            TpKey_, std::hash<TpKey_>,
            std::equal_to<TpKey_>,
            allocator_type<TpKey_>
    >;
    
    /**
     * @brief       Singleton class used to forward parameters to the basic_arg_parser
     *              constructor.
     */
    class constructor_params
    {
    public:
        /**
         * @brief       Get the constructor_params object.
         * @return      The constructor_params object.
         */
        static constructor_params& get()
        {
            static constructor_params instance;
            
            instance.program_name_ = string_type();
            instance.short_prefixes_ = {kcs::type_casting::type_cast<string_type>("-")};
            instance.long_prefixes_ = {kcs::type_casting::type_cast<string_type>("--")};
            instance.arg_description_indentation_ = 2u;
            instance.max_description_line_length_ = 80u;
            instance.description_newline_indentation_ = 2u;
            instance.error_id_ = string_type();
            instance.max_unrecognized_args_ = std::numeric_limits<std::size_t>::max();
            instance.flags_ = arg_parser_flags::DEFAULT_ARG_PARSER_FLAGS;
            
            return instance;
        }
    
        /**
         * @brief       Set the program name.
         * @param       program_name : The name of the program.
         * @return      The object who call the method.
         */
        template<typename TpString__>
        constructor_params& program_name(TpString__&& program_name)
        {
            program_name_ = std::forward<TpString__>(program_name);
            return *this;
        }
    
        /**
         * @brief       Set the short prefixes.
         * @param       short_prefixes : Collection that contains the arguments short prefixes.
         * @return      The object who call the method.
         */
        template<typename TpStringSet__ = unordered_set_type<string_type>>
        constructor_params& short_prefixes(TpStringSet__&& short_prefixes)
        {
            short_prefixes_ = std::forward<TpStringSet__>(short_prefixes);
            return *this;
        }
    
        /**
         * @brief       Set the long prefixes.
         * @param       long_prefixes : Collection that contains the arguments long prefixes.
         * @return      The object who call the method.
         */
        template<typename TpStringSet__ = unordered_set_type<string_type>>
        constructor_params& long_prefixes(TpStringSet__&& long_prefixes)
        {
            long_prefixes_ = std::forward<TpStringSet__>(long_prefixes);
            return *this;
        }
    
        /**
         * @brief       Set the key help description indentation.
         * @param       arg_description_indentation : Indentation used to print arguments help
         *              description.
         * @return      The object who call the method.
         */
        constructor_params& arg_description_indentation(std::size_t arg_description_indentation)
        {
            arg_description_indentation_ = arg_description_indentation;
            return *this;
        }
    
        /**
         * @brief       Set the maximum description line length.
         * @param       max_description_line_length : The maximum description length that will be
         *              printed in a single line.
         * @return      The object who call the method.
         */
        constructor_params& max_description_line_length(std::size_t max_description_line_length)
        {
            max_description_line_length_ = max_description_line_length;
            return *this;
        }
    
        /**
         * @brief       Set the newline indentation length.
         * @param       description_newline_indentation : The indentation used when a newline is
         *              found in a description.
         * @return      The object who call the method.
         */
        constructor_params& description_newline_indentation(
                std::size_t description_newline_indentation
        )
        {
            description_newline_indentation_ = description_newline_indentation;
            return *this;
        }
    
        /**
         * @brief       Set the error id.
         * @param       error_id : Error id that will be used for global errors.
         * @return      The object who call the method.
         */
        constructor_params& error_id(string_type error_id)
        {
            error_id_ = error_id;
            return *this;
        }
    
        /**
         * @brief       Set the maximum unrecognized arguments.
         * @param       max_unrecognized_args : The muximum number of unrecognized arguments that
         *              will be catched.
         * @return      The object who call the method.
         */
        constructor_params& max_unrecognized_args(std::size_t max_unrecognized_args)
        {
            max_unrecognized_args_ = max_unrecognized_args;
            return *this;
        }
    
        /**
         * @brief       Set the flags.
         * @param       flags : Flags that dictates the argument parser behavior.
         * @return      The object who call the method.
         */
        constructor_params& flags(arg_parser_flags flags)
        {
            flags_ = flags;
            return *this;
        }
    
    private:
        /** @cond */
        constructor_params()
        {
        }
        
        constructor_params(const constructor_params&) = delete;
        constructor_params(constructor_params&&) = delete;
        constructor_params& operator =(const constructor_params&) = delete;
        constructor_params& operator =(constructor_params&&) = delete;
    
        string_type program_name_;
        unordered_set_type<string_type> short_prefixes_;
        unordered_set_type<string_type> long_prefixes_;
        std::size_t arg_description_indentation_;
        std::size_t max_description_line_length_;
        std::size_t description_newline_indentation_;
        string_type error_id_;
        std::size_t max_unrecognized_args_;
        arg_parser_flags flags_;
        
        friend class basic_arg_parser;
        /** @endcond */
    };
    
    /**
     * @brief       Singleton class used to forward parameters to the add_key_value_arg
     *              methode.
     */
    class add_key_value_arg_params
    {
    public:
        /**
         * @brief       Get the add_key_value_arg_params object.
         * @return      The add_key_value_arg_params object.
         */
        static add_key_value_arg_params& get()
        {
            static add_key_value_arg_params instance;
            
            instance.keys_ = vector_type<string_type>();
            instance.description_ = string_type();
            instance.values_types_ = {arg_value_types::STRING};
            instance.min_values_ = 1;
            instance.max_values_ = 1;
            instance.flags_ = arg_flags::DEFAULT_ARG_FLAGS;
            instance.regex_collection_ = vector_type<string_type>();
            instance.error_id_ = string_type();
            
            return instance;
        }
    
        /**
         * @brief       Set the argument keys.
         * @param       keys : Strings vector that contains the argument keys.
         * @return      The object who call the method.
         */
        template<typename TpStringVector__ = vector_type<string_type>>
        add_key_value_arg_params& keys(TpStringVector__&& keys)
        {
            keys_ = std::forward<TpStringVector__>(keys);
            return *this;
        }
    
        /**
         * @brief       Set the argument description.
         * @param       description : Argument description content. If the string is empty the
         *              argument will not be displayed when the help information will be printed.
         * @return      The object who call the method.
         */
        template<typename TpString__>
        add_key_value_arg_params& description(TpString__&& description)
        {
            description_ = std::forward<TpString__>(description);
            return *this;
        }
    
        /**
         * @brief       Set the argument values types.
         * @param       values_types : Vector that has the values types that must have the argument
         *              values.
         * @return      The object who call the method.
         */
        template<typename TpArgsValueTypesVector__ = vector_type<arg_value_types>>
        add_key_value_arg_params& values_types(TpArgsValueTypesVector__&& values_types)
        {
            values_types_ = std::forward<TpArgsValueTypesVector__>(values_types);
            return *this;
        }
    
        /**
         * @brief       Set the minumum values.
         * @param       min_values : Minimum number of values for an option.
         * @return      The object who call the method.
         */
        add_key_value_arg_params& min_values(std::size_t min_values)
        {
            min_values_ = min_values;
            return *this;
        }
    
        /**
         * @brief       Set the maximum values.
         * @param       max_values : Maximum number of values for an option.
         * @return      The object who call the method.
         */
        add_key_value_arg_params& max_values(std::size_t max_values)
        {
            max_values_ = max_values;
            return *this;
        }
    
        /**
         * @brief       Set the argument flags.
         * @param       flags : Flags that dictates the argument behavior.
         * @return      The object who call the method.
         */
        add_key_value_arg_params& flags(arg_flags flags)
        {
            flags_ = flags;
            return *this;
        }
        
        /**
         * @brief       Set the argument regex collection.
         * @param       regex_collection : Regular expressions that the values have to match. If the
         *              string is empty all values will match.
         * @return      The object who call the method.
         */
        template<typename TpStringVector__ = vector_type<string_type>>
        add_key_value_arg_params& regex_collection(TpStringVector__&& regex_collection)
        {
            regex_collection_ = std::forward<TpStringVector__>(regex_collection);
            return *this;
        }
    
        /**
         * @brief       Set the argument error id.
         * @param       error_id : The id used to reference an arguments during the error display.
         * @return      The object who call the method.
         */
        template<typename TpString__>
        add_key_value_arg_params& error_id(TpString__&& error_id)
        {
            error_id_ = std::forward<TpString__>(error_id);
            return *this;
        }
        
    private:
        /** @cond */
        add_key_value_arg_params()
        {
        }
    
        add_key_value_arg_params(const add_key_value_arg_params&) = delete;
        add_key_value_arg_params(add_key_value_arg_params&&) = delete;
        add_key_value_arg_params& operator =(const add_key_value_arg_params&) = delete;
        add_key_value_arg_params& operator =(add_key_value_arg_params&&) = delete;
    
        vector_type<string_type> keys_;
        string_type description_;
        vector_type<arg_value_types> values_types_;
        std::size_t min_values_;
        std::size_t max_values_;
        arg_flags flags_;
        vector_type<string_type> regex_collection_;
        string_type error_id_;
    
        friend class basic_arg_parser;
        /** @endcond */
    };
    
    /**
     * @brief       Singleton class used to forward parameters to the add_foreign_arg methode.
     */
    class add_foreign_arg_params
    {
    public:
        /**
         * @brief       Get the add_foreign_arg_params object.
         * @return      The add_foreign_arg_params object.
         */
        static add_foreign_arg_params& get()
        {
            static add_foreign_arg_params instance;
            
            instance.usage_key_ = string_type();
            instance.help_key_ = string_type();
            instance.description_ = string_type();
            instance.values_types_ = {arg_value_types::STRING};
            instance.min_values_ = 1;
            instance.max_values_ = 1;
            instance.flags_ = arg_flags::DEFAULT_FOREIGN_ARG_FLAGS;
            instance.regex_collection_ = vector_type<string_type>();
            instance.error_id_ = string_type();
            
            return instance;
        }
    
        /**
         * @brief       Set the argument usage key.
         * @param       usage_key : The key used to reference an arguments during the help usage
         *              sentence display.
         * @return      The object who call the method.
         */
        template<typename TpString__>
        add_foreign_arg_params& usage_key(TpString__&& usage_key)
        {
            usage_key_ = std::forward<TpString__>(usage_key);
            return *this;
        }
    
        /**
         * @brief       Set the argument help key.
         * @param       help_key : The key used to reference an arguments during the help display.
         * @return      The object who call the method.
         */
        template<typename TpString__>
        add_foreign_arg_params& help_key(TpString__&& help_key)
        {
            help_key_ = std::forward<TpString__>(help_key);
            return *this;
        }
    
        /**
         * @brief       Set the argument description.
         * @param       description : Argument description content. If the string is empty the
         *              argument will not be displayed when the help information will be printed.
         * @return      The object who call the method.
         */
        template<typename TpString__>
        add_foreign_arg_params& description(TpString__&& description)
        {
            description_ = std::forward<TpString__>(description);
            return *this;
        }
    
        /**
         * @brief       Set the argument values types.
         * @param       values_types : Vector that has the values types that must have the argument
         *              values.
         * @return      The object who call the method.
         */
        template<typename TpArgValueTypesVector__ = vector_type<arg_value_types>>
        add_foreign_arg_params& values_types(TpArgValueTypesVector__&& values_types)
        {
            values_types_ = std::forward<TpArgValueTypesVector__>(values_types);
            return *this;
        }
    
        /**
         * @brief       Set the minumum values.
         * @param       min_values : Minimum number of values for an option.
         * @return      The object who call the method.
         */
        add_foreign_arg_params& min_values(std::size_t min_values)
        {
            min_values_ = min_values;
            return *this;
        }
    
        /**
         * @brief       Set the maximum values.
         * @param       max_values : Maximum number of values for an option.
         * @return      The object who call the method.
         */
        add_foreign_arg_params& max_values(std::size_t max_values)
        {
            max_values_ = max_values;
            return *this;
        }
    
        /**
         * @brief       Set the argument flags.
         * @param       flags : Flags that dictates the argument behavior.
         * @return      The object who call the method.
         */
        add_foreign_arg_params& flags(arg_flags flags)
        {
            flags_ = flags;
            return *this;
        }
    
        /**
         * @brief       Set the argument regex collection.
         * @param       regex_collection : Regular expressions that the values have to match. If the
         *              string is empty all values will match.
         * @return      The object who call the method.
         */
        template<typename TpStringVector__ = vector_type<string_type>>
        add_foreign_arg_params& regex_collection(TpStringVector__&& regex_collection)
        {
            regex_collection_ = std::forward<TpStringVector__>(regex_collection);
            return *this;
        }
    
        /**
         * @brief       Set the argument error id.
         * @param       error_id : The id used to reference an arguments during the error display.
         * @return      The object who call the method.
         */
        template<typename TpString__>
        add_foreign_arg_params& error_id(TpString__&& error_id)
        {
            error_id_ = std::forward<TpString__>(error_id);
            return *this;
        }
        
    private:
        /** @cond */
        add_foreign_arg_params()
        {
        }
    
        add_foreign_arg_params(const add_foreign_arg_params&) = delete;
        add_foreign_arg_params(add_foreign_arg_params&&) = delete;
        add_foreign_arg_params& operator =(const add_foreign_arg_params&) = delete;
        add_foreign_arg_params& operator =(add_foreign_arg_params&&) = delete;
        
        string_type usage_key_;
        string_type help_key_;
        string_type description_;
        vector_type<arg_value_types> values_types_;
        std::size_t min_values_;
        std::size_t max_values_;
        arg_flags flags_;
        vector_type<string_type> regex_collection_;
        string_type error_id_;
    
        friend class basic_arg_parser;
        /** @endcond */
    };
    
    /**
     * @brief       Perfect forwarding constructor.
     * @param       program_name : The program name.
     * @param       short_prefixes : Collection that contains the arguments short prefixes.
     * @param       long_prefixes : Collection that contains the arguments long prefixes.
     * @param       arg_description_indentation : Indentation used to print arguments help
     *              description.
     * @param       max_description_line_length : The maximum description length that will be
     *              printed in a single line.
     * @param       description_newline_indentation : The indentation used when a newline is found
     *              in a description.
     * @param       error_id : The argument parser error id.
     * @param       max_unrecognized_args : The muximum number of unrecognized arguments that will
     *              be catched.
     * @param       flags : Flags that dictates the argument parser behavior.
     */
    template<
            typename TpString_ = string_type,
            typename TpStringSet1_ = unordered_set_type<string_type>,
            typename TpStringSet2_ = unordered_set_type<string_type>,
            typename = std::enable_if_t<
                    !std::is_base_of<basic_arg_parser, std::decay_t<TpString_>>::value
            >
    >
    basic_arg_parser(
            TpString_&& program_name = string_type(),
            TpStringSet1_&& short_prefixes = {kcs::type_casting::type_cast<string_type>("-")},
            TpStringSet2_&& long_prefixes = {kcs::type_casting::type_cast<string_type>("--")},
            std::size_t arg_description_indentation = 2u,
            std::size_t max_description_line_length = 80u,
            std::size_t description_newline_indentation = 2u,
            string_type error_id = string_type(),
            std::size_t max_unrecognized_args = std::numeric_limits<std::size_t>::max(),
            arg_parser_flags flags = arg_parser_flags::DEFAULT_ARG_PARSER_FLAGS
    )
            : program_name_(std::forward<TpString_>(program_name))
            , short_prefixes_(std::forward<TpStringSet1_>(short_prefixes))
            , long_prefixes_(std::forward<TpStringSet2_>(long_prefixes))
            , arg_description_indentation_(arg_description_indentation)
            , max_description_line_length_(max_description_line_length)
            , description_newline_indentation_(description_newline_indentation)
            , ihelp_text_list_()
            , current_help_arg_(nullptr)
            , current_version_arg_(nullptr)
            , foreign_args_list_()
            , base_arg_map_()
            , error_id_(error_id)
            , unrecognized_args_()
            , max_unrecognized_args_(max_unrecognized_args)
            , flags_(flags)
            , error_flags_(arg_parser_error_flags::NIL)
    {
        if (error_id_.empty() &&
            flags_.flag_is_raised(arg_parser_flags::USE_DEFAULT_ERROR_ID_IF_ERROR_ID_EMPTY))
        {
            error_id_ = "error";
        }
    }
    
    /**
     * @brief       Constructor with parameters.
     * @param       constructor_params : Object that contains all the parameters to call this
     *              constructor.
     */
    basic_arg_parser(constructor_params& constructor_params)
            : program_name_(std::move(constructor_params.program_name_))
            , short_prefixes_(std::move(constructor_params.short_prefixes_))
            , long_prefixes_(std::move(constructor_params.long_prefixes_))
            , arg_description_indentation_(constructor_params.arg_description_indentation_)
            , max_description_line_length_(constructor_params.max_description_line_length_)
            , description_newline_indentation_(constructor_params.description_newline_indentation_)
            , ihelp_text_list_()
            , current_help_arg_(nullptr)
            , current_version_arg_(nullptr)
            , foreign_args_list_()
            , base_arg_map_()
            , error_id_(std::move(constructor_params.error_id_))
            , unrecognized_args_()
            , max_unrecognized_args_(constructor_params.max_unrecognized_args_)
            , flags_(constructor_params.flags_)
            , error_flags_(arg_parser_error_flags::NIL)
    {
        if (error_id_.empty() &&
            flags_.flag_is_raised(arg_parser_flags::USE_DEFAULT_ERROR_ID_IF_ERROR_ID_EMPTY))
        {
            error_id_ = "error";
        }
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_arg_parser(const basic_arg_parser& rhs)
            : program_name_(rhs.program_name_)
            , short_prefixes_(rhs.short_prefixes_)
            , long_prefixes_(rhs.long_prefixes_)
            , arg_description_indentation_(rhs.arg_description_indentation_)
            , max_description_line_length_(rhs.max_description_line_length_)
            , description_newline_indentation_(rhs.description_newline_indentation_)
            , ihelp_text_list_()
            , current_help_arg_(nullptr)
            , current_version_arg_(nullptr)
            , foreign_args_list_()
            , base_arg_map_()
            , error_id_(rhs.error_id_)
            , unrecognized_args_(rhs.unrecognized_args_)
            , max_unrecognized_args_(rhs.max_unrecognized_args_)
            , flags_(rhs.flags_)
            , error_flags_(rhs.error_flags_)
    {
        key_arg_type *key_arg;
        help_arg_type *help_arg;
        version_arg_type *version_arg;
        foreign_arg_type *foreign_arg;
        
        ihelp_text_list_ = rhs.get_args_list_deep_copy();
        
        for (auto& x : ihelp_text_list_)
        {
            key_arg = dynamic_cast<key_arg_type*>(x);
            help_arg = dynamic_cast<help_arg_type*>(x);
            version_arg = dynamic_cast<version_arg_type*>(x);
            foreign_arg = dynamic_cast<foreign_arg_type*>(x);
            
            if (key_arg != nullptr)
            {
                for (auto& y : key_arg->get_keys())
                {
                    base_arg_map_.insert(std::make_pair(y, key_arg));
                }
            }
            
            if (help_arg != nullptr)
            {
                current_help_arg_ = help_arg;
            }
            
            if (version_arg != nullptr)
            {
                current_version_arg_ = version_arg;
            }
            
            if (foreign_arg != nullptr)
            {
                foreign_args_list_.push_back(foreign_arg);
                base_arg_map_.insert(std::make_pair(foreign_arg->get_usage_key(), foreign_arg));
                base_arg_map_.insert(std::make_pair(foreign_arg->get_help_key(), foreign_arg));
            }
        }
    }
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_arg_parser(basic_arg_parser&& rhs) noexcept
            : program_name_(std::move(rhs.program_name_))
            , short_prefixes_(std::move(rhs.short_prefixes_))
            , long_prefixes_(std::move(rhs.long_prefixes_))
            , arg_description_indentation_(std::move(rhs.arg_description_indentation_))
            , max_description_line_length_(std::move(rhs.max_description_line_length_))
            , description_newline_indentation_(std::move(rhs.description_newline_indentation_))
            , ihelp_text_list_(std::move(rhs.ihelp_text_list_))
            , current_help_arg_(std::move(rhs.current_help_arg_))
            , current_version_arg_(std::move(rhs.current_version_arg_))
            , foreign_args_list_(std::move(rhs.foreign_args_list_))
            , base_arg_map_(std::move(rhs.base_arg_map_))
            , error_id_(std::move(rhs.error_id_))
            , unrecognized_args_(std::move(rhs.unrecognized_args_))
            , max_unrecognized_args_(std::move(rhs.max_unrecognized_args_))
            , flags_(std::move(rhs.flags_))
            , error_flags_(std::move(rhs.error_flags_))
    {
        rhs.arg_description_indentation_ = 0;
        rhs.current_help_arg_ = nullptr;
        rhs.current_version_arg_ = nullptr;
        rhs.max_unrecognized_args_ = 0;
    }
    
    /**
     * @brief       Destructor.
     */
    ~basic_arg_parser()
    {
        for (auto& x : ihelp_text_list_)
        {
            delete_ihelp_text(x);
        }
    
        current_help_arg_ = nullptr;
        current_version_arg_ = nullptr;
    }
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     */
    basic_arg_parser& operator =(const basic_arg_parser& rhs)
    {
        if (this != &rhs)
        {
            local_destructor();
            
            program_name_ = rhs.program_name_;
            short_prefixes_ = rhs.short_prefixes_;
            long_prefixes_ = rhs.long_prefixes_;
            arg_description_indentation_ = rhs.arg_description_indentation_;
            max_description_line_length_ = rhs.max_description_line_length_;
            description_newline_indentation_ = rhs.description_newline_indentation_;
            error_id_ = rhs.error_id_;
            unrecognized_args_ = rhs.unrecognized_args_;
            max_unrecognized_args_ = rhs.max_unrecognized_args_;
            flags_ = rhs.flags_;
            error_flags_ = rhs.error_flags_;
    
            key_arg_type *key_arg;
            help_arg_type *help_arg;
            version_arg_type *version_arg;
            foreign_arg_type *foreign_arg;
    
            ihelp_text_list_ = rhs.get_args_list_deep_copy();
    
            for (auto& x : ihelp_text_list_)
            {
                key_arg = dynamic_cast<key_arg_type*>(x);
                help_arg = dynamic_cast<help_arg_type*>(x);
                version_arg = dynamic_cast<version_arg_type*>(x);
                foreign_arg = dynamic_cast<foreign_arg_type*>(x);
        
                if (key_arg != nullptr)
                {
                    for (auto& y : key_arg->get_keys())
                    {
                        base_arg_map_.insert(std::make_pair(y, key_arg));
                    }
                }
        
                if (help_arg != nullptr)
                {
                    current_help_arg_ = help_arg;
                }
        
                if (version_arg != nullptr)
                {
                    current_version_arg_ = version_arg;
                }
        
                if (foreign_arg != nullptr)
                {
                    foreign_args_list_.push_back(foreign_arg);
                    base_arg_map_.insert(std::make_pair(foreign_arg->get_usage_key(), foreign_arg));
                    base_arg_map_.insert(std::make_pair(foreign_arg->get_help_key(), foreign_arg));
                }
            }
        }
        
        return *this;
    }
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     */
    basic_arg_parser& operator =(basic_arg_parser&& rhs) noexcept
    {
        if (this != &rhs)
        {
            program_name_ = std::move(rhs.program_name_);
            short_prefixes_ = std::move(rhs.short_prefixes_);
            long_prefixes_ = std::move(rhs.long_prefixes_);
            std::swap(arg_description_indentation_, rhs.arg_description_indentation_);
            std::swap(max_description_line_length_, rhs.max_description_line_length_);
            std::swap(description_newline_indentation_, rhs.description_newline_indentation_);
            ihelp_text_list_ = std::move(rhs.ihelp_text_list_);
            std::swap(current_help_arg_, rhs.current_help_arg_);
            std::swap(current_version_arg_, rhs.current_version_arg_);
            foreign_args_list_ = std::move(rhs.foreign_args_list_);
            base_arg_map_ = std::move(rhs.base_arg_map_);
            error_id_ = std::move(rhs.error_id_);
            unrecognized_args_ = std::move(rhs.unrecognized_args_);
            std::swap(max_unrecognized_args_, rhs.max_unrecognized_args_);
            flags_ = std::move(rhs.flags_);
            error_flags_ = std::move(rhs.error_flags_);
        }
        
        return *this;
    }
    
    /**
     * @brief       Add a string that represents a text in the help information.
     * @param       description : Argument description content.
     */
    template<typename TpString_>
    void add_help_text(TpString_&& description)
    {
        allocator_type<help_text_type> help_text_type_alloc;
        help_text_type *help_text = help_text_type_alloc.allocate(1);
        help_text_type_alloc.construct(help_text, std::forward<TpString_>(description));
        
        ihelp_text_list_.push_back(help_text);
    }
    
    /**
     * @brief       Add an argument that has keys.
     * @param       keys : Strings vector that contains the argument keys.
     * @param       description : Argument description content. If the string is empty the argument
     *              will not be displayed when the help information will be printed.
     * @param       flags : Flags that dictates the argument behavior.
     * @param       error_id : The id used to reference an arguments during the error display. If
     *              the string is empty the argument will display the first key.
     * @throw       kcs::argparse::arg_parser_exception : An exception is thrown if there is no key
     *              specified or if an argument with one of the keys already exist.
     */
    template<
            typename TpString1_,
            typename TpString2_ = string_type,
            typename TpStringVector_ = vector_type<string_type>
    >
    void add_key_arg(
            TpStringVector_&& keys,
            TpString1_&& description,
            arg_flags flags = arg_flags::DEFAULT_ARG_FLAGS,
            TpString2_&& error_id = string_type()
    )
    {
        if (keys.empty())
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there is no key "
                                               "specified");
        }
        
        for (auto& x : keys)
        {
            if (arg_key_exists(x))
            {
                throw arg_parser_exception("kcs::argparse::arg_parser_exception: one of the "
                                                   "keys already exist");
            }
        }
        
        auto arg_key_list = get_arg_key_list_from_strings(keys);
    
        allocator_type<key_arg_type> key_arg_type_alloc;
        key_arg_type *key_arg = key_arg_type_alloc.allocate(1);
        key_arg_type_alloc.construct(key_arg,
                                     std::forward<TpString1_>(description),
                                     std::forward<TpString2_>(error_id),
                                     flags,
                                     std::move(arg_key_list));
        
        for (auto& x : keys)
        {
            if (std::is_rvalue_reference<TpStringVector_&&>::value)
            {
                base_arg_map_.insert(std::make_pair(std::move(x),
                                                    static_cast<base_arg_type *>(key_arg)));
            }
            else
            {
                base_arg_map_.insert(std::make_pair(x, static_cast<base_arg_type *>(key_arg)));
            }
        }
        
        ihelp_text_list_.push_back(key_arg);
    }
    
    /**
     * @brief       Add an argument that represents the option to get the help information.
     * @param       keys : Strings vector that contains the argument keys.
     * @param       description : Argument description content. If the string is empty the argument
     *              will not be displayed when the help information will be printed.
     * @param       flags : Flags that dictates the argument behavior.
     * @param       error_id : The id used to reference an arguments during the error display If the
     *              string is empty the argument will display the first key.
     * @throw       kcs::argparse::arg_parser_exception : An exception is thrown if there is no key
     *              specified or if an argument with one of the keys already exist or if there is
     *              currently a help argument set.
     */
    template<
            typename TpString1_,
            typename TpString2_ = string_type,
            typename TpStringVector_ = vector_type<string_type>
    >
    void add_help_arg(
            TpStringVector_&& keys,
            TpString1_&& description,
            arg_flags flags = arg_flags::DEFAULT_ARG_FLAGS,
            TpString2_&& error_id = string_type()
    )
    {
        if (keys.empty())
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there is no key "
                                               "specified");
        }
        
        for (auto& x : keys)
        {
            if (arg_key_exists(x))
            {
                throw arg_parser_exception("kcs::argparse::arg_parser_exception: one of the "
                                                   "keys already exist");
            }
        }
        
        if (current_help_arg_ != nullptr)
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there is "
                                               "currently a help argument set");
        }
        
        auto arg_key_list = get_arg_key_list_from_strings(keys);
    
        allocator_type<help_arg_type> help_arg_type_alloc;
        help_arg_type *help_arg = help_arg_type_alloc.allocate(1);
        help_arg_type_alloc.construct(help_arg,
                                      std::forward<TpString1_>(description),
                                      std::forward<TpString2_>(error_id),
                                      flags,
                                      std::move(arg_key_list));
    
        for (auto& x : keys)
        {
            if (std::is_rvalue_reference<TpStringVector_&&>::value)
            {
                base_arg_map_.insert(std::make_pair(std::move(x),
                                                    static_cast<base_arg_type *>(help_arg)));
            }
            else
            {
                base_arg_map_.insert(std::make_pair(x, static_cast<base_arg_type *>(help_arg)));
            }
        }
        
        ihelp_text_list_.push_back(help_arg);
        current_help_arg_ = help_arg;
    }
    
    /**
     * @brief       Add an argument that represents the option to get the version information.
     * @param       keys : Strings vector that contains the argument keys.
     * @param       description : Argument description content. If the string is empty the argument
     *              will not be displayed when the help information will be printed.
     * @param       version_information : Argument version information content.
     * @param       flags : Flags that dictates the argument behavior.
     * @param       error_id : The id used to reference an arguments during the error display If the
     *              string is empty the argument will display the first key.
     * @throw       kcs::argparse::arg_parser_exception : An exception is thrown if there is no key
     *              specified or if an argument with one of the keys already exist or if there is
     *              currently a version argument set.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpString3_ = string_type,
            typename TpStringVector_ = vector_type<string_type>
    >
    void add_version_arg(
            TpStringVector_&& keys,
            TpString1_&& description,
            TpString2_&& version_information,
            arg_flags flags = arg_flags::DEFAULT_ARG_FLAGS,
            TpString3_&& error_id = string_type()
    )
    {
        if (keys.empty())
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there is no key "
                                             "specified");
        }
    
        for (auto& x : keys)
        {
            if (arg_key_exists(x))
            {
                throw arg_parser_exception("kcs::argparse::arg_parser_exception: one of the "
                                                   "keys already exist");
            }
        }
    
        if (current_version_arg_ != nullptr)
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there is "
                                               "corrently a version argument set");
        }
    
        auto arg_key_list = get_arg_key_list_from_strings(keys);
    
        allocator_type<version_arg_type> version_arg_type_alloc;
        version_arg_type *version_arg = version_arg_type_alloc.allocate(1);
        version_arg_type_alloc.construct(version_arg,
                                         std::forward<TpString1_>(description),
                                         std::forward<TpString3_>(error_id),
                                         flags,
                                         std::move(arg_key_list),
                                         std::forward<TpString2_>(version_information));
    
        for (auto& x : keys)
        {
            if (std::is_rvalue_reference<TpStringVector_&&>::value)
            {
                base_arg_map_.insert(std::make_pair(std::move(x),
                                                    static_cast<base_arg_type *>(version_arg)));
            }
            else
            {
                base_arg_map_.insert(std::make_pair(x, static_cast<base_arg_type *>(version_arg)));
            }
        }
        
        ihelp_text_list_.push_back(version_arg);
        current_version_arg_ = version_arg;
    }
    
    /**
     * @brief       Add an argument that represents the option to get the version information
     *              (Programs with GPLv3 licence).
     * @param       keys : Strings vector that contains the argument keys.
     * @param       description : Argument description content. If the string is empty the argument
     *              will not be displayed when the help information will be printed.
     * @param       program_version : The version that will be displayed in the vesion information.
     * @param       date : The program release date that will be displayed in the vesion
     *              information.
     * @param       author : The author name that will be displayed in the vesion information.
     * @param       flags : Flags that dictates the argument behavior.
     * @param       error_id : The id used to reference an arguments during the error display If the
     *              string is empty the argument will display the first key.
     * @throw       kcs::argparse::arg_parser_exception : An exception is thrown if there is no key
     *              specified or if an argument with one of the keys already exist or if there is
     *              currently a version argument set.
     */
    template<
            typename TpString1_,
            typename TpString2_ = string_type,
            typename TpStringVector_ = vector_type<string_type>
    >
    void add_gplv3_version_arg(
            TpStringVector_&& keys,
            TpString1_&& description,
            const string_type& program_version,
            const string_type& date,
            const string_type& author,
            arg_flags flags = arg_flags::DEFAULT_ARG_FLAGS,
            TpString2_&& error_id = string_type()
    )
    {
        string_type version_information;
        version_information.append(program_name_);
        version_information.append(kcs::type_casting::type_cast<string_type>(" "));
        version_information.append(program_version);
        version_information.append(kcs::type_casting::type_cast<string_type>("\nCopyright (C) "));
        version_information.append(date);
        version_information.append(kcs::type_casting::type_cast<string_type>(
                " Free Software Foundation, Inc.\n"
                        "License GPLv3+: GNU GPL version 3 or later "
                        "<http://gnu.org/licenses/gpl.html>1.\n"
                        "This is free software: you are free to change and redistribute it.\n"
                        "There is NO WARRANTY, to the extent permitted by law.\n\n"
                        "Written by "));
        version_information.append(author);
        version_information.append(kcs::type_casting::type_cast<string_type>("."));
    
        add_version_arg(std::forward<TpStringVector_>(keys),
                        std::forward<TpString1_>(description),
                        std::move(version_information),
                        flags,
                        std::forward<TpString2_>(error_id));
    }
    
    /**
     * @brief       Add an argument that has keys and values.
     * @param       keys : Strings vector that contains the argument keys.
     * @param       description : Argument description content. If the string is empty the argument
     *              will not be displayed when the help information will be printed.
     * @param       values_types : Vector that has the values types that must have the argument
     *              values.
     * @param       min_values : Minimum number of values for an option.
     * @param       max_values : Maximum number of values for an option.
     * @param       flags : Flags that dictates the argument behavior.
     * @param       regex_collection : Regular expressions that the values have to match. If the
     *              string is empty all values will match.
     * @param       error_id : The id used to reference an arguments during the error display. If
     *              the string is empty the argument will display the first key.
     * @throw       kcs::argparse::arg_parser_exception : An exception is thrown if there is no key
     *              specified or if an argument with one of the keys already exist.
     */
    template<
            typename TpString1_,
            typename TpString2_ = string_type,
            typename TpStringVector1_ = vector_type<string_type>,
            typename TpStringVector2_ = vector_type<string_type>,
            typename TpArgValueTypesVector_ = vector_type<arg_value_types>
    >
    void add_key_value_arg(
            TpStringVector1_&& keys,
            TpString1_&& description,
            TpArgValueTypesVector_&& values_types = {arg_value_types::STRING},
            std::size_t min_values = 1,
            std::size_t max_values = 1,
            arg_flags flags = arg_flags::DEFAULT_ARG_FLAGS,
            TpStringVector2_&& regex_collection = vector_type<string_type>(),
            TpString2_&& error_id = string_type()
    )
    {
        if (keys.empty())
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there is no key "
                                             "specified");
        }
    
        for (auto& x : keys)
        {
            if (arg_key_exists(x))
            {
                throw arg_parser_exception("kcs::argparse::arg_parser_exception: one of "
                                                   "the keys already exist");
            }
        }
    
        auto arg_key_list = get_arg_key_list_from_strings(keys);
    
        allocator_type<key_value_arg_type> key_value_arg_type_alloc;
        key_value_arg_type *key_value_arg = key_value_arg_type_alloc.allocate(1);
        key_value_arg_type_alloc.construct(key_value_arg,
                                           std::forward<TpString1_>(description),
                                           std::forward<TpString2_>(error_id),
                                           flags,
                                           std::move(arg_key_list),
                                           min_values,
                                           max_values,
                                           std::forward<TpArgValueTypesVector_>(values_types),
                                           std::forward<TpStringVector2_>(regex_collection));
    
        for (auto& x : keys)
        {
            if (std::is_rvalue_reference<TpStringVector1_&&>::value)
            {
                base_arg_map_.insert(std::make_pair(std::move(x),
                                                    static_cast<base_arg_type *>(key_value_arg)));
            }
            else
            {
                base_arg_map_.insert(std::make_pair(x, static_cast<base_arg_type *>(key_value_arg)));
            }
        }
        
        ihelp_text_list_.push_back(key_value_arg);
    }
    
    /**
     * @brief       Add an argument that has keys and values.
     * @param       add_key_value_params : Object that contains all the parameters to call this
     *              methode.
     * @throw       kcs::argparse::arg_parser_exception : An exception is thrown if there is no key
     *              specified or if an argument with one of the keys already exist.
     */
    void add_key_value_arg(add_key_value_arg_params& add_key_value_params)
    {
        add_key_value_arg(std::move(add_key_value_params.keys_),
                          std::move(add_key_value_params.description_),
                          std::move(add_key_value_params.values_types_),
                          std::move(add_key_value_params.min_values_),
                          std::move(add_key_value_params.max_values_),
                          std::move(add_key_value_params.flags_),
                          std::move(add_key_value_params.regex_collection_),
                          std::move(add_key_value_params.error_id_));
    }
    
    /**
     * @brief       Add an argument that represents an argument without keys.
     * @param       usage_key : The key used to reference an arguments during the help usage
     *              sentence display.
     * @param       help_key : The key used to reference an arguments during the help display.
     * @param       description : Argument description content. If the string is empty the argument
     *              will not be displayed when the help information will be printed.
     * @param       values_types : Vector that has the values types that must have the argument
     *              values.
     * @param       min_values : Minimum number of values for an option.
     * @param       max_values : Maximum number of values for an option.
     * @param       flags : Flags that dictates the argument behavior.
     * @param       regex_collection : Regular expressions that the values have to match. If the
     *              string is empty all values will match.
     * @param       error_id : The id used to reference an arguments during the error display.
     * @throw       kcs::argparse::arg_parser_exception : An exception is thrown if there is not one
     *              of the keys specified or if an argument with the same key already exists.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpString3_,
            typename TpString4_ = string_type,
            typename TpArgValueTypesVector_ = vector_type<arg_value_types>,
            typename TpStringVector_ = vector_type<string_type>
    >
    void add_foreign_arg(
            TpString1_&& usage_key,
            TpString2_&& help_key,
            TpString3_&& description,
            TpArgValueTypesVector_&& values_types = {arg_value_types::STRING},
            std::size_t min_values = 1,
            std::size_t max_values = 1,
            arg_flags flags = arg_flags::DEFAULT_FOREIGN_ARG_FLAGS,
            TpStringVector_&& regex_collection = vector_type<string_type>(),
            TpString4_&& error_id = string_type()
    )
    {
        if (kcs::stringutils::strlen(usage_key) == 0 || kcs::stringutils::strlen(help_key) == 0)
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there is no "
                                               "key specified.");
        }
    
        if (arg_key_exists(usage_key) || arg_key_exists(help_key))
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: the key is "
                                               "already used.");
        }
    
        allocator_type<foreign_arg_type> foreign_arg_type_alloc;
        foreign_arg_type* foreign_arg = foreign_arg_type_alloc.allocate(1);
        foreign_arg_type_alloc.construct(foreign_arg,
                                          std::forward<TpString3_>(description),
                                          std::forward<TpString4_>(error_id),
                                          flags,
                                          min_values,
                                          max_values,
                                          std::forward<TpArgValueTypesVector_>(values_types),
                                          std::forward<TpStringVector_>(regex_collection),
                                          usage_key,
                                          help_key);
        
        base_arg_map_.insert(std::make_pair(std::forward<TpString1_>(usage_key),
                                            static_cast<base_arg_type*>(foreign_arg)));
        base_arg_map_.insert(std::make_pair(std::forward<TpString2_>(help_key),
                                            static_cast<base_arg_type*>(foreign_arg)));
        
        ihelp_text_list_.push_back(foreign_arg);
        foreign_args_list_.push_back(foreign_arg);
    }
    
    /**
     * @brief       Add an argument that represents an argument without keys.
     * @param       add_foreign_arg_params : Object that contains all the parameters to call this
     *              methode.
     * @throw       kcs::argparse::arg_parser_exception : An exception is thrown if there is no 
     *              usage id specified or if an argument with the same usage id already exist.
     */
    void add_foreign_arg(add_foreign_arg_params& add_foreign_arg_params)
    {
        add_foreign_arg(std::move(add_foreign_arg_params.usage_key_),
                        std::move(add_foreign_arg_params.help_key_),
                        std::move(add_foreign_arg_params.description_),
                        std::move(add_foreign_arg_params.values_types_),
                        std::move(add_foreign_arg_params.min_values_),
                        std::move(add_foreign_arg_params.max_values_),
                        std::move(add_foreign_arg_params.flags_),
                        std::move(add_foreign_arg_params.regex_collection_),
                        std::move(add_foreign_arg_params.error_id_));
    }
    
    /**
     * @brief       Get a reference to a base argument.
     * @param       key : Id of the argument to get.
     * @return      If function was successful the argument found is returned.
     * @throw       kcs::argparse::arg_parser_exception : If there isn't any argument with the
     *              specified key an exception is thrown.
     */
    const base_arg_type& get_base_arg(const string_type& key) const
    {
        auto it = base_arg_map_.find(key);
        
        if (it != base_arg_map_.end())
        {
            return *(it->second);
        }
        
        throw arg_parser_exception("kcs::argparse::arg_parser_exception: there isn't any "
                                           "argument with the specified key");
    }
    
    /**
     * @brief       Get a reference to a key argument.
     * @param       key : Key of the argument to get.
     * @return      If function was successful the argument found is returned.
     * @throw       kcs::argparse::arg_parser_exception : If there isn't any argument with the
     *              specified key or the argument can't be cast to the specified type an exception
     *              is thrown.
     */
    const key_arg_type& get_key_arg(const string_type& key) const
    {
        auto it = base_arg_map_.find(key);
        
        if (it != base_arg_map_.end())
        {
            key_arg_type *key_arg = dynamic_cast<key_arg_type*>(it->second);
            if (key_arg == nullptr)
            {
                throw arg_parser_exception("kcs::argparse::arg_parser_exception: the argument c'ant"
                                                   " be cast to the specified type");
            }
            
            return *key_arg;
        }
    
        throw arg_parser_exception("kcs::argparse::arg_parser_exception: there isn't any "
                                           "argument with the specified key");
    }
    
    /**
     * @brief       Get a reference to a value argument.
     * @param       key : Key of the argument to get.
     * @return      If function was successful the argument found is returned.
     * @throw       kcs::argparse::arg_parser_exception : If there isn't any argument with the
     *              specified key or the argument can't be cast to the specified type an exception
     *              is thrown.
     */
    const value_arg_type& get_value_arg(const string_type& key) const
    {
        auto it = base_arg_map_.find(key);
        
        if (it != base_arg_map_.end())
        {
            value_arg_type *value_arg = dynamic_cast<value_arg_type*>(it->second);
            if (value_arg == nullptr)
            {
                throw arg_parser_exception("kcs::argparse::arg_parser_exception: the argument c'ant"
                                                   " be cast to the specified type");
            }
    
            return *value_arg;
        }
    
        throw arg_parser_exception("kcs::argparse::arg_parser_exception: there isn't any "
                                           "argument with the specified key");
    }
    
    /**
     * @brief       Get a reference to a key value argument.
     * @param       key : Key of the argument to get.
     * @return      If function was successful the argument found is returned.
     * @throw       kcs::argparse::arg_parser_exception : If there isn't any argument with the
     *              specified key or the argument can't be cast to the specified type an exception
     *              is thrown.
     */
    const key_value_arg_type& get_key_value_arg(const string_type& key) const
    {
        auto it = base_arg_map_.find(key);
        
        if (it != base_arg_map_.end())
        {
            key_value_arg_type *key_value_arg = dynamic_cast<key_value_arg_type*>(it->second);
            if (key_value_arg == nullptr)
            {
                throw arg_parser_exception("kcs::argparse::arg_parser_exception: the argument c'ant"
                                                   " be cast to the specified type");
            }
    
            return *key_value_arg;
        }
        
        throw arg_parser_exception("kcs::argparse::arg_parser_exception: there isn't any "
                                           "argument with the specified key");
    }
    
    /**
     * @brief       Get a reference to an foreign argument.
     * @param       key : Key of the argument to get.
     * @return      If function was successful the argument found is returned.
     * @throw       kcs::argparse::arg_parser_exception : If there isn't any argument with the
     *              specified key or the argument can't be cast to the specified type an exception
     *              is thrown.
     */
    const foreign_arg_type& get_foreign_arg(const string_type& key) const
    {
        auto it = base_arg_map_.find(key);
        
        if (it != base_arg_map_.end())
        {
            foreign_arg_type *foreign_arg = dynamic_cast<foreign_arg_type*>(it->second);
            if (foreign_arg == nullptr)
            {
                throw arg_parser_exception("kcs::argparse::arg_parser_exception: the argument c'ant"
                                                   " be cast to the specified type");
            }
    
            return *foreign_arg;
        }
    
        throw arg_parser_exception("kcs::argparse::arg_parser_exception: there isn't any "
                                           "argument with the specified key");
    }
    
    /**
     * @param       argc : Contains the number of arguments gotten through the program call.
     * @param       argv : Contains the arguments gotten through the program call.
     * @brief       Parse all the arguments added to the parser. It's used to associate arguments
     *              with the values from the program call.
     */
    template<typename TpStringArray_>
    void parse_args(std::size_t argc, TpStringArray_ argv)
    {
        string_type current_arg_value;
        base_arg_type *base_arg = nullptr;
        key_arg_type *key_arg = nullptr;
        key_value_arg_type *key_value_arg = nullptr;
        foreign_arg_type *foreign_arg = nullptr;
        foreign_arg_type *old_foreign_arg = nullptr;
        std::size_t pos_increment;
        vector_type<key_arg_type*> chained_args;
        std::pair<string_type, string_type> key_value_arg_pair;
    
        reset_args_parse();
        
        if (argc > 0 && program_name_.empty())
        {
            program_name_ = argv[0];
        }
        
        for (auto& x : base_arg_map_)
        {
            x.second->set_parsed(true);
        }
        
        for (std::size_t i = 1; i < argc; i++)
        {
            current_arg_value = argv[i];
            
            // Normal arguments
            if ((key_arg = get_key_arg_reference(current_arg_value)) != nullptr)
            {
                key_arg->set_found(true);
                key_value_arg = dynamic_cast<key_value_arg_type*>(key_arg);
                
                if (key_value_arg != nullptr)
                {
                    for (pos_increment = 1;
                         !key_value_arg->max_values_number_reached() &&
                                 i + pos_increment < argc &&
                                 !arg_key_exists(argv[i + pos_increment]);
                         pos_increment++)
                    {
                        key_value_arg->add_value(argv[i + pos_increment]);
                    }
                    
                    i += pos_increment - 1;
                }
            }
                
            // Chained arguments
            else if (try_get_chained_args(current_arg_value, &chained_args))
            {
                for (auto it = chained_args.begin(); it != chained_args.end(); it++)
                {
                    key_arg = *it;
                    key_arg->set_found(true);
                    key_value_arg = dynamic_cast<key_value_arg_type*>(key_arg);
                    
                    if (key_value_arg != nullptr)
                    {
                        for (pos_increment = 1;
                             !key_value_arg->max_values_number_reached() &&
                                     i + pos_increment < argc &&
                                     !arg_key_exists(argv[i + pos_increment]) /*&&
                                     *it == chained_args.back()*/;
                             pos_increment++)
                        {
                            key_value_arg->add_value(argv[i + pos_increment]);
                        }
                        
                        i += pos_increment - 1;
                    }
                }
            }
                
            // EQ arguments
            else if (try_get_current_arg_value_splitted(current_arg_value, &key_value_arg_pair) &&
                     arg_key_with_flags_exists(key_value_arg_pair.first, arg_flags::ALLOW_EQ) &&
                     (key_value_arg = get_key_value_arg_reference(key_value_arg_pair.first)) !=
                             nullptr)
            {
                key_value_arg->set_found(true);
                key_value_arg->add_value(key_value_arg_pair.second);
            }
                
            // Foreign values
            else if (try_add_value_to_foreign_arg(current_arg_value, &foreign_arg))
            {
                if (old_foreign_arg != foreign_arg)
                {
                    foreign_arg->set_found(true);
                }
                
                old_foreign_arg = foreign_arg;
    
                for (pos_increment = 1; i + pos_increment < argc; pos_increment++)
                {
                    if (!try_add_value_to_foreign_arg(argv[i + pos_increment], &foreign_arg))
                    {
                        break;
                    }
                }
                
                i += pos_increment - 1;
            }
            
            // Unrecognized arguments
            else if (unrecognized_args_.size() + 1 < unrecognized_args_.max_size() &&
                     unrecognized_args_.size() < max_unrecognized_args_)
            {
                unrecognized_args_.push_back(std::move(current_arg_value));
            }
        }
        
        // Print Help if necessary
        if ((help_arg_found() && flags_.flag_is_raised(arg_parser_flags::PRINT_HELP)))
        {
            print_help();
        }
        
        // Print Version if necessary
        if (version_arg_found() && flags_.flag_is_raised(arg_parser_flags::PRINT_VERSION))
        {
            print_version();
        }
        
        // Raise argument parser flags if there are errors.
        update_error_flags();
        
        // Print argument parser errors if necessary
        if (flags_.flag_is_raised(arg_parser_flags::PRINT_ERRORS) && !error_flags_.is_empty())
        {
            print_errors();
        }
    }
    
    /**
     * @brief       Reset the arguments parse.
     */
    void reset_args_parse() noexcept
    {
        value_arg_type *value_arg;
        
        for (auto& x : base_arg_map_)
        {
            x.second->set_parsed(false);
            x.second->set_found(false);
            
            if ((value_arg = dynamic_cast<value_arg_type*>(x.second)) != nullptr)
            {
                value_arg->clear_values();
            }
        }
        
        unrecognized_args_.clear();
    
        update_error_flags();
    }
    
    /**
     * @brief       Allows knowing wether an argument has been found in the program call.
     * @param       key : Argument key to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool arg_found(const string_type& key) const noexcept
    {
        base_arg_type *base_arg = get_base_arg_reference(key);
        return (base_arg != nullptr && base_arg->check_key(key) && base_arg->was_found());
    }
    
    /**
     * @brief       Allows knowing wether the argument that represents the option to get the help
     *              information has been found in the program call.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool help_arg_found() const noexcept
    {
        return (current_help_arg_ != nullptr && current_help_arg_->was_found());
    }
    
    /**
     * @brief       Allows knowing wether the argument that represents the option to get the version
     *              information has been found in the program call.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool version_arg_found() const noexcept
    {
        return (current_version_arg_ != nullptr && current_version_arg_->was_found());
    }
    
    /**
     * @brief       Get the first argument value with the specified key.
     * @param       key : Key of the argument to get the value.
     * @return      The argument first value found.
     * @throw       kcs::argparse::arg_parser_exception : If there isn't any argument with the
     *              specified key an exception is thrown.
     * @throw       std::out_of_range : If argument hasn't any value an exception is thrown.
     */
    const arg_value_type& get_front_arg_value(const string_type& key) const
    {
        value_arg_type *value_arg = get_value_arg_reference(key);
        
        if (value_arg == nullptr)
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there isn't any "
                                               "argument with the specified key");
        }
        
        return value_arg->get_front_value();
    }
    
    /**
     * @brief       Get the first argument value with the specified key.
     * @param       key : Key of the argument to get the value.
     * @param       default_value : The value used to construct an argument value if there isn't any
     *              value.
     * @return      If function was successful the first argument value is returned, otherwise an
     *              argument value constructed with the default value is returned.
     * @throw       kcs::argparse::arg_parser_exception : If there isn't any argument with the
     *              specified key an exception is thrown.
     */
    template<typename TpString_>
    const arg_value_type& get_front_arg_value(
            const string_type& key,
            TpString_&& default_value
    ) const
    {
        value_arg_type *value_arg = get_value_arg_reference(key);
        
        if (value_arg == nullptr)
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there isn't any "
                                               "argument with the specified key");
        }
        
        return value_arg->get_front_value(std::forward<TpString_>(default_value));
    }
    
    /**
     * @brief       Get the 'n' argument value with the specified key.
     * @param       key : Key of the argument to get the value.
     * @param       index : Position of the element.
     * @return      The element specified.
     * @throw       kcs::argparse::arg_parser_exception : If there isn't any argument with the
     *              specified key an exception is thrown.
     * @throw       std::out_of_range : If there isn't any element in the specified position an
     *              exception is thrown.
     */
    const arg_value_type& get_arg_value_at(
            const string_type& key,
            std::size_t index
    ) const
    {
        value_arg_type *value_arg = get_value_arg_reference(key);
        
        if (value_arg == nullptr)
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there isn't any "
                                               "argument with the specified key");
        }
        
        return value_arg->get_value_at(index);
    }
    
    /**
     * @brief       Get the 'n' argument value with the specified key.
     * @param       key : Key of the argument to get the value.
     * @param       index : Position of the element.
     * @param       default_value : The value used to construct an argument value if there isn't any
     *              value.
     * @return      If function was successful the argument value in the specified position is
     *              returned, otherwise an argument value constructed with the default value is
     *              returned.
     * @throw       kcs::argparse::arg_parser_exception : If there isn't any argument with the
     *              specified key an exception is thrown.
     */
    template<typename TpString_>
    const arg_value_type& get_arg_value_at(
            const string_type& key,
            std::size_t index,
            TpString_&& default_value
    ) const
    {
        value_arg_type *value_arg = get_value_arg_reference(key);
        
        if (value_arg == nullptr)
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there isn't any "
                                               "argument with the specified key");
        }
        
        return value_arg->get_value_at(index, std::forward<TpString_>(default_value));
    }
    
    /**
     * @brief       Get all the argument values with the specified key.
     * @param       key : Key of the argument to get the values.
     * @return      All arguments values found.
     * @throw       kcs::argparse::arg_parser_exception : If there isn't any argument with the
     *              specified key or the argument doesn't has any value an exception is thrown.
     */
    const vector_type<arg_value_type>& get_arg_values(const string_type& key) const
    {
        value_arg_type *value_arg = get_value_arg_reference(key);
        
        if (value_arg == nullptr)
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there isn't "
                                               "any argument with the specified key");
        }
        
        return value_arg->get_values();
    }
    
    /**
     * @brief       Get all the argument values with the specified key.
     * @param       key : Key of the argument to get the values.
     * @return      All arguments values found.
     * @throw       kcs::argparse::arg_parser_exception : If there isn't any argument with the
     *              specified key or the argument doesn't has any value an exception is thrown.
     * @throw       kcs::type_casting::bad_type_cast : If no conversion could be performed, an
     *              exception is thrown.
     */
    template<typename T>
    const vector_type<T> get_arg_values_as(const string_type& key) const
    {
        value_arg_type *value_arg = get_value_arg_reference(key);
        
        if (value_arg == nullptr)
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there isn't "
                                               "any argument with the specified key");
        }
        
        vector_type<T> result;
        for (auto& x : value_arg->get_values())
        {
            result.push_back(x.template as<T>());
        }
        
        return result;
    }
    
    /**
     * @brief       Get all the argument values with the specified key.
     * @param       key : Key of the argument to get the values.
     * @return      All arguments values found.
     * @throw       kcs::argparse::arg_parser_exception : If there isn't any argument with the
     *              specified key or the argument doesn't has any value an exception is thrown.
     */
    template<typename T, typename TpString_>
    const vector_type<T> get_arg_values_as(const string_type& key, TpString_&& default_value) const
    {
        value_arg_type *value_arg = get_value_arg_reference(key);
        
        if (value_arg == nullptr)
        {
            throw arg_parser_exception("kcs::argparse::arg_parser_exception: there isn't "
                                               "any argument with the specified key");
        }
        
        vector_type<T> result;
        for (auto& x : value_arg->get_values())
        {
            result.push_back(x.template as<T>(std::forward<TpString_>(default_value)));
        }
        
        return result;
    }
    
    /**
     * @brief       Allows knowing wether a flag is raised in the argument parser.
     * @param       flag : Flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool flag_is_raised(arg_parser_flags flag) const noexcept
    {
        return flags_.flag_is_raised(flag);
    }
    
    /**
     * @brief       Allows knowing wether a error flag is set in the argument parser.
     * @param       flag : Flag to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool error_flag_is_raised(arg_parser_error_flags flag) const noexcept
    {
        return error_flags_.flag_is_raised(flag);
    }
    
    /**
     * @brief       Allows knowing wether there are flags errors in the argument parser.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool there_are_errors() const noexcept
    {
        return !error_flags_.is_empty();
    }
    
    /**
     * @brief       Print the help information in standard output.
     */
    void print_help() const
    {
        auto& os = kcs::iostream::get_cout<TpChar>();
        base_arg_type *base_arg;
        std::size_t n_key_args = 0;
        std::size_t short_id_length = 0;
        std::size_t long_id_length = 0;
        std::size_t current_id_length;
    
        if (flags_.flag_is_raised(arg_parser_flags::PRINT_ARGS_ID_ON_PRINT_HELP))
        {
            for (auto& x : base_arg_map_)
            {
                current_id_length = x.second->get_short_keys_length();
                if (short_id_length < current_id_length)
                {
                    short_id_length = current_id_length;
                }
            
                current_id_length = x.second->get_long_keys_length();
                if (long_id_length < current_id_length)
                {
                    long_id_length = current_id_length;
                }
            }
        }
        
        if (flags_.flag_is_raised(arg_parser_flags::PRINT_USAGE_ON_PRINT_HELP))
        {
            for (auto& x : ihelp_text_list_)
            {
                if (dynamic_cast<key_arg_type*>(x) != nullptr &&
                    dynamic_cast<help_arg_type*>(x) == nullptr &&
                    dynamic_cast<version_arg_type*>(x) == nullptr)
                {
                    n_key_args++;
                    
                    if (n_key_args > 1)
                    {
                        break;
                    }
                }
            }
            
            if ((n_key_args == 0 && current_help_arg_ != nullptr) ||
                (n_key_args == 0 && current_version_arg_ != nullptr))
            {
                n_key_args = 1;
            }
            
            os << "Usage: " << program_name_;
            
            if (n_key_args >= 1)
            {
                os << " [OPTION]";
                
                if (n_key_args > 1)
                {
                    os << "...";
                }
            }
            
            for (auto& x : foreign_args_list_)
            {
                x->print_usage_key();
            }
            
            os << std::endl;
        }
        
        for (auto& x : ihelp_text_list_)
        {
            if (dynamic_cast<help_text_type*>(x) != nullptr ||
                !flags_.flag_is_raised(arg_parser_flags::PRINT_ARGS_ID_ON_PRINT_HELP))
            {
                x->print_help_text(max_description_line_length_,
                                   description_newline_indentation_,
                                   0);
            }
            else if ((base_arg = dynamic_cast<base_arg_type*>(x)) != nullptr)
            {
                base_arg->print_help_text(arg_description_indentation_,
                                          max_description_line_length_,
                                          description_newline_indentation_,
                                          short_id_length,
                                          long_id_length);
            }
        }
        
        if (flags_.flag_is_raised(arg_parser_flags::EXIT_ON_PRINT_HELP))
        {
            exit_program(0);
        }
    }
    
    /**
     * @brief       Print the version information in standard output.
     */
    void print_version() const
    {
        if (current_version_arg_ != nullptr)
        {
            current_version_arg_->print_version_information();
            kcs::iostream::get_cout<char_type >() << std::endl;
        }
        
        if (flags_.flag_is_raised(arg_parser_flags::EXIT_ON_PRINT_VERSION))
        {
            exit_program(0);
        }
    }
    
    /**
     * @brief       Print the argument parser errors in standard output.
     */
    void print_errors() const
    {
        if (!error_flags_.is_empty())
        {
            auto& os = kcs::iostream::get_cout<char_type >();
            base_arg_type *base_arg;
            
            if (error_flags_.flag_is_raised(arg_parser_error_flags::ARGS_ERROR))
            {
                for (auto& x : ihelp_text_list_)
                {
                    base_arg = dynamic_cast<base_arg_type*>(x);
                    if (base_arg != nullptr && base_arg->there_are_errors())
                    {
                        base_arg->print_errors(program_name_, flags_.flag_is_raised(
                                arg_parser_flags::USE_COLORS_ON_PRINT_ERRORS));
                    }
                }
            }
            
            if (error_flags_.flag_is_raised(arg_parser_error_flags::UNRECOGNIZED_ARGS_ERROR))
            {
                for (auto& x : unrecognized_args_)
                {
                    os << program_name_ << ": ";
                    if (!error_id_.empty())
                    {
                        if (flags_.flag_is_raised(arg_parser_flags::USE_COLORS_ON_PRINT_ERRORS))
                        {
                            kcs::system::set_ostream_text_attribute(
                                    os, kcs::system::text_attribute::LIGHT_RED);
                            os << error_id_ << ": ";
                            kcs::system::set_ostream_text_attribute(
                                    os, kcs::system::text_attribute::DEFAULT);
                        }
                        else
                        {
                            os << error_id_ << ": ";
                        }
                    }
                    os << "Unrecognized option '" << x << "'" << std::endl;
                }
            }
            
            if (current_help_arg_ != nullptr &&
                flags_.flag_is_raised(arg_parser_flags::PRINT_TRY_HELP_ON_ERRORS))
            {
                os << "Try '" << program_name_ << " " << current_help_arg_->get_front_key()
                   << "' for more information." << std::endl;
            }
            
            if (flags_.flag_is_raised(arg_parser_flags::PRINT_HELP_ON_ERRORS) &&
                error_flags_.flag_is_raised(arg_parser_error_flags::ARGS_ERROR))
            {
                print_help();
            }
            
            if (flags_.flag_is_raised(arg_parser_flags::EXIT_ON_PRINT_ARGS_ERRORS))
            {
                exit_program(-1);
            }
        }
    }
    
    /**
     * @brief       Allows throwing an error to the user using the arguent parser and its syntax.
     *              This method has been made for particular occasions in which an argument error is
     *              not compatible with the argument parser.
     * @param       error_message : The message to print in the standard output.
     * @param       arg_key : The key of the argument to associate with the
     *              error.
     */
    template<typename TpString_>
    void print_error_message(
            const TpString_& error_message,
            const string_type& arg_key = string_type()
    ) const
    {
        auto& os = kcs::iostream::get_cout<TpChar>();
        base_arg_type *base_arg = get_base_arg_reference(arg_key);
        
        if (base_arg != nullptr)
        {
            base_arg->print_error_message(error_message, program_name_, flags_.flag_is_raised(
                    arg_parser_flags::USE_COLORS_ON_PRINT_ERRORS));
        }
        else
        {
            os << program_name_ << ": " << error_message << std::endl;
        }
        
        if (current_help_arg_ != nullptr &&
            flags_.flag_is_raised(arg_parser_flags::PRINT_TRY_HELP_ON_ERRORS))
        {
            os << "Try '" << program_name_ << " " << current_help_arg_->get_front_key()
               << "' for more information." << std::endl;
        }
        
        if (flags_.flag_is_raised(arg_parser_flags::PRINT_HELP_ON_ERRORS) &&
            error_flags_.flag_is_raised(arg_parser_error_flags::ARGS_ERROR))
        {
            print_help();
        }
        
        if (flags_.flag_is_raised(arg_parser_flags::EXIT_ON_PRINT_ARGS_ERRORS))
        {
            exit_program(-1);
        }
    }

private:
    /**
     * @brief       Local destruction of the object.
     */
    void local_destructor() noexcept
    {
        for (auto& x : ihelp_text_list_)
        {
            delete_ihelp_text(x);
        }
        
        program_name_.clear();
        short_prefixes_.clear();
        long_prefixes_.clear();
        ihelp_text_list_.clear();
        current_help_arg_ = nullptr;
        current_version_arg_ = nullptr;
        foreign_args_list_.clear();
        base_arg_map_.clear();
        unrecognized_args_.clear();
        flags_.clear();
        error_flags_.clear();
    }
    
    /**
     * @brief       Set error flags if it's required.
     */
    void update_error_flags() noexcept
    {
        bool in = false;
        
        for (auto& x : base_arg_map_)
        {
            
            x.second->update_error_flags();
            if (x.second->there_are_errors())
            {
                in = true;
                error_flags_.raise_flag(arg_parser_error_flags::ARGS_ERROR);
            }
        }
        if (!in)
        {
            error_flags_.erase_flag(arg_parser_error_flags::ARGS_ERROR);
        }
        
        if (!unrecognized_args_.empty())
        {
            error_flags_.raise_flag(arg_parser_error_flags::UNRECOGNIZED_ARGS_ERROR);
        }
        else
        {
            error_flags_.erase_flag(arg_parser_error_flags::UNRECOGNIZED_ARGS_ERROR);
        }
    }
    
    /**
     * @brief       Allows knowing wether an argument key has been added to the parser.
     * @param       key : Argument key to check.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool arg_key_exists(const string_type& key) const noexcept
    {
        return base_arg_map_.count(key) > 0;
    }
    
    /**
     * @brief       Allows knowing wether an argument l with specefic flags has been added to the
     *              parser.
     * @param       key : Argument key to check.
     * @param       flag : Flag that the argument found must has raised.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool arg_key_with_flags_exists(const string_type& key, arg_flags flag) const noexcept
    {
        auto it = base_arg_map_.find(key);
    
        if (it != base_arg_map_.end())
        {
            if (it->second->check_key(key) &&
                it->second->flag_is_raised(flag) &&
                dynamic_cast<key_arg_type*>(it->second) != nullptr)
            {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * @brief       Get a reference to a base argument.
     * @param       key : Id of the argument to get.
     * @return      If function was successful a reference to the argument found is returned,
     *              otherwise a null pointer is returned.
     */
    base_arg_type* get_base_arg_reference(const string_type& key) const noexcept
    {
        auto it = base_arg_map_.find(key);
    
        if (it != base_arg_map_.end())
        {
            return it->second;
        }
    
        return nullptr;
    }
    
    /**
     * @brief       Get a reference to a key argument.
     * @param       key : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    key_arg_type* get_key_arg_reference(const string_type& key) const noexcept
    {
        auto it = base_arg_map_.find(key);
    
        if (it != base_arg_map_.end())
        {
            return dynamic_cast<key_arg_type*>(it->second);
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Get a reference to a value argument.
     * @param       key : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    value_arg_type* get_value_arg_reference(const string_type& key) const noexcept
    {
        auto it = base_arg_map_.find(key);
        
        if (it != base_arg_map_.end())
        {
            return dynamic_cast<value_arg_type*>(it->second);
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Get a reference to a key value argument.
     * @param       key : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    key_value_arg_type* get_key_value_arg_reference(const string_type& key) const noexcept
    {
        auto it = base_arg_map_.find(key);
    
        if (it != base_arg_map_.end())
        {
            return dynamic_cast<key_value_arg_type*>(it->second);
        }
        
        return nullptr;
    }
    
    /**
     * @brief       Get a reference to an foreign argument.
     * @param       key : Key of the argument to get.
     * @return      If function was successful a reference to the argument found is returned, if not
     *              a null pointer is returned.
     */
    foreign_arg_type* get_foreign_arg_reference(const string_type& key) const noexcept
    {
        auto it = base_arg_map_.find(key);
    
        if (it != base_arg_map_.end())
        {
            return dynamic_cast<foreign_arg_type*>(it->second);
        }
    
        return nullptr;
    }
    
    /**
     * @brief       Get a deep copy to the arguments container.
     * @return      A deep copy of the arguments container.
     */
    vector_type<ihelp_text_type*> get_args_list_deep_copy() const
    {
        vector_type<ihelp_text_type*> new_ihelp_text_list;
        void* p;
        ihelp_text_type* ihelp_text = nullptr;
        help_text_type* help_text;
        key_arg_type* key_arg;
        help_arg_type* help_arg;
        version_arg_type* version_arg;
        key_value_arg_type* key_value_arg;
        foreign_arg_type* foreign_arg;
        allocator_type<help_text_type> help_text_type_alloc;
        allocator_type<key_arg_type> key_arg_type_alloc;
        allocator_type<help_arg_type> help_arg_type_alloc;
        allocator_type<version_arg_type> version_arg_type_alloc;
        allocator_type<key_value_arg_type> key_value_arg_type_alloc;
        allocator_type<foreign_arg_type> foreign_arg_type_alloc;
        
        for (auto& x : ihelp_text_list_)
        {
            help_text = dynamic_cast<help_text_type*>(x);
            key_arg = dynamic_cast<key_arg_type*>(x);
            help_arg = dynamic_cast<help_arg_type*>(x);
            version_arg = dynamic_cast<version_arg_type*>(x);
            key_value_arg = dynamic_cast<key_value_arg_type*>(x);
            foreign_arg = dynamic_cast<foreign_arg_type*>(x);
            
            if (foreign_arg != nullptr)
            {
                p = foreign_arg_type_alloc.allocate(1);
                foreign_arg_type_alloc.construct((foreign_arg_type*)p, *foreign_arg);
                ihelp_text = (foreign_arg_type*)p;
            }
            else if (key_value_arg != nullptr)
            {
                p = key_value_arg_type_alloc.allocate(1);
                key_value_arg_type_alloc.construct((key_value_arg_type*)p, *key_value_arg);
                ihelp_text = (key_value_arg_type*)p;
            }
            else if (version_arg != nullptr)
            {
                p = version_arg_type_alloc.allocate(1);
                version_arg_type_alloc.construct((version_arg_type*)p, *version_arg);
                ihelp_text = (version_arg_type*)p;
            }
            else if (help_arg != nullptr)
            {
                p = help_arg_type_alloc.allocate(1);
                help_arg_type_alloc.construct((help_arg_type*)p, *help_arg);
                ihelp_text = (help_arg_type*)p;
            }
            else if (key_arg != nullptr)
            {
                p = key_arg_type_alloc.allocate(1);
                key_arg_type_alloc.construct((key_arg_type*)p, *key_arg);
                ihelp_text = (key_arg_type*)p;
            }
            else if (help_text != nullptr)
            {
                p = help_text_type_alloc.allocate(1);
                help_text_type_alloc.construct((help_text_type*)p, *help_text);
                ihelp_text = (help_text_type*)p;
            }
            
            if (ihelp_text != nullptr)
            {
                new_ihelp_text_list.push_back(ihelp_text);
            }
            
            ihelp_text = nullptr;
        }
        
        return new_ihelp_text_list;
    }
    
    /**
     * @brief       Get the prefix of the specified key.
     * @param       key : String that contains the prefix.
     * @return      If function was successful the prefix is returned, if not an empty string is
     *              returned.
     */
    string_type get_key_prefix(const string_type& key) const
    {
        string_type prefix_builder;
        string_type short_prefix;
        string_type long_prefix;
        
        for (auto& x : key)
        {
            prefix_builder += x;
            
            if (short_prefixes_.count(prefix_builder) > 0)
            {
                short_prefix = prefix_builder;
            }
    
            if (long_prefixes_.count(prefix_builder) > 0)
            {
                long_prefix = prefix_builder;
            }
        }
        
        if (long_prefix.length() >= short_prefix.length())
        {
            return long_prefix;
        }
        
        return short_prefix;
    }
    
    /**
     * @brief       Allows knowing wether a key prefix is long.
     * @param       key : String that contains the prefix
     * @return      If function was successful true is returned, if not false is returned.
     */
    bool is_key_prefix_long(const string_type& key) const
    {
        string_type prefix_builder;
        string_type short_prefix;
        string_type long_prefix;
    
        for (auto& x : key)
        {
            prefix_builder += x;
        
            if (short_prefixes_.count(prefix_builder) > 0)
            {
                short_prefix = prefix_builder;
            }
        
            if (long_prefixes_.count(prefix_builder) > 0)
            {
                long_prefix = prefix_builder;
            }
        }
    
        if (long_prefix.length() > short_prefix.length())
        {
            return true;
        }
    
        return false;
    }
    
    /**
     * @brief       Get an arg keys list from a strings list.
     * @param       keys : The list of string keys.
     * @return      A list of arg keys.
     */
    vector_type<arg_key_type> get_arg_key_list_from_strings(const vector_type<string_type>& keys)
    {
        bool is_long;
        vector_type<arg_key_type> arg_key_list;
        
        for (auto& key : keys)
        {
            is_long = is_key_prefix_long(key);
            arg_key_list.push_back(arg_key_type(key, is_long));
        }
        
        return arg_key_list;
    }
    
    /**
     * @brief       Get an argument collection composed by the chained arguments specified in a
     *              string.
     * @param       key : String that contains the chained arguments.
     * @param       chained_args : The collection in which the chained arguments will be set.
     * @return      If function was successful a collection with the references of the chained
     *              arguments is returned, if not an empty collection is returned.
     */
    bool try_get_chained_args(
            const string_type& key,
            vector_type<key_arg_type*> *chained_args
    ) const
    {
        string_type key_prefix = get_key_prefix(key);
        string_type key_builder;
        key_arg_type *key_arg;
        chained_args->clear();
        
        for (auto i = key_prefix.size(); i < key.size(); i++)
        {
            key_builder = key_prefix;
            key_builder += key[i];
            
            key_arg = get_key_arg_reference(key_builder);
            
            if (key_arg != nullptr && key_arg->flag_is_raised(arg_flags::ALLOW_CHAIN))
            {
                chained_args->push_back(key_arg);
            }
            else
            {
                return false;
            }
        }
        
        return !chained_args->empty();
    }
    
    /**
     * @brief       Get the current argument value splitted by the equal operator.
     * @param       current_arg_value : Argument currently parsed.
     * @param       key_value_arg_pair : The result of the split.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    bool try_get_current_arg_value_splitted(
            const string_type& current_arg_value,
            std::pair<string_type, string_type> *key_value_arg_pair
    ) const
    {
        std::size_t eq_pos;
        
        if (current_arg_value.size() > 2)
        {
            eq_pos = current_arg_value.find('=', 1);
            if (eq_pos != string_type::npos)
            {
                key_value_arg_pair->second = current_arg_value.substr(eq_pos + 1);
                if (!key_value_arg_pair->second.empty())
                {
                    key_value_arg_pair->first = current_arg_value.substr(0, eq_pos);
                    return true;
                }
            }
        }
        
        return false;
    }
    
    /**
     * @brief       Try to add a value to the next foreign argument that accept this value.
     * @param       current_arg_value : The foreign argument value.
     * @param       foreign_arg : The pointer to get the argument.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    template<typename TpString_>
    bool try_add_value_to_foreign_arg(
            TpString_&& current_arg_value,
            foreign_arg_type **foreign_arg
    ) const
    {
        *foreign_arg = nullptr;
        
        if (arg_key_exists(current_arg_value) ||
            (flags_.flag_is_raised(
                    arg_parser_flags::FOREIGN_VALUE_ARG_CANNOT_BE_ADDED_WITH_PREFIX) &&
                    !get_key_prefix(current_arg_value).empty()))
        {
            return false;
        }
        
        for (auto& x : foreign_args_list_)
        {
            if (x->try_add_value(current_arg_value))
            {
                *foreign_arg = x;
                return true;
            }
        }
        
        for (auto& x : foreign_args_list_)
        {
            if (!x->max_values_number_reached())
            {
                x->add_value(std::forward<TpString_>(current_arg_value));
                *foreign_arg = x;
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * @brief       Destroy and deallocate the specified argument.
     * @param       arg : The specified argument.
     */
    inline void delete_ihelp_text(ihelp_text_type *&arg) noexcept
    {
        help_text_type *heltp_text;
        key_arg_type *key_arg;
        help_arg_type *help_arg;
        version_arg_type *version_arg;
        key_value_arg_type *key_value_arg;
        foreign_arg_type *foreign_arg;
        
        if ((foreign_arg = dynamic_cast<foreign_arg_type*>(arg)) != nullptr)
        {
            allocator_type<foreign_arg_type> foreign_arg_type_alloc;
            foreign_arg_type_alloc.destroy(foreign_arg);
            foreign_arg_type_alloc.deallocate(foreign_arg, 1);
        }
        else if ((key_value_arg = dynamic_cast<key_value_arg_type*>(arg)) != nullptr)
        {
            allocator_type<key_value_arg_type> key_value_arg_type_alloc;
            key_value_arg_type_alloc.destroy(key_value_arg);
            key_value_arg_type_alloc.deallocate(key_value_arg, 1);
        }
        else if ((version_arg = dynamic_cast<version_arg_type*>(arg)) != nullptr)
        {
            allocator_type<version_arg_type> version_arg_type_alloc;
            version_arg_type_alloc.destroy(version_arg);
            version_arg_type_alloc.deallocate(version_arg, 1);
        }
        else if ((help_arg = dynamic_cast<help_arg_type*>(arg)) != nullptr)
        {
            allocator_type<help_arg_type> help_arg_type_alloc;
            help_arg_type_alloc.destroy(help_arg);
            help_arg_type_alloc.deallocate(help_arg, 1);
        }
        else if ((key_arg = dynamic_cast<key_arg_type*>(arg)) != nullptr)
        {
            allocator_type<key_arg_type> key_arg_type_alloc;
            key_arg_type_alloc.destroy(key_arg);
            key_arg_type_alloc.deallocate(key_arg, 1);
        }
        else if ((heltp_text = dynamic_cast<help_text_type*>(arg)) != nullptr)
        {
            allocator_type<help_text_type> help_text_type_alloc;
            help_text_type_alloc.destroy(heltp_text);
            help_text_type_alloc.deallocate(heltp_text, 1);
        }
        
        arg = nullptr;
    }
    
    /**
     * @brief       Exit the program with the value specified.
     * @param       value : Value that the program will return.
     */
    inline void exit_program(int value) const noexcept
    {
        this->~basic_arg_parser();
        exit(value);
    }

private:
    /** Contains the program name that use the argument parser. */
    string_type program_name_;
    
    /** Collection that contains the short prefixes arguments. The default short prefix is '-' */
    unordered_set_type<string_type> short_prefixes_;
    
    /** Collection that contains the long prefixes arguments. The default long prefix is '--' */
    unordered_set_type<string_type> long_prefixes_;
    
    /** Indentation used to print arguments help description. */
    std::size_t arg_description_indentation_;
    
    /** The maximum description length that will be printed in a single line. */
    std::size_t max_description_line_length_;
    
    /** The indentation used when a newline is found in a description. */
    std::size_t description_newline_indentation_;
    
    /** Collection that contains all the arguments. */
    vector_type<ihelp_text_type*> ihelp_text_list_;
    
    /** Contains all the help argument. */
    help_arg_type* current_help_arg_;
    
    /** Contains all the version argument. */
    version_arg_type* current_version_arg_;
    
    /** Collection that contains all the foreign arguments. */
    vector_type<foreign_arg_type*> foreign_args_list_;
    
    /** Collection used to allow O(1) access on argument keys. */
    unordered_map_type<string_type, base_arg_type*> base_arg_map_;
    
    /** Error id that will be used for global errors. */
    string_type error_id_;
    
    /** Contains the unrecognized argument if an error happen. */
    vector_type<string_type> unrecognized_args_;
    
    /** Contains the maximum number of unrecognized args to be catched. */
    std::size_t max_unrecognized_args_;
    
    /** Flags that dictates the argument parser behavior. */
    flags_container_type<arg_parser_flags> flags_;
    
    /** Error flags that allows knowing wether there are errors. */
    flags_container_type<arg_parser_error_flags> error_flags_;
};


/** Class used to parse arguments based on 8 bit characters. */
using arg_parser = basic_arg_parser<char>;


/** Class used to parse arguments based on 16 bit characters. */
using warg_parser = basic_arg_parser<wchar_t>;
    
    
}
}


#endif
