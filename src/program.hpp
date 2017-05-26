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

#include "kcs/argparse.hpp"
#include "c_standard.hpp"
#include "cpp_standard.hpp"
#include "language.hpp"
#include "tool_chain.hpp"


namespace runsource {


namespace stdfs = std::experimental::filesystem;
namespace kap = kcs::argparse;
namespace rs = runsource;


class program
{
public:
    program(
            bool execute,
            rs::language language,
            rs::c_standard c_standard,
            rs::cpp_standard cpp_standard,
            rs::tool_chain tool_chain,
            std::vector<std::string> compiler_args,
            std::vector<std::string> program_args,
            std::vector<stdfs::path> files
    );
    
    int exec() const;

private:
    bool is_c() const noexcept;
    bool is_cpp() const noexcept;
    bool is_bash() const noexcept;
    bool is_python() const noexcept;
    int gcc_build_c(std::string output_name, bool verbose) const;
    int gcc_exec_c() const;
    int gcc_build_cpp(std::string output_name, bool verbose) const;
    int gcc_exec_cpp() const;
    int exec_bash() const;
    int exec_python() const;

private:
    bool execute_;
    rs::language language_;
    rs::c_standard c_standard_;
    rs::cpp_standard cpp_standard_;
    rs::tool_chain tool_chain_;
    std::vector<std::string> compiler_args_;
    std::vector<std::string> program_args_;
    std::vector<stdfs::path> files_;
    
    static std::unordered_set<std::string> c_extensions_;
    static std::unordered_set<std::string> cpp_extensions_;
    static std::unordered_set<std::string> bash_extensions_;
    static std::unordered_set<std::string> python_extensions_;
};


}


#endif
