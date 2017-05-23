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
 * @file        kcs/filesystem/filesystem.hpp
 * @brief       filesystem functions header.
 * @author      Killian
 * @date        2016/01/08 - 20:04
 */

#ifndef KCS_FILESYSTEM_FILESYSTEM_HPP
#define KCS_FILESYSTEM_FILESYSTEM_HPP

#include <experimental/filesystem>
#include <stdexcept>
#include <string>
#include <system_error>
#include <type_traits>

#include "../stringutils/stringutils.hpp"
#include "../system/system.hpp"
#include "../type_traits/type_traits.hpp"


namespace kcs {
namespace filesystem {


/**
 * @brief       Get the file name from a C string path.
 * @param       path : The path in which extract the file name.
 * @return      A 'std::basic_string' with the file name. If a file name isn't found path is
 *              returned in the 'std::basic_string'.
 */
template<
        typename TpChar,
        typename TpCharTraits = std::char_traits<TpChar>,
        typename TpCharAlloc = std::allocator<TpChar>
>
std::enable_if_t<
        kcs::type_traits::is_character<TpChar>::value,
        std::basic_string<TpChar, TpCharTraits, TpCharAlloc>
>
get_file_name(const TpChar *path)
{
    using string_type = std::basic_string<TpChar, TpCharTraits, TpCharAlloc>;
    using string_size_type = typename string_type::size_type;
    
    string_size_type found_pos;

#ifdef _WIN32
    found_pos = kcs_test::stringutils::strfindlast(path, "/\\");
#else
    found_pos = kcs::stringutils::strfindlast(path, "/");
#endif
    
    if (found_pos < ~(string_size_type)0 && found_pos + 1 < kcs::stringutils::strlen(path))
    {
        return string_type(path + found_pos);
    }
    
    return string_type(path);
}


/**
 * @brief       Get the file name from a 'std::basic_string' path.
 * @param       path : The path in which extract the file name.
 * @return      A 'std::basic_string' with the file name. If a file name isn't found path is
 *              returned in the 'std::basic_string'.
 */
template<typename TpBasicString>
std::enable_if_t<
        kcs::type_traits::is_basic_string<std::decay_t<TpBasicString>>::value,
        TpBasicString
>
get_file_name(TpBasicString&& path)
{
    using string_type = typename std::decay_t<TpBasicString>;
    using string_size_type = typename string_type::size_type;
    
    string_size_type found_pos = string_type::npos;

#ifdef _WIN32
    found_pos = kcs_test::stringutils::strfindlast(path, "/\\");
#else
    found_pos = kcs::stringutils::strfindlast(path, "/");
#endif
    
    if (found_pos != string_type::npos && found_pos + 1 < path.length())
    {
        return path.substr(found_pos + 1);
    }
    
    return string_type(std::forward<TpBasicString>(path));
}


/**
 * @brief       Allows to know if the specified directory can be read by the current process.
 * @param       dir_path : The directory to check.
 * @param       error_condition : If the process was successful ec is set to 0, if the path does not
 *              exists is set to ENOENT, if the path is not a directory is set to ENOTDIR and if the
 *              permission is not matched is set to EACCES.
 * @return      If the process was successful true is returned, otherwise false is returned.
 */
bool can_current_process_read_dir(
        const std::experimental::filesystem::path& dir_path,
        std::error_condition& error_condition
) noexcept;


/**
 * @brief       Allows to know if the specified directory can be written by the current process.
 * @param       dir_path : The directory to check.
 * @param       error_condition : If the process was successful ec is set to 0, if the path does not
 *              exists is set to ENOENT, if the path is not a directory is set to ENOTDIR and if the
 *              permission is not matched is set to EACCES.
 * @return      If the process was successful true is returned, otherwise false is returned.
 */
bool can_current_process_write_dir(
        const std::experimental::filesystem::path& dir_path,
        std::error_condition& error_condition
) noexcept;


/**
 * @brief       Allows to know if the specified directory can be executed by the current process.
 * @param       dir_path : The directory to check.
 * @param       error_condition : If the process was successful ec is set to 0, if the path does not
 *              exists is set to ENOENT, if the path is not a directory is set to ENOTDIR and if the
 *              permission is not matched is set to EACCES.
 * @return      If the process was successful true is returned, otherwise false is returned.
 */
bool can_current_process_execute_dir(
        const std::experimental::filesystem::path& dir_path,
        std::error_condition& error_condition
) noexcept;


/**
 * @brief       Allows to know if the specified file can be read by the current process.
 * @param       file_path : The file to check.
 * @param       error_condition : If the process was successful ec is set to 0, if the path does not
 *              exists is set to ENOENT, if the path is not a file is set to EINVAL and if the
 *              permission is not matched is set to EACCES.
 * @return      If the process was successful true is returned, otherwise false is returned.
 */
bool can_current_process_read_file(
        const std::experimental::filesystem::path& file_path,
        std::error_condition& error_condition
) noexcept;


/**
 * @brief       Allows to know if the specified file can be written by the current process.
 * @param       file_path : The file to check.
 * @param       error_condition : If the process was successful ec is set to 0, if the path does not
 *              exists is set to ENOENT, if the path is not a file is set to EINVAL and if the
 *              permission is not matched is set to EACCES.
 * @return      If the process was successful true is returned, otherwise false is returned.
 */
bool can_current_process_write_file(
        const std::experimental::filesystem::path& file_path,
        std::error_condition& error_condition
) noexcept;


/**
 * @brief       Allows to know if the specified file can be read by the current process.
 * @param       file_path : The file to check.
 * @param       error_condition : If the process was successful ec is set to 0, if the path does not
 *              exists is set to ENOENT, if the path is not a file is set to EINVAL and if the
 *              permission is not matched is set to EACCES.
 * @return      If the process was successful true is returned, otherwise false is returned.
 */
bool can_current_process_write_or_create_file(
        const std::experimental::filesystem::path& file_path,
        std::error_condition& error_condition
) noexcept;


/**
 * @brief       Allows to know if the specified file can be executed by the current process.
 * @param       file_path : The file to check.
 * @param       error_condition : If the process was successful ec is set to 0, if the path does not
 *              exists is set to ENOENT, if the path is not a file is set to EINVAL and if the
 *              permission is not matched is set to EACCES.
 * @return      If the process was successful true is returned, otherwise false is returned.
 */
bool can_current_process_execute_file(
        const std::experimental::filesystem::path& file_path,
        std::error_condition& error_condition
) noexcept;
    
    
}
}


#endif
