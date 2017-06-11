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
 * @file        kcs/argparse/basic_value_arg.hpp
 * @brief       basic_value_arg class header.
 * @author      Killian
 * @date        2016/03/10 - 16:26
 */

#ifndef KCS_ARGPARSE_BASIC_VALUE_ARG_HPP
#define KCS_ARGPARSE_BASIC_VALUE_ARG_HPP

#include <vector>

#include "arg_parser_exception.hpp"
#include "basic_arg_value.hpp"
#include "basic_base_arg.hpp"


namespace kcs {
namespace argparse {


/**
 * @brief       Class that represents arguments that have values.
 */
template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAlloc = std::allocator<int>
>
class basic_value_arg : public virtual basic_base_arg<TpChar, TpCharTraits, TpAlloc>
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
     * @param       values_types : Collection that has the values types that must have the argument
     *              values.
     * @param       regex_collection : Regular expressions that the values have to match. If the
     *              vector is empty all values will match.
     */
    template<
            typename TpString1_,
            typename TpString2_,
            typename TpArgValueTypesVector_ = vector_type<arg_value_types>,
            typename TpStringVector_ = vector_type<string_type>
    >
    basic_value_arg(
            TpString1_&& description,
            TpString2_&& error_id,
            arg_flags flags,
            std::size_t min_values,
            std::size_t max_values,
            TpArgValueTypesVector_&& values_types,
            TpStringVector_&& regex_collection
    )
            : base_arg_type(std::forward<TpString1_>(description),
                            std::forward<TpString2_>(error_id), flags)
            , values_()
            , min_values_(min_values)
            , max_values_(max_values)
            , values_types_(std::forward<TpArgValueTypesVector_>(values_types))
            , regex_collection_(std::forward<TpStringVector_>(regex_collection))
    {
        if (min_values_ > max_values_)
        {
            max_values_ = min_values_;
        }
        if (base_arg_type::flag_is_raised(arg_flags::ALLOW_CHAIN) && (min_values_ != max_values_))
        {
            base_arg_type::erase_flag(arg_flags::ALLOW_CHAIN);
        }
        if (base_arg_type::flag_is_raised(arg_flags::ALLOW_EQ) && max_values_ != 1)
        {
            base_arg_type::erase_flag(arg_flags::ALLOW_EQ);
        }
    
        update_error_flags();
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : Object to copy.
     */
    basic_value_arg(const basic_value_arg& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : Object to move.
     */
    basic_value_arg(basic_value_arg&& rhs) noexcept
            : base_arg_type(std::move(rhs))
            , values_(std::move(rhs.values_))
            , min_values_(rhs.min_values_)
            , max_values_(rhs.max_values_)
            , values_types_(std::move(rhs.values_types_))
            , regex_collection_(std::move(rhs.regex_collection_))
    {
        rhs.min_values_ = 0;
        rhs.max_values_ = 0;
    }
    
    /**
     * @brief       Destructor.
     */
    ~basic_value_arg() = default;
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_value_arg& operator =(const basic_value_arg& rhs) = default;
    
    /**
     * @brief       Move assignment operator.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_value_arg& operator =(basic_value_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            base_arg_type::operator =(std::move(rhs));
            values_ = std::move(rhs.values_);
            std::swap(min_values_, rhs.min_values_);
            std::swap(max_values_, rhs.max_values_);
            values_types_ = std::move(rhs.values_types_);
            regex_collection_ = std::move(rhs.regex_collection_);
        }
        
        return *this;
    }
    
    /**
     * @brief       Get values collection.
     * @return      The values argument collection.
     */
    inline const vector_type<arg_value_type>& get_values() const noexcept
    {
        return values_;
    }
    
    /**
     * @brief       Get minimum number of values for an option.
     * @return      The minimum number of values for an option.
     */
    inline std::size_t get_min_values() const noexcept
    {
        return min_values_;
    }
    
    /**
     * @brief       Get maximum number of values for an option.
     * @return      The maximum number of values for an option.
     */
    inline std::size_t get_max_values() const noexcept
    {
        return max_values_;
    }
    
    /**
     * @brief       Add a value to the argument.
     * @param       value : Value to add to the arguement.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    template<typename TpString_>
    bool add_value(TpString_&& value)
    {
        bool success = true;
        arg_value_types values_types;
        string_type regex_to_match;
        
        if (!max_values_number_reached())
        {
            if (values_.size() + 1 <= values_types_.size())
            {
                values_types = values_types_[values_.size()];
            }
            else if (!values_types_.empty())
            {
                values_types = values_types_.back();
            }
            else
            {
                values_types = arg_value_types::STRING;
            }
            
            if (values_.size() + 1 <= regex_collection_.size())
            {
                regex_to_match = regex_collection_[values_.size()];
            }
            else if (!regex_collection_.empty())
            {
                regex_to_match = regex_collection_.back();
            }
            
            values_.push_back(arg_value_type(std::forward<TpString_>(value),
                                             values_types,
                                             regex_to_match,
                                             base_arg_type::get_flags().get_value()));
        }
        else
        {
            base_arg_type::raise_error_flag(arg_error_flags::MAX_VALUES_ERROR);
            success = false;
        }
    
        update_error_flags();
        
        return success;
    }
    
    /**
     * @brief       Try to add a value to the argument. No error flags will be raised.
     * @param       value : Value to add to the arguement.
     * @return      If the function was successful true is returned, otherwise false is returned.
     */
    template<typename TpString_>
    bool try_add_value(TpString_&& value)
    {
        bool success = false;
        arg_value_types values_types;
        string_type regex_to_match;
        
        if (!max_values_number_reached())
        {
            if (values_.size() + 1 <= values_types_.size())
            {
                values_types = values_types_[values_.size()];
            }
            else if (!values_types_.empty())
            {
                values_types = values_types_.back();
            }
            else
            {
                values_types = arg_value_types::STRING;
            }
            
            if (values_.size() + 1 <= regex_collection_.size())
            {
                regex_to_match = regex_collection_[values_.size()];
            }
            else if (!regex_collection_.empty())
            {
                regex_to_match = regex_collection_.back();
            }
            
            arg_value_type av(std::forward<TpString_>(value), values_types, regex_to_match,
                              base_arg_type::get_flags().get_value());
            
            if (!av.there_are_errors())
            {
                values_.push_back(av);
                success = true;
                update_error_flags();
            }
        }
        
        return success;
    }
    
    /**
     * @brief       Get first argument value.
     * @return      The first argument value.
     * @throw       std::out_of_range : If argument hasn't any value an exception is thrown.
     */
    const arg_value_type& get_front_value() const
    {
        if (values_.empty())
        {
            throw std::out_of_range("std::out_of_range: argument hasn't any value");
        }
        
        return values_.front();
    }
    
    /**
     * @brief       Get first argument value.
     * @param       default_value : The value used to construct an argument value if there isn't any
     *              value.
     * @return      If function was successful the first argument value is returned, otherwise an
     *              argument value constructed with the default value is returned.
     */
    template<typename TpString_>
    const arg_value_type& get_front_value(TpString_&& default_value) const
    {
        if (values_.empty())
        {
            static arg_value_type default_arg_value;
            default_arg_value = arg_value_type(std::forward<TpString_>(default_value),
                                               arg_value_types::STRING,
                                               string_type(),
                                               base_arg_type::get_flags().get_value());
            return default_arg_value;
        }
        
        return values_.front();
    }
    
    /**
     * @brief       Get the value in the specified position.
     * @param       index : Position of the element.
     * @return      The element specified.
     * @throw       std::out_of_range : If there isn't any element in the specified position an
     *              exception is thrown.
     */
    const arg_value_type& get_value_at(std::size_t index) const
    {
        if (values_.size() <= index)
        {
            throw std::out_of_range("std::out_of_range: there isn't any value in the specified "
                                            "position");
        }
        
        return values_.at(index);
    }
    
    /**
     * @brief       Get the value in the specified position.
     * @param       index : Position of the element.
     * @param       default_value : The value used to construct an argument value if there isn't any
     *              value.
     * @return      If function was successful the argument value in the specified position is
     *              returned, otherwise an argument value constructed with the default value is
     *              returned.
     */
    template<typename TpString_>
    const arg_value_type& get_value_at(
            std::size_t index,
            TpString_&& default_value
    ) const
    {
        if (values_.size() <= index)
        {
            static arg_value_type default_arg_value;
            default_arg_value = arg_value_type(std::forward<TpString_>(default_value),
                                               arg_value_types::STRING,
                                               string_type(),
                                               base_arg_type::get_flags().get_value());
            return default_arg_value;
        }
        
        return values_.at(index);
    }
    
    /**
     * @brief       Allows knowing wether there are values in the argument.
     * @return      If function was successful true is returned, otherwise false is returned.
     */
    inline bool there_are_values() const noexcept
    {
        return !values_.empty();
    }
    
    /**
     * @brief       Clear values collection.
     */
    void clear_values() noexcept
    {
        values_.clear();
        base_arg_type::clear_error_flags();
    }
    
    /**
     * @brief       Allows knowing whether the argument has rached the minimal number of values.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    inline bool min_values_number_reached() const noexcept
    {
        return values_.size() >= min_values_;
    }
    
    /**
     * @brief       Allows knowing whether the argument can't get more values.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    inline bool max_values_number_reached() const noexcept
    {
        return values_.size() >= max_values_;
    }
    
    /**
     * @brief       Set error flags if it's required.
     */
    void update_error_flags() noexcept override
    {
        bool in;
    
        base_arg_type::update_error_flags();
        
        if (values_.size() < min_values_ && base_arg_type::was_found())
        {
            base_arg_type::raise_error_flag(arg_error_flags::MIN_VALUES_ERROR);
        }
        else
        {
            base_arg_type::erase_error_flag(arg_error_flags::MIN_VALUES_ERROR);
        }
        
        if (values_.size() > max_values_)
        {
            base_arg_type::raise_error_flag(arg_error_flags::MAX_VALUES_ERROR);
            while (values_.size() > max_values_)
            {
                values_.pop_back();
            }
        }
        
        in = false;
        for (auto& x : values_)
        {
            if (x.there_are_errors())
            {
                in = true;
                base_arg_type::raise_error_flag(arg_error_flags::VALUES_ERROR);
            }
        }
        if (!in)
        {
            base_arg_type::erase_error_flag(arg_error_flags::VALUES_ERROR);
        }
    }
    
    /**
     * @brief       Print argument errors in standard output.
     * @param       program_name : The name of the program.
     * @param       colors_enable : If it is true, colors will be used during the print.
     */
    void print_errors(const string_type& program_name, bool colors_enable) const override
    {
        base_arg_type::print_errors(program_name, colors_enable);
        
        auto& os = kcs::iostream::get_cout<char_type>();
        
        if (base_arg_type::error_flag_is_raised(arg_error_flags::MIN_VALUES_ERROR))
        {
            os << program_name << ": ";
            if (!this->get_error_id().empty())
            {
                if (colors_enable)
                {
                    kcs::system::set_ostream_text_attribute(
                            os, kcs::system::text_attribute::LIGHT_RED);
                    os << this->get_error_id() << ": ";
                    kcs::system::set_ostream_text_attribute(
                            os, kcs::system::text_attribute::DEFAULT);
                }
                else
                {
                    os << this->get_error_id() << ": ";
                }
            }
            os << "Option requires at least " << min_values_
               << (min_values_ > 1 ? " arguments" : " argument") << std::endl;
        }
        
        if (base_arg_type::error_flag_is_raised(arg_error_flags::MAX_VALUES_ERROR))
        {
            os << program_name << ": ";
            if (!this->get_error_id().empty())
            {
                if (colors_enable)
                {
                    kcs::system::set_ostream_text_attribute(
                            os, kcs::system::text_attribute::LIGHT_RED);
                    os << this->get_error_id() << ": ";
                    kcs::system::set_ostream_text_attribute(
                            os, kcs::system::text_attribute::DEFAULT);
                }
                else
                {
                    os << this->get_error_id() << ": ";
                }
            }
            os << "Option must have a maximum of " << max_values_
               << (max_values_ > 1 ? " arguments" : " argument") << std::endl;
        }
        
        if (base_arg_type::error_flag_is_raised(arg_error_flags::VALUES_ERROR) &&
            there_are_values())
        {
            for (auto& x : values_)
            {
                if (x.there_are_errors())
                {
                    x.print_errors(program_name, this->get_error_id(), colors_enable);
                }
            }
        }
    }

protected:
    /**
     * @brief       Local copy assignment method.
     * @param       rhs : Object to copy.
     * @return      The object who call the method.
     */
    basic_value_arg& local_assignment(const basic_value_arg& rhs)
    {
        if (this != &rhs)
        {
            values_ = rhs.values_;
            min_values_ = rhs.min_values_;
            max_values_ = rhs.max_values_;
            values_types_ = rhs.values_types_;
            regex_collection_ = rhs.regex_collection_;
        }
        
        return *this;
    }
    
    /**
     * @brief       Local move assignment method.
     * @param       rhs : Object to move.
     * @return      The object who call the method.
     */
    basic_value_arg& local_assignment(basic_value_arg&& rhs) noexcept
    {
        if (this != &rhs)
        {
            values_ = std::move(rhs.values_);
            std::swap(min_values_, rhs.min_values_);
            std::swap(max_values_, rhs.max_values_);
            values_types_ = std::move(rhs.values_types_);
            regex_collection_ = std::move(rhs.regex_collection_);
        }
        
        return *this;
    }

private:
    /** Collection that has the values gotten through the program call for an option. */
    vector_type<arg_value_type> values_;
    
    /** Minimum number of values for an option. */
    std::size_t min_values_;
    
    /** Maximum number of values for an option. */
    std::size_t max_values_;
    
    /** Collection that has the values types that must have the values. */
    vector_type<arg_value_types> values_types_;
    
    /** Regular expressions that the values has to match. If the collection is empty all values will
     *  match. */
    vector_type<string_type> regex_collection_;
};


/** Class that represents arguments that have values with 8 bits characters. */
using value_arg = basic_value_arg<char>;


/** Class that represents arguments that have values with 16 bits characters. */
using wvalue_arg = basic_value_arg<wchar_t>;
    
    
}
}


#endif
