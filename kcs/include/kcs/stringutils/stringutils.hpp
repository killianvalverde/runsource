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
 * @file        kcs/stringutils/stringutils.hpp
 * @brief       stringutils fonctions header.
 * @author      Killian
 * @date        2016/01/08 - 19:32
 */

#ifndef KCS_STRINGUTILS_STRINGUTILS_HPP
#define KCS_STRINGUTILS_STRINGUTILS_HPP

#include <string>
#include <type_traits>
#include <vector>

#include "../type_traits/type_traits.hpp"


namespace kcs {
namespace stringutils {


/**
 * @brief       Returns the length of a C string str.
 * @param       str : A C string.
 * @return      The length of the C string.
 */
template<typename TpChar>
std::enable_if_t<
        kcs::type_traits::is_character<TpChar>::value,
        std::size_t
>
strlen(const TpChar *str) noexcept
{
    std::size_t len = 0;
    
    if (str != nullptr)
    {
        for (; *str != '\0'; str++)
        {
            len++;
        }
    }
    
    return len;
}


/**
 * @brief       Returns the length of a string.
 * @param       str : A string.
 * @return      The length of the string.
 */
template<typename TpString>
inline typename TpString::size_type strlen(const TpString& str) noexcept
{
    return str.length();
}


/**
 * @brief       Copies the C string pointed by source into the array pointed by destination,
 *              including the terminating null character (and stopping at that point).
 * @param       destination : Pointer to the destination array where the content is to be copied.
 * @param       source : C string to be copied.
 * @return      The pointer to the destination array.
 */
template<typename TpDestinationChar, typename TpSourceChar>
std::enable_if_t<
        kcs::type_traits::is_character<TpDestinationChar>::value,
        TpDestinationChar*
>
strcpy(TpDestinationChar *destination, const TpSourceChar* source) noexcept
{
    TpDestinationChar* orig_dest = destination;
    
    if (destination != nullptr && source != nullptr)
    {
        for (; *source != '\0'; source++, destination++)
        {
            *destination = *source;
        }
        
        *destination = '\0';
    }
    
    return orig_dest;
}


/**
 * @brief       Copies the source string into the destination string.
 * @param       destination : The string where the content has to be copied.
 * @param       source : String to be copied.
 * @return      The destination string.
 */
template<typename TpDestinationString, typename TpSourceString>
inline std::enable_if_t<
        !kcs::type_traits::is_character<TpDestinationString>::value,
        TpDestinationString&
>
strcpy(
        TpDestinationString& destination,
        TpSourceString&& source
)
{
    return destination.assign(std::forward<TpSourceString>(source));
}


/**
 * @brief       Copies the C string first num characters of source to destination. If the end of the
 *              source C string (which is signaled by a null-character) is found before num
 *              characters have been copied, destination is padded with zeros until a total of num
 *              characters have been written to it.
 * @param       destination : Pointer to the 'destination' array where the content is to be copied.
 * @param       source : C string to be copied.
 * @param       number : Maximum number of characters to be copied from source.
 * @return      The pointer to the destination array
 */
template<typename TpDestinationChar, typename TpSourceChar>
std::enable_if_t<
        kcs::type_traits::is_character<TpDestinationChar>::value,
        TpDestinationChar*
>
strncpy(TpDestinationChar *destination, const TpSourceChar* source, std::size_t number) noexcept
{
    TpDestinationChar *orig_dest = destination;
    std::size_t i = 0;
    
    if (destination != nullptr && source != nullptr)
    {
        for (; *source != '\0' && i < number; source++, destination++, i++)
        {
            *destination = *source;
        }
        
        for (; i < number; destination++, i++)
        {
            *destination = '\0';
        }
        
        *destination = '\0';
    }
    
    return orig_dest;
}


/**
 * @brief       Copies the string first num characters of source to destination.
 * @param       destination : The string where the content is to be copied.
 * @param       source : String to be copied.
 * @param       number : Maximum number of characters to be copied from source.
 * @return      The destination string.
 * @throw       std::out_of_range : If num is greater than the source string length, an out_of_range
 *              exception is thrown.
 */
template<typename TpDestinationString, typename TpSourceString>
inline std::enable_if_t<
        !kcs::type_traits::is_character<TpDestinationString>::value,
        TpDestinationString&
>
strncpy(
        TpDestinationString& destination,
        TpSourceString&& source,
        typename TpDestinationString::size_type number
)
{
    destination.assign(std::forward<TpSourceString>(source));
    destination.erase(number, TpDestinationString::npos);
    return destination;
}


/**
 * @brief       Assign the source into the destination including the terminating null character
 *              (and stopping at that point).
 * @param       destination : Destination to assign the source content.
 * @param       source : Source to be assigned.
 * @return      The destination.
 */
template<typename TpDestinationChar, typename TpSourceChar>
TpDestinationChar& strassign(TpDestinationChar& destination, const TpSourceChar& source)
{
    const auto src_size = strlen(source);
    decltype(src_size) i;
    
    for (i = 0; i < src_size; i++)
    {
        destination[i] = source[i];
    }
    
    destination[i] = '\0';
    
    return destination;
}


/**
 * @brief       Compare the target string to the destination.
 * @param       source : Comparation source.
 * @param       target : Comparation target.
 * @return      If the first character that does not match has a lower value in source than in
 *              target <0 is returned, if the contents of both strings are equal 0 is returned, and
 *              if the first character that does not match has a greater value in source than in
 *              target >0 is returned.
 */
template<typename TpSource, typename TpTarget>
int strcmp(const TpSource& source, const TpTarget& target)
{
    const auto src_size = strlen(source);
    decltype(src_size) targ_size = strlen(source);
    typename std::remove_const<decltype(src_size)>::type min_size;
    typename std::remove_const<decltype(src_size)>::type i;
    
    min_size = (src_size <= targ_size) ? src_size : targ_size;
    
    for (i = 0; i < min_size; i++)
    {
        if (source[i] < target[i])
        {
            return -1;
        }
        else if (source[i] > target[i])
        {
            return 1;
        }
    }
    
    if (src_size != targ_size)
    {
        return (src_size < targ_size) ? -1 : 1;
    }
    
    return 0;
}


/**
 * @brief       Returns a pointer to the first occurrence of character in the C string str.
 * @param       str : The C string.
 * @param       character : Character to be located.
 * @return      A pointer to the first occurrence of character in str. If the character is not
 *              found, the function returns a null pointer.
 */
template<typename TpChar, typename TpIntegral>
std::enable_if_t<
        kcs::type_traits::is_character<TpChar>::value,
        TpChar*
>
strchr(TpChar* str, const TpIntegral character) noexcept
{
    if (str != nullptr)
    {
        while (*str != '\0' && *str != character)
        {
            str++;
        }
        
        if (*str != character)
        {
            str = nullptr;
        }
    }
    
    return str;
}


/**
 * @brief       Returns a pointer to the first occurrence of character in the string.
 * @param       str : The string
 * @param       character : Character to be located.
 * @return      A pointer to the first occurrence of character in str. If the character is not
 *              found, the function returns a null pointer.
 */
template<typename TpString, typename TpIntegral>
std::enable_if_t<
        !kcs::type_traits::is_character<TpString>::value,
        typename TpString::pointer
>
strchr(TpString& str, const TpIntegral character) noexcept
{
    auto it = str.begin();
    typename TpString::pointer pchr = nullptr;
    
    while (it != str.end() && *it != character)
    {
        it++;
    }
    
    if (it != str.end())
    {
        pchr = &*it;
    }
    
    return pchr;
}


/**
 * @brief       Returns a pointer to the first occurrence of character in the C string str. Only the
 *              firs num characters will be evaluated.
 * @param       str : The C string.
 * @param       character : Character to be located.
 * @param       number : Maximum number of characters in which search.
 * @return      A pointer to the first occurrence of character in str. If the character is not
 *              found, the function returns a null pointer.
 */
template<typename TpChar, typename TpIntegral>
std::enable_if_t<
        kcs::type_traits::is_character<TpChar>::value,
        TpChar*
>
strnchr(TpChar* str, const TpIntegral character, std::size_t number) noexcept
{
    std::size_t i;
    
    if (str != nullptr)
    {
        for (i = 0; *str != '\0' && *str != character && i < number; i++)
        {
            str++;
        }
        
        if (*str != character)
        {
            str = nullptr;
        }
    }
    
    return str;
}


/**
 * @brief       Returns a iterator to the first occurrence of character in the string str. Only the
 *              first num characters will be evaluated.
 * @param       str : The string.
 * @param       character : Character to be located.
 * @param       number : Maximum number of characters in which search.
 * @return      A iterator to the first occurrence of character in str. If the character is not
 *              found, the function returns the end iterator.
 */
template<typename TpString, typename TpIntegral>
std::enable_if_t<
        !kcs::type_traits::is_character<TpString>::value,
        typename TpString::pointer
>
strnchr(
        TpString& str,
        const TpIntegral character,
        typename TpString::size_type number
) noexcept
{
    auto it = str.begin();
    typename TpString::size_type i;
    typename TpString::pointer pchr = nullptr;
    
    for (i = 0; it != str.end() && *it != character && i < number; i++)
    {
        it++;
    }
    
    if (*it != character)
    {
        it = str.end();
    }
    
    if (it != str.end())
    {
        pchr = &*it;
    }
    
    return pchr;
}


/**
 * @brief       Searches the C string for the first character that matches the characters specified
 *              in its arguments.
 * @param       str : The C string in which search.
 * @param       characters : Another C string with the characters to search for.
 * @param       position : Position of the first character in the C string to be considered in the
 *              search.
 * @param       number : Number of character values to search for in characters.
 * @return      The position of the first character that matches. If no matches are found, the
 *              function returns the biggest 'std::size_t' number.
 */
template<typename TpChar, typename TpFindChar>
std::enable_if_t<
        kcs::type_traits::is_character<TpChar>::value,
        std::size_t
>
strfindfirst(
        const TpChar* str,
        const TpFindChar* characters,
        std::size_t position = 0,
        std::size_t number = ~(std::size_t)0
) noexcept
{
    std::size_t fnd_pos = ~(std::size_t)0;
    std::size_t i;
    
    if (str != nullptr)
    {
        for (i = 0; *str != '\0' && position > 0; str++, i++, position--)
        {
        }
        
        for (; *str != '\0' && i < ~(std::size_t)0; str++, i++)
        {
            if (kcs::stringutils::strnchr(characters, *str, number) != nullptr)
            {
                fnd_pos = i;
                break;
            }
        }
    }
    
    return fnd_pos;
}


/**
 * @brief       Searches the string for the first character that matches the characters specified in
 *              its arguments.
 * @param       str : The string in which search.
 * @param       characters : Another string with the characters to search for.
 * @param       position : Position of the first character in the string to be considered in
 *              the search.
 * @param       number : Number of character values to search for in characters.
 * @return      The position of the first character that matches. If no matches are found, the
 *              function returns 'TpString::npos'.
 */
template<typename TpString, typename TpFindString>
std::enable_if_t<
        !kcs::type_traits::is_character<TpString>::value,
        typename TpString::size_type
>
strfindfirst(
        const TpString& str,
        TpFindString&& characters,
        typename TpString::size_type position = 0,
        typename TpString::size_type number = TpString::npos
) noexcept
{
    using string_size_type = typename TpString::size_type;
    
    constexpr auto snpos = TpString::npos;
    auto it = str.cbegin();
    string_size_type found_position = snpos;
    string_size_type i = 0;
    
    for (i = 0; it != str.cend() && position > 0; it++, i++, position--)
    {
    }
    
    for (; it != str.cend() && i < snpos; it++, i++)
    {
        if (kcs::stringutils::strnchr(characters, *it, number) != nullptr)
        {
            found_position = i;
            break;
        }
    }
    
    return found_position;
}


/**
 * @brief       Searches the C string for the last character that matches any of the characters
 *              specified in its arguments.
 * @param       str : The C string in which search.
 * @param       characters : Another C string with the characters to search for.
 * @param       position : Position of the last character in the string to be considered in the
 *              search.
 * @param       number : Number of character values to search for in characters.
 * @return      The position of the first character that matches. If no matches are found, the
 *              function returns the biggest 'std::size_t' number.
 */
template<typename TpChar, typename TpFindChar>
std::enable_if_t<
        kcs::type_traits::is_character<TpChar>::value,
        std::size_t
>
strfindlast(
        const TpChar *str,
        const TpFindChar *characters,
        std::size_t position = ~(std::size_t)0 - 1,
        std::size_t number = ~(std::size_t)0
) noexcept
{
    std::size_t found_pos = ~(std::size_t)0;
    std::size_t i;
    
    if (str != nullptr)
    {
        if (position == ~(std::size_t)0)
        {
            position--;
        }
        
        for (i = 0; *str != '\0' && i <= position; str++, i++)
        {
            if (kcs::stringutils::strnchr(characters, *str, number) != nullptr)
            {
                found_pos = i;
            }
        }
    }
    
    return found_pos;
}


/**
 * @brief       Searches the string for the last character that matches any of the characters
 *              specified in its arguments.
 * @param       str : The string in which search.
 * @param       characters : Another string with the characters to search for.
 * @param       position : Position of the last character in the string to be considered in the
 *              search.
 * @param       number : Number of character values to search for in characters.
 * @return      The position of the last character that matches. If no matches are found, the
 *              function returns 'TpString::npos'.
 */
template<typename TpString, typename TpFindString>
std::enable_if_t<
        kcs::type_traits::is_character<TpString>::value,
        typename TpString::size_type
>
strfindlast(
        const TpString& str,
        TpFindString&& characters,
        typename TpString::size_type position = TpString::npos - 1,
        typename TpString::size_type number = TpString::npos
) noexcept
{
    using string_size_type = typename TpString::size_type;
    
    constexpr auto snpos = TpString::npos;
    auto it = str.cbegin();
    string_size_type fnd_pos = snpos;
    string_size_type i = 0;
    
    if (position == snpos)
    {
        position--;
    }
    
    for (i = 0; it != str.cend() && i < position; it++, i++)
    {
        if (kcs::stringutils::strnchr(characters, *it, number) != nullptr)
        {
            fnd_pos = i;
        }
    }
    
    return fnd_pos;
}


/**
 * @brief       Split a C string by a specified separator.
 * @param       str : The C string to split.
 * @param       separator : The character used to split the string.
 * @return      A 'std::vector' of 'std::basic_string' with all the values obtained.
 */
template<
        typename TpChar,
        typename TpIntegral,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpAlloc = std::allocator<int>
>
std::enable_if_t<
        kcs::type_traits::is_character<TpChar>::value,
        std::vector<std::basic_string<TpChar, TpCharTraits, TpAlloc>, TpAlloc>
>
strsplit(const TpChar *str, const TpIntegral separator)
{
    using string_t = std::basic_string<TpChar, TpCharTraits, TpAlloc>;
    
    std::vector<string_t, TpAlloc> values;
    string_t value_builder;
    
    if (str != nullptr)
    {
        for (const TpChar* it = str; *it != '\0'; it++)
        {
            if (*it != separator)
            {
                if (*it == '\\' &&
                    *(it + 1) != '\0' &&
                    *(it + 1) == separator)
                {
                    continue;
                }
                
                else
                {
                    value_builder += *it;
                }
            }
            
            else
            {
                if (it != str && *(it - 1) == '\\')
                {
                    value_builder += *it;
                }
                
                else if (!value_builder.empty())
                {
                    values.push_back(std::move(value_builder));
                }
            }
            
            if (*(it + 1) == '\0' && !value_builder.empty())
            {
                values.push_back(std::move(value_builder));
            }
        }
    }
    
    return values;
}


/**
 * @brief       Split a string by a specified separator.
 * @param       str : The string to split.
 * @param       separator : The character used to split the string.
 * @return      A 'std::vector' of 'std::basic_string' with all the values obtained.
 */
template<typename TpString>
inline auto strsplit(const TpString& str, const typename TpString::value_type separator)
{
    return strsplit<typename TpString::value_type,
                    typename TpString::value_type,
                    typename TpString::traits_type,
                    typename TpString::allocator_type>(str.c_str(), separator);
}


/**
 * @brief       Get a C string without the values lower than the specified value.
 * @param       str : The C string to erase the values lower than the specified value.
 * @param       value : The value used to discard characters.
 * @return      The C string without the values lower than the specified value.
 */
template<typename TpChar, typename TpIntegral>
std::enable_if_t<
        kcs::type_traits::is_character<TpChar>::value,
        TpChar*
>
strdisclower(TpChar *str, const TpIntegral value) noexcept
{
    if (str != nullptr)
    {
        TpChar* it = str;
        for (TpChar* forward_it = str; *forward_it != '\0'; it++, forward_it++)
        {
            while (*forward_it < value && *forward_it != '\0')
            {
                forward_it++;
            }
            
            *it = *forward_it;
        }
        
        *it = '\0';
    }
    
    return str;
}


/**
 * @brief       Get a string without the values lower than the specified value.
 * @param       str : The string to erase the values lower than the specified value.
 * @param       value : The value used to discard characters.
 * @return      The str parameter without the values lower than the specified value.
 */
template<typename TpString, typename TpIntegral>
std::enable_if_t<
        !kcs::type_traits::is_character<TpString>::value,
        TpString&
>
strdisclower(TpString& str, const TpIntegral value) noexcept
{
    for (auto it = str.begin(); it != str.end();)
    {
        if (*it < value)
        {
            it = str.erase(it);
        }
        
        else
        {
            it++;
        }
    }
    
    return str;
}
    
    
}
}


#endif
