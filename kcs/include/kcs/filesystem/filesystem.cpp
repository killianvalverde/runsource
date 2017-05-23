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
 * @file        kcs/filesystem/filesystem.cpp
 * @brief       filesystem functions source.
 * @author      Killian
 * @date        2016/01/08 - 20:04
 */

#include "filesystem.hpp"


namespace kcs {
namespace filesystem {


bool can_current_process_read_dir(
        const std::experimental::filesystem::path& dir_path,
        std::error_condition& error_condition
) noexcept
{
    namespace stdfs = std::experimental::filesystem;
    
    std::error_code error_code_aux;
    stdfs::perms dir_permissions = stdfs::status(dir_path, error_code_aux).permissions();
    auto uid = kcs::system::get_uid();
    auto gid = kcs::system::get_gid();
    auto file_uid = kcs::system::get_path_uid(dir_path);
    auto file_gid = kcs::system::get_path_gid(dir_path);
    
    if (!stdfs::exists(dir_path, error_code_aux))
    {
        if (error_code_aux.value() != 0)
        {
            error_condition = error_code_aux.default_error_condition();
        }
        else
        {
            error_condition = std::system_category().default_error_condition(ENOENT);
        }
        return false;
    }
    
    if (!stdfs::is_directory(dir_path, error_code_aux))
    {
        if (error_code_aux.value() != 0)
        {
            error_condition = error_code_aux.default_error_condition();
        }
        else
        {
            error_condition = std::system_category().default_error_condition(ENOTDIR);
        }
        return false;
    }
    
    if (uid == file_uid)
    {
        if ((dir_permissions & stdfs::perms::owner_read) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    else if (gid == file_gid)
    {
        if ((dir_permissions & stdfs::perms::group_read) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    else
    {
        if ((dir_permissions & stdfs::perms::others_read) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    
    error_condition.assign(0, std::generic_category());
    return true;
}


bool can_current_process_write_dir(
        const std::experimental::filesystem::path& dir_path,
        std::error_condition& error_condition
) noexcept
{
    namespace stdfs = std::experimental::filesystem;
    
    std::error_code error_condition_aux;
    stdfs::perms dir_permissions = stdfs::status(dir_path, error_condition_aux).permissions();
    auto uid = kcs::system::get_uid();
    auto gid = kcs::system::get_gid();
    auto file_uid = kcs::system::get_path_uid(dir_path);
    auto file_gid = kcs::system::get_path_gid(dir_path);
    
    if (!stdfs::exists(dir_path, error_condition_aux))
    {
        if (error_condition_aux.value() != 0)
        {
            error_condition = error_condition_aux.default_error_condition();
        }
        else
        {
            error_condition = std::system_category().default_error_condition(ENOENT);
        }
        return false;
    }
    
    if (!stdfs::is_directory(dir_path, error_condition_aux))
    {
        if (error_condition_aux.value() != 0)
        {
            error_condition = error_condition_aux.default_error_condition();
        }
        else
        {
            error_condition = std::system_category().default_error_condition(ENOTDIR);
        }
        return false;
    }
    
    if (uid == file_uid)
    {
        if ((dir_permissions & stdfs::perms::owner_write) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    else if (gid == file_gid)
    {
        if ((dir_permissions & stdfs::perms::group_write) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    else
    {
        if ((dir_permissions & stdfs::perms::others_write) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    
    error_condition.assign(0, std::generic_category());
    return true;
}


bool can_current_process_execute_dir(
        const std::experimental::filesystem::path& dir_path,
        std::error_condition& error_condition
) noexcept
{
    namespace stdfs = std::experimental::filesystem;
    
    std::error_code error_condition_aux;
    stdfs::perms dir_permissions = stdfs::status(dir_path, error_condition_aux).permissions();
    auto uid = kcs::system::get_uid();
    auto gid = kcs::system::get_gid();
    auto file_uid = kcs::system::get_path_uid(dir_path);
    auto file_gid = kcs::system::get_path_gid(dir_path);
    
    if (!stdfs::exists(dir_path, error_condition_aux))
    {
        if (error_condition_aux.value() != 0)
        {
            error_condition = error_condition_aux.default_error_condition();
        }
        else
        {
            error_condition = std::system_category().default_error_condition(ENOENT);
        }
        return false;
    }
    
    if (!stdfs::is_directory(dir_path, error_condition_aux))
    {
        if (error_condition_aux.value() != 0)
        {
            error_condition = error_condition_aux.default_error_condition();
        }
        else
        {
            error_condition = std::system_category().default_error_condition(ENOTDIR);
        }
        return false;
    }
    
    if (uid == file_uid)
    {
        if ((dir_permissions & stdfs::perms::owner_exec) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    else if (gid == file_gid)
    {
        if ((dir_permissions & stdfs::perms::group_exec) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    else
    {
        if ((dir_permissions & stdfs::perms::others_exec) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    
    error_condition.assign(0, std::generic_category());
    return true;
}


bool can_current_process_read_file(
        const std::experimental::filesystem::path& file_path,
        std::error_condition& error_condition
) noexcept
{
    namespace stdfs = std::experimental::filesystem;
    
    std::error_code error_condition_aux;
    stdfs::perms file_permissions = stdfs::status(file_path, error_condition_aux).permissions();
    auto uid = kcs::system::get_uid();
    auto gid = kcs::system::get_gid();
    auto file_uid = kcs::system::get_path_uid(file_path);
    auto file_gid = kcs::system::get_path_gid(file_path);
    
    if (!stdfs::exists(file_path, error_condition_aux))
    {
        if (error_condition_aux.value() != 0)
        {
            error_condition = error_condition_aux.default_error_condition();
        }
        else
        {
            error_condition = std::system_category().default_error_condition(ENOENT);
        }
        return false;
    }
    
    if (!stdfs::is_regular_file(file_path, error_condition_aux))
    {
        if (error_condition_aux.value() != 0)
        {
            error_condition = error_condition_aux.default_error_condition();
        }
        else
        {
            error_condition = std::system_category().default_error_condition(EINVAL);
        }
        return false;
    }
    
    if (uid == file_uid)
    {
        if ((file_permissions & stdfs::perms::owner_read) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    else if (gid == file_gid)
    {
        if ((file_permissions & stdfs::perms::group_read) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    else
    {
        if ((file_permissions & stdfs::perms::others_read) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    
    error_condition.assign(0, std::generic_category());
    return true;
}


bool can_current_process_write_file(
        const std::experimental::filesystem::path& file_path,
        std::error_condition& error_condition
) noexcept
{
    namespace stdfs = std::experimental::filesystem;
    
    std::error_code error_condition_aux;
    stdfs::perms file_permissions = stdfs::status(file_path, error_condition_aux).permissions();
    auto uid = kcs::system::get_uid();
    auto gid = kcs::system::get_gid();
    auto file_uid = kcs::system::get_path_uid(file_path);
    auto file_gid = kcs::system::get_path_gid(file_path);
    
    if (!stdfs::exists(file_path, error_condition_aux))
    {
        if (error_condition_aux.value() != 0)
        {
            error_condition = error_condition_aux.default_error_condition();
        }
        else
        {
            error_condition = std::system_category().default_error_condition(ENOENT);
        }
        return false;
    }
    
    if (!stdfs::is_regular_file(file_path, error_condition_aux))
    {
        if (error_condition_aux.value() != 0)
        {
            error_condition = error_condition_aux.default_error_condition();
        }
        else
        {
            error_condition = std::system_category().default_error_condition(EINVAL);
        }
        return false;
    }
    
    if (uid == file_uid)
    {
        if ((file_permissions & stdfs::perms::owner_write) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    else if (gid == file_gid)
    {
        if ((file_permissions & stdfs::perms::group_write) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    else
    {
        if ((file_permissions & stdfs::perms::others_write) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    
    error_condition.assign(0, std::generic_category());
    return true;
}


bool can_current_process_write_or_create_file(
        const std::experimental::filesystem::path& file_path,
        std::error_condition& error_condition
) noexcept
{
    if (!can_current_process_write_file(file_path, error_condition))
    {
        if (error_condition.value() != ENOENT ||
            !can_current_process_execute_dir(file_path.parent_path(), error_condition))
        {
            return false;
        }
    }
    
    return true;
}


bool can_current_process_execute_file(
        const std::experimental::filesystem::path& file_path,
        std::error_condition& error_condition
) noexcept
{
    namespace stdfs = std::experimental::filesystem;
    
    std::error_code error_condition_aux;
    stdfs::perms file_permissions = stdfs::status(file_path, error_condition_aux).permissions();
    auto uid = kcs::system::get_uid();
    auto gid = kcs::system::get_gid();
    auto file_uid = kcs::system::get_path_uid(file_path);
    auto file_gid = kcs::system::get_path_gid(file_path);
    
    if (!stdfs::exists(file_path, error_condition_aux))
    {
        if (error_condition_aux.value() != 0)
        {
            error_condition = error_condition_aux.default_error_condition();
        }
        else
        {
            error_condition = std::system_category().default_error_condition(ENOENT);
        }
        return false;
    }
    
    if (!stdfs::is_regular_file(file_path, error_condition_aux))
    {
        if (error_condition_aux.value() != 0)
        {
            error_condition = error_condition_aux.default_error_condition();
        }
        else
        {
            error_condition = std::system_category().default_error_condition(EINVAL);
        }
        return false;
    }
    
    if (uid == file_uid)
    {
        if ((file_permissions & stdfs::perms::owner_exec) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    else if (gid == file_gid)
    {
        if ((file_permissions & stdfs::perms::group_exec) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    else
    {
        if ((file_permissions & stdfs::perms::others_exec) == stdfs::perms::none)
        {
            error_condition = std::system_category().default_error_condition(EACCES);
            return false;
        }
    }
    
    error_condition.assign(0, std::generic_category());
    return true;
}


}
}
