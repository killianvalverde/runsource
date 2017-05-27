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
 * @file        kcs/system/filesystem.hpp
 * @brief       filesystem fonctions header.
 * @author      Killian
 * @date        2017/05/26 - 17:20
 */

#ifndef KCS_SYSTEM_FILESYSTEM_HPP
#define KCS_SYSTEM_FILESYSTEM_HPP

#ifdef __unix__
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif


namespace kcs {
namespace system {


/**
 * @brief       Change the current execution directory.
 * @param       path : The path of the new current directory.
 * @return      If function was successful 0 is returned, otherwise -1 is returned and and errno may
 *              be set to indicate the error
 */
inline int chdir(const char* path) noexcept
{
#if _POSIX_VERSION >= 200112L
    return ::chdir(path);
#else
#error "system not supported"
#endif
}


}
}


#endif
