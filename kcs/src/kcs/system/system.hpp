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
 * @file        kcs/system/system.hpp
 * @brief       system fonctions header.
 * @author      Killian
 * @date        2017/01/08 - 16:37
 */

#ifndef KCS_SYSTEM_SYSTEM_HPP
#define KCS_SYSTEM_SYSTEM_HPP

#ifdef __unix__
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

#include <experimental/filesystem>


namespace kcs {
namespace system {


/**
 * @brief       Get the UID of the current process.
 * @return      The UID of the current process.
 */
inline auto get_uid() noexcept
{
#if _POSIX_VERSION >= 200112L
    return getuid();
#else
#error "system not supported"
#endif
}


/**
 * @brief       Get the GID of the current process.
 * @return      The GID of the current process.
 */
inline auto get_gid() noexcept
{
#if _POSIX_VERSION >= 200112L
    return getgid();
#else
#error "system not supported"
#endif
}


/**
 * @brief       Get the UID of the specified path.
 * @param       current_path : The path to get the UID.
 * @return      On success the UID of the sepcified path is returned, otherwise -1 is returned.
 */
inline auto get_path_uid(const std::experimental::filesystem::path& current_path) noexcept
{
#if _POSIX_VERSION >= 200112L
    struct stat stt;
    if (stat(current_path.c_str(), &stt) == -1)
    {
        return (decltype(stt.st_uid))-1;
    }
    return stt.st_uid;
#else
#error "system not supported"
#endif
}


/**
 * @brief       Get the GID of the specified path.
 * @param       current_path : The path to get the GID.
 * @return      On success the UID of the sepcified path is returned, otherwise -1 is returned.
 */
inline auto get_path_gid(const std::experimental::filesystem::path& current_path) noexcept
{
#if _POSIX_VERSION >= 200112L
    struct stat stt;
    if (stat(current_path.c_str(), &stt) == -1)
    {
        return (decltype(stt.st_uid))-1;
    }
    return stt.st_gid;
#else
#error "system not supported"
#endif
}


}
}


#endif
