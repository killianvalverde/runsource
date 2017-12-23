/* Copyright (C) 2017 Killian Poulaud.
   This file is part of runsource.

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

//
// Created by Killian on 22/05/17.
//

#ifndef RUNSOURCE_PROGRAM_HPP
#define RUNSOURCE_PROGRAM_HPP

#include <experimental/filesystem>
#include <unordered_set>
#include <vector>

#include "c_standard.hpp"
#include "cpp_standard.hpp"
#include "language.hpp"
#include "tool_chain.hpp"


namespace runsource {


namespace stdfs = std::experimental::filesystem;
namespace rs = runsource;


class program
{
public:
    program(
            bool exec,
            rs::language lang,
            rs::c_standard c_std,
            rs::cpp_standard cpp_std,
            bool optmz,
            rs::tool_chain tool_chn,
            std::string comp_args,
            std::string prog_args,
            bool monotonic_chrn,
            std::vector<stdfs::path> fles
    );
    
    int execute() const;

private:
    bool is_c() const noexcept;
    
    bool is_cpp() const noexcept;
    
    bool is_bash() const noexcept;
    
    bool is_python() const noexcept;
    
    int gcc_build_c(const std::string& out_nme, bool verb) const;
    
    int gcc_execute_c() const;
    
    int gcc_build_cpp(const std::string& out_nme, bool verb) const;
    
    int gcc_execute_cpp() const;
    
    int execute_bash() const;
    
    int execute_python() const;
    
    void add_c_libs_to_link_from_file(
            const stdfs::path& fle_path,
            std::vector<std::string>& libs_to_link
    ) const;

private:
    bool exec_;
    
    rs::language lang_;
    
    rs::c_standard c_std_;
    
    rs::cpp_standard cpp_std_;
    
    bool optmz_;
    
    rs::tool_chain tool_chn_;
    
    std::string comp_args_;
    
    std::string prog_args_;
    
    bool monotonic_chrn_;
    
    std::vector<stdfs::path> fles_;
    
    static std::unordered_set<std::string> c_exts_;
    
    static std::unordered_set<std::string> cpp_exts_;
    
    static std::unordered_set<std::string> bash_exts_;
    
    static std::unordered_set<std::string> python_exts_;
};


}


#endif
