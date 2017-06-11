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
 * @file        kcs/system/filesystem.cpp
 * @brief       filesystem functions source.
 * @author      Killian
 * @date        2017/05/26 - 17:20
 */

#ifdef __unix__
#include <sys/stat.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

#include "system_exception.hpp"
#include "filesystem.hpp"


namespace kcs {
namespace system {


namespace stdfs = std::experimental::filesystem;


int get_file_uid(const stdfs::path& pathname)
{
#if _POSIX_VERSION >= 200112L
    struct ::stat stt;
    auto res = ::stat(pathname.c_str(), &stt);
    if (res == -1)
    {
        return res;
    }
    return stt.st_uid;
#else
    throw system_exception("kcs::system::system_exception: system not supported");
#endif
}


int get_file_gid(const stdfs::path& pathname)
{
#if _POSIX_VERSION >= 200112L
    struct ::stat stt;
    auto res = ::stat(pathname.c_str(), &stt);
    if (res == -1)
    {
        return res;
    }
    return stt.st_gid;
#else
    throw system_exception("kcs::system::system_exception: system not supported");
#endif
}


bool is_directory(const stdfs::path& pathname, std::error_code *error_code)
{
#if _POSIX_VERSION >= 200112L
    struct stat infos;
    if (stat(pathname.c_str(), &infos) == -1)
    {
        *error_code = std::error_code(errno, std::system_category());
        return false;
    }
    else
    {
        return S_ISDIR(infos.st_mode);
    }
#else
    throw system_exception("kcs::system::system_exception: system not supported");
#endif
}


bool is_regular_file(const stdfs::path& pathname, std::error_code *error_code)
{
#if _POSIX_VERSION >= 200112L
    struct stat infos;
    if (stat(pathname.c_str(), &infos) == -1)
    {
        *error_code = std::error_code(errno, std::system_category());
        return false;
    }
    else
    {
        return S_ISREG(infos.st_mode);
    }
#else
    throw system_exception("kcs::system::system_exception: system not supported");
#endif
}


bool access(
        const stdfs::path& pathname,
        access_modes modes,
        std::error_code *error_code
)
{
#if _POSIX_VERSION >= 200112L
    decltype(F_OK) mode_builder = 0;
    if ((modes & access_modes::EXISTS) != access_modes::NIL)
    {
        mode_builder |= F_OK;
    }
    if ((modes & access_modes::READ) != access_modes::NIL)
    {
        mode_builder |= R_OK;
    }
    if ((modes & access_modes::WRITE) != access_modes::NIL)
    {
        mode_builder |= W_OK;
    }
    if ((modes & access_modes::EXECUTE) != access_modes::NIL)
    {
        mode_builder |= X_OK;
    }
    
    int res = ::access(pathname.c_str(), mode_builder);
    if (res == -1 && error_code != nullptr)
    {
        *error_code = std::error_code(errno, std::system_category());
        return false;
    }
    
    if ((modes & access_modes::CREATE) != access_modes::NIL)
    {
        res = access(pathname.parent_path(), access_modes::EXECUTE, error_code);
        if (res == 0 && stdfs::exists(pathname))
        {
            res = access(pathname, access_modes::WRITE, error_code);
        }
    }
    
    return (res == 0) ? true : false;
#else
    throw system_exception("kcs::system::system_exception: system not supported");
#endif
}


bool access(
        const stdfs::path& pathname,
        file_types types,
        access_modes modes,
        std::error_code *error_code
)
{
    bool success = true;
    
    if ((types & file_types::BLOCK_FILE) != file_types::NIL)
    {
        if (error_code != nullptr)
        {
            success &= stdfs::is_block_file(pathname, *error_code);
        }
        else
        {
            success &= stdfs::is_block_file(pathname);
        }
    }
    if ((types & file_types::CHARACTER_FILE) != file_types::NIL)
    {
        if (error_code != nullptr)
        {
            success &= stdfs::is_character_file(pathname, *error_code);
        }
        else
        {
            success &= stdfs::is_character_file(pathname);
        }
    }
    if ((types & file_types::DIRECTORY) != file_types::NIL)
    {
        if (error_code != nullptr)
        {
            success &= stdfs::is_directory(pathname, *error_code);
        }
        else
        {
            success &= stdfs::is_directory(pathname);
        }
    }
    if ((types & file_types::FIFO) != file_types::NIL)
    {
        if (error_code != nullptr)
        {
            success &= stdfs::is_fifo(pathname, *error_code);
        }
        else
        {
            success &= stdfs::is_fifo(pathname);
        }
    }
    if ((types & file_types::REGULAR_FILE) != file_types::NIL)
    {
        if (error_code != nullptr)
        {
            success &= stdfs::is_regular_file(pathname, *error_code);
        }
        else
        {
            success &= stdfs::is_regular_file(pathname);
        }
    }
    if ((types & file_types::SOCKET) != file_types::NIL)
    {
        if (error_code != nullptr)
        {
            success &= stdfs::is_socket(pathname, *error_code);
        }
        else
        {
            success &= stdfs::is_socket(pathname);
        }
    }
    if ((types & file_types::SYMLINK) != file_types::NIL)
    {
        if (error_code != nullptr)
        {
            success &= stdfs::is_symlink(pathname, *error_code);
        }
        else
        {
            success &= stdfs::is_symlink(pathname);
        }
    }
    if (!success)
    {
        return false;
    }
    
    return access(pathname, modes, error_code);
}


bool chdir(const stdfs::path& path)
{
#if _POSIX_VERSION >= 200112L
    return (::chdir(path.c_str()) == 0) ? true : false;
#else
    throw system_exception("kcs::system::system_exception: system not supported");
#endif
}


}
}
