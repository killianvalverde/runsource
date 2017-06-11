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

#include <cstdint>
#include <experimental/filesystem>
#include <system_error>

#include "../lowlevel/enum_bitwise_operators.hpp"


namespace kcs {
namespace system {


namespace stdfs = std::experimental::filesystem;


/**
 * @brief       Represents the file types.
 */
enum class file_types : std::uint8_t
{
    /** Any file type. */
    NIL = 0x0,
    
    /** Block device. */
    BLOCK_FILE = 0x1,
    
    /** Character device. */
    CHARACTER_FILE = 0x2,
    
    /** Directory. */
    DIRECTORY = 0x4,
    
    /** Named pipe */
    FIFO = 0x8,
    
    /** Regular file. */
    REGULAR_FILE = 0x10,
    
    /** Named IPC socket. */
    SOCKET = 0x20,
    
    /** Symbolic link. */
    SYMLINK = 0x40,
    
    /** All file types. */
    FULL = 0x7F
};


/** Represents the file types. */
using ft_t = file_types;


/**
 * @brief       Represents the files access modes.
 */
enum class access_modes : std::uint8_t
{
    /** Any access mode. */
    NIL = 0,
    
    /** The file exists. */
    EXISTS = 0x1,
    
    /** Read is available. */
    READ = 0x2,
    
    /** Write is available. */
    WRITE = 0x4,
    
    /** Execute is available. */
    EXECUTE = 0x8,
    
    /** Create is available. */
    CREATE = 0x10,
    
    /** All access modes. */
    FULL = 0x1F
};


/** Represents the files access modes. */
using am_t = access_modes;


/**
 * @brief       Get the UID of the specified file.
 * @param       pathname : The file to get the UID.
 * @return      On success the UID of the sepcified file is returned, otherwise -1 is returned and
 *              errno may be set to indicate the error.
 * @throw       kcs::system::system_exception : If the system is not suported an exception in
 *              thrown.
 */
int get_file_uid(const stdfs::path& pathname);


/**
 * @brief       Get the GID of the specified file.
 * @param       pathname : The file to get the GID.
 * @return      On success the GID of the sepcified file is returned, otherwise -1 is returned and
 *              errno may be set to indicate the error.
 * @throw       kcs::system::system_exception : If the system is not suported an exception in
 *              thrown.
 */
int get_file_gid(const stdfs::path& pathname);


/**
 * @brief       Checks if the given path corresponds to a directory.
 * @param       pathname : Path to check.
 * @param       error_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned and errno
 *              may be set to indicate the error.
 * @throw       kcs::system::system_exception : If the system is not suported an exception in
 *              thrown.
 */
bool is_directory(const stdfs::path& pathname, std::error_code *error_code = nullptr);


/**
 * @brief       Checks if the given path corresponds to a regular file.
 * @param       pathname : Path to check.
 * @param       error_code : If function fails it holds the platform-dependent error code.
 * @return      If function was successful true is returned, otherwise false is returned and errno
 *              may be set to indicate the error.
 * @throw       kcs::system::system_exception : If the system is not suported an exception in
 *              thrown.
 */
bool is_regular_file(const stdfs::path& pathname, std::error_code *error_code = nullptr);


/**
 * @brief       Checks whether the calling process can access the file pathname. If pathname is a
 *              symbolic link, it is dereferenced.
 * @param       pathname : The file pathname.
 * @param       modes : Specifies the accessibility check(s) to be performed.
 * @param       error_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned and errno may be set to
 *              indicate the error.
 * @throw       kcs::system::system_exception : If the system is not suported an exception in
 *              thrown.
 */
bool access(
        const stdfs::path& pathname,
        access_modes modes,
        std::error_code *error_code = nullptr
);


/**
 * @brief       Checks whether the calling process can access the file pathname. If pathname is a
 *              symbolic link, it is dereferenced.
 * @param       pathname : The file pathname.
 * @param       types : The file type.
 * @param       modes : Specifies the accessibility check(s) to be performed.
 * @param       error_code : If function fails it holds the platform-dependent error code.
 * @return      On success the true is returned, otherwise false is returned and errno may be set to
 *              indicate the error.
 * @throw       kcs::system::system_exception : If the system is not suported an exception in
 *              thrown.
 */
bool access(
        const stdfs::path& pathname,
        file_types types,
        access_modes modes,
        std::error_code *error_code = nullptr
);


/**
 * @brief       Change the current execution directory.
 * @param       path : The path of the new current directory.
 * @return      If function was successful true is returned, otherwise false is returned and and
 *              errno may be set to indicate the error.
 * @throw       kcs::system::system_exception : If the system is not suported an exception in
 *              thrown.
 */
bool chdir(const stdfs::path& path);


}
}


/** @cond */
namespace kcs {
namespace lowlevel {
template<>
struct enum_bitwise_operators<kcs::system::file_types>
{
    static constexpr bool enable = true;
};
}
}
/** @endcond */


/** @cond */
namespace kcs {
namespace lowlevel {
template<>
struct enum_bitwise_operators<kcs::system::access_modes>
{
    static constexpr bool enable = true;
};
}
}
/** @endcond */


#endif
