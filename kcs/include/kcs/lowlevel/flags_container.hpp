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
 * @file       kcs/lowlevel/flags_container.hpp
 * @brief      flags_container class header.
 * @author     Killian
 * @date       2016/01/08 - 11:06
 */

#ifndef KCS_LOWLEVEL_FLAGS_CONTAINER_HPP
#define KCS_LOWLEVEL_FLAGS_CONTAINER_HPP

#include <iostream>

#include "../type_traits/type_traits.hpp"


namespace kcs {
namespace lowlevel {


/**
 * @brief       Class that represents flags conteiner.
 */
template<typename TpValue>
class flags_container
{
public:
    /** Value type of the flags conteiner. */
    using value_type = TpValue;
    
    /** Underlying value of the flags conteiner. */
    using underlying_type = std::conditional_t<
            std::is_enum<TpValue>::value,
            kcs::type_traits::try_underlying_type_t<TpValue>,
            TpValue
    >;
    
    /**
     * @brief       Default constructor.
     */
    inline flags_container() noexcept
            : value_(static_cast<value_type>(0U))
    {
    }
    
    /**
     * @brief       Constructor with parameters.
     * @param       value : Default flags value.
     */
    inline flags_container(const value_type& value) noexcept
            : value_(value)
    {
    }
    
    /**
     * @brief       Copy constructor.
     * @param       rhs : The object to copy.
     */
    inline flags_container(const flags_container& rhs) = default;
    
    /**
     * @brief       Move constructor.
     * @param       rhs : The object to move.
     */
    inline flags_container(flags_container&& rhs) noexcept
            : value_(rhs.value_)
    {
        rhs.value_ = static_cast<value_type>(0U);
    }
    
    /**
     * @brief       Destrunctor.
     */
    inline ~flags_container()
    {
        value_ = static_cast<value_type>(0U);
    }
    
    /**
     * @brief       Copy assignment operator.
     * @param       rhs : The object to copy.
     * @return      The object who call the method.
     */
    flags_container& operator =(const flags_container& rhs) = default;
    
    /**
     * @brief       Move assignement operator.
     * @param       rhs : The object to move.
     * @return      The object who call the method.
     */
    flags_container& operator =(flags_container&& rhs) noexcept
    {
        if (this != &rhs)
        {
            std::swap(value_, rhs.value_);
        }
        
        return *this;
    }
    
    /**
     * @brief       Get the value that contains the flags conteiner.
     * @return      The value that constains the flags conteiner.
     */
    inline const value_type& get_value() const noexcept
    {
        return value_;
    }
    
    /**
     * @brief       Set a frag in the conteiner.
     * @param       flag : Flag to set in the conteiner.
     */
    inline void raise_flag(const value_type& flag) noexcept
    {
        value_ = static_cast<value_type>(static_cast<underlying_type>(value_) |
                                        static_cast<underlying_type>(flag));
    }
    
    /**
     * @brief       Set a frag in the conteiner by index.
     * @param       index : Index in which set a flag in the conteiner.
     */
    inline void raise_flag_by_index(std::uint16_t index) noexcept
    {
        value_ = static_cast<value_type>(static_cast<underlying_type>(value_) | (1U << index));
    }
    
    /**
     * @brief       Erase a flag in the conteiner.
     * @param       flag : Flag to erase in the conteiner.
     */
    inline void erase_flag(const value_type& flag) noexcept
    {
        value_ = static_cast<value_type>(static_cast<underlying_type>(value_) &
                                        ~static_cast<underlying_type>(flag));
    }
    
    /**
     * @brief       Erase a flag in the conteiner.
     * @param       index : Index in which erase a flag in the conteiner.
     */
    inline void erase_flag_by_index(std::uint16_t index) noexcept
    {
        value_ = static_cast<value_type>(static_cast<underlying_type>(value_) & ~(1U << index));
    }
    
    /**
     * @brief       Allows knowing wether a flag is set in the conteiner.
     * @param       flag : Flag to verify its presence in the conteiner.
     */
    inline bool flag_is_raised(const value_type& flag) const noexcept // todo fix for multiple flags
    {
        return ((static_cast<underlying_type>(value_) & static_cast<underlying_type>(flag)) != 0U);
    }
    
    /**
     * @brief       Allows knowing wether a flag is set in the conteiner by index.
     * @param       index : Index in which verify a flag presence in the conteiner.
     */
    inline bool flag_is_raised_by_index(std::uint16_t index) const noexcept
    {
        return ((static_cast<underlying_type>(value_) & (1U << index)) != 0U);
    }
    
    /**
     * @brief       Allows knowing wether the container is empty.
     * @return      If function was successfull true is returned, otherwise false is returned.
     */
    inline bool is_empty() const noexcept
    {
        return (static_cast<underlying_type>(value_) == 0U);
    }
    
    /**
     * @brief       Put all the flags to zero.
     */
    inline void clear() noexcept
    {
        value_ = static_cast<value_type>(0U);
    }
     
    /**
     * @brief       Print the flag in the standard output with a byte separator.
     * @param       os : The object used to print in standard output.
     * @param       sequence_length : The number of flags to print until print a white space.
     * @throw       std::basic_ostream::failure : It throws an exception if the resulting error
     *              state flag of the 'std::basic_ostream' object is not goodbit and member
     *              exceptions was set to throw for that state.
     */
    template<typename CharT, typename CharTraitsT>
    void print(
            std::basic_ostream<CharT, CharTraitsT>& os,
            std::uint16_t sequence_length = 8U
    ) const
    {
        std::uint16_t n_bits = sizeof(value_type) * 8U;
        
        for (std::uint16_t i = 0U, cont = 1U; i < n_bits; i++)
        {
            os << (static_cast<underlying_type>(value_) >> (n_bits - 1U - i) & 1U);
            
            if (sequence_length != 0 && cont == sequence_length && i + 1U < n_bits)
            {
                os << " ";
                cont = 1U;
            }
            else
            {
                cont++;
            }
        }
        
        os << std::endl;
    }
    
    /**
     * @brief       Allows knowing wether two flags conteiner are equal.
     * @param       rhs : Object to compare.
     * @return      If the objets are equal true is returned, otherwise false is returned.
     */
    inline bool operator ==(const flags_container& rhs) const noexcept
    {
        return (static_cast<underlying_type>(value_) == static_cast<underlying_type>(rhs.value_));
    }
    
    /**
     * @brief       Allows knowing wether two flags conteiner are different.
     * @param       rhs : Object to compare.
     * @return      If the objets are different true is returned, otherwise false is returned.
     */
    inline bool operator !=(const flags_container& rhs) const noexcept
    {
        return (static_cast<underlying_type>(value_) != static_cast<underlying_type>(rhs.value_));
    }

private:
    value_type value_;
};


/**
 * @brief       Allows to use an object to print in standard output the value that constains the
 *              flags conteiner.
 * @param       os : The object who call the function.
 * @param       flags : The flags conteiner to print in standard output.
 * @return      The object who call the function.
 * @throw       std::basic_ostream::failure : It throws an exception if the resulting error state
 *              flag of the 'std::basic_ostream' object is not goodbit and member exceptions was set
 *              to throw for that state.
 */
template<typename TpChar, typename TpCharTraits, typename TpValue>
std::basic_ostream<TpChar, TpCharTraits>& operator <<(
        std::basic_ostream<TpChar, TpCharTraits>& os,
        const flags_container<TpValue>& flags
)
{
    using ValueType = typename flags_container<TpValue>::underlying_type;
    
    std::uint16_t n_bits = sizeof(TpValue) * 8U;
    
    for (std::uint16_t i = 0U, cont = 1U; i < n_bits; i++)
    {
        os << (static_cast<ValueType>(flags.get_value()) >> (n_bits - 1 - i) & 1);
        
        if (cont == 8U)
        {
            os << " ";
            cont = 1U;
        }
        else
        {
            cont++;
        }
    }
    
    return os;
}


}
}


#endif
