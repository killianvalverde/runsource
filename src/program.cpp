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

#include <iomanip>
#include <iostream>
#include <fstream>
#include <regex>

#include <kboost/kboost.hpp>

#include "program.hpp"


namespace runsource {


namespace stdfs = std::experimental::filesystem;
namespace rs = runsource;


program::program(
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
)
        : exec_(exec)
        , lang_(lang)
        , c_std_(c_std)
        , cpp_std_(cpp_std)
        , optmz_(optmz)
        , tool_chn_(tool_chn)
        , comp_args_(std::move(comp_args))
        , prog_args_(std::move(prog_args))
        , monotonic_chrn_(monotonic_chrn)
        , fles_(std::move(fles))
{
    if (lang == rs::language::NIL)
    {
        if (is_c())
        {
            lang_ = rs::language::C;
        }
        else if (is_cpp())
        {
            lang_ = rs::language::CPP;
        }
        else if (is_python())
        {
            lang_ = rs::language::PYTHON;
        }
        else if (is_bash())
        {
            lang_ = rs::language::BASH;
        }
    }
}


int program::execute() const
{
    ksys::chdir(fles_.front().parent_path().c_str());
    
    switch (lang_)
    {
        case rs::language::C:
            if (tool_chn_ == tool_chain::GCC)
            {
                return exec_ ? gcc_execute_c() : gcc_build_c(std::string(), true);
            }
            break;
        
        case rs::language::CPP:
            if (tool_chn_ == tool_chain::GCC)
            {
                return exec_ ? gcc_execute_cpp() : gcc_build_cpp(std::string(), true);
            }
            break;
        
        case rs::language::BASH:
            return execute_bash();
        
        case rs::language::PYTHON:
            return execute_python();
    }
    
    return -1;
}


bool program::is_c() const noexcept
{
    for (auto& x : fles_)
    {
        if (c_exts_.count(x.extension()) == 0)
        {
            return false;
        }
    }
    
    return true;
}


bool program::is_cpp() const noexcept
{
    for (auto& x : fles_)
    {
        if (cpp_exts_.count(x.extension()) == 0)
        {
            return false;
        }
    }
    
    return true;
}


bool program::is_bash() const noexcept
{
    for (auto& x : fles_)
    {
        if (bash_exts_.count(x.extension()) == 0 && x.has_extension())
        {
            return false;
        }
    }
    
    return true;
}


bool program::is_python() const noexcept
{
    for (auto& x : fles_)
    {
        if (python_exts_.count(x.extension()) == 0)
        {
            return false;
        }
    }
    
    return true;
}


int program::gcc_build_c(const std::string& out_nme, bool verb) const
{
    int result;
    std::string command = "gcc ";
    std::vector<std::string> libs_to_link;
    ktime::monotonic_chrono monotonic_chrn;
    
    for (auto& x : fles_)
    {
        add_c_libs_to_link_from_file(x, libs_to_link);
        
        command += "\"";
        command += x.string();
        command += "\"";
        command += ' ';
    }
    
    command += "-o ";
    command += "\"";
    command += (out_nme.empty()) ? fles_.front().stem().string() : out_nme;
    command += "\"";
    command += ' ';
    
    if (!comp_args_.empty())
    {
        command += comp_args_;
        command += ' ';
    }
    
    for (auto& x : libs_to_link)
    {
        command += x;
        command += ' ';
    }
    
    if (c_std_ != rs::c_standard::NIL)
    {
        if (c_std_ == rs::c_standard::C89)
        {
            command += "-std=c89 ";
        }
        else if (c_std_ == rs::c_standard::C90)
        {
            command += "-std=c90 ";
        }
        else if (c_std_ == rs::c_standard::C99)
        {
            command += "-std=c99 ";
        }
        else if (c_std_ == rs::c_standard::C11)
        {
            command += "-std=c11 ";
        }
    }
    
    if (optmz_)
    {
        command += "-O3 ";
    }
    
    monotonic_chrn.start();
    result = system(command.c_str());
    monotonic_chrn.stop();
    
    if (verb && result == 0)
    {
        std::cout << "C program build in "
                  << std::setprecision(3)
                  << std::fixed
                  << monotonic_chrn
                  << " seconds"
                  << std::endl;
    }
    
    return result;
}


int program::gcc_execute_c() const
{
    std::string output_name;
    std::string command;
    int build_result;
    int exec_result;
    std::stringstream strstream;
    std::string strstream_str;
    ktime::monotonic_chrono monotonic_chrn;
    
    output_name = std::tmpnam(nullptr);
    output_name += "runsource";
    build_result = gcc_build_c(output_name, false);
    
    if (build_result == 0)
    {
        command += "\"";
        command += output_name;
        command += "\"";
        command += ' ';
        
        if (!prog_args_.empty())
        {
            command += prog_args_;
            command += ' ';
        }
        
        monotonic_chrn.start();
        exec_result = system(command.c_str());
        monotonic_chrn.stop();
        remove(output_name.c_str());
        
        strstream << "Process exited after "
                  << std::setprecision(3)
                  << std::fixed
                  << monotonic_chrn
                  << " seconds with return value " << exec_result;
        
        strstream_str = strstream.str();
        
        std::cout << std::endl;
        for (std::size_t i = 0; i < strstream_str.size(); i++)
        {
            std::cout << "-";
        }
        std::cout << std::endl
                  << strstream_str
                  << std::endl;
        
        return exec_result;
    }
    else
    {
        return build_result;
    }
}


int program::gcc_build_cpp(const std::string& out_nme, bool verb) const
{
    int result;
    std::string command = "g++ ";
    std::vector<std::string> libs_to_link;
    ktime::monotonic_chrono monotonic_chrn;
    
    for (auto& x : fles_)
    {
        add_c_libs_to_link_from_file(x, libs_to_link);
        
        command += "\"";
        command += x.string();
        command += "\"";
        command += ' ';
    }
    
    command += "-o ";
    command += "\"";
    command += (out_nme.empty()) ? fles_.front().stem().string() : out_nme;
    command += "\"";
    command += ' ';
    
    if (!comp_args_.empty())
    {
        command += comp_args_;
        command += ' ';
    }
    
    for (auto& x : libs_to_link)
    {
        command += x;
        command += ' ';
    }
    
    if (cpp_std_ != rs::cpp_standard::NIL)
    {
        if (cpp_std_ == rs::cpp_standard::CPP98)
        {
            command += "-std=c++98 ";
        }
        else if (cpp_std_ == rs::cpp_standard::CPP03)
        {
            command += "-std=c++03 ";
        }
        else if (cpp_std_ == rs::cpp_standard::CPP11)
        {
            command += "-std=c++11 ";
        }
        else if (cpp_std_ == rs::cpp_standard::CPP14)
        {
            command += "-std=c++14 ";
        }
        else if (cpp_std_ == rs::cpp_standard::CPP17)
        {
            command += "-std=c++17 ";
        }
    }
    
    if (optmz_)
    {
        command += "-O3 ";
    }
    
    monotonic_chrn.start();
    result = system(command.c_str());
    monotonic_chrn.stop();
    
    if (verb && result == 0)
    {
        std::cout << "C++ program build in "
                  << std::setprecision(3)
                  << std::fixed
                  << monotonic_chrn
                  << " seconds"
                  << std::endl;
    }
    
    return result;
}


int program::gcc_execute_cpp() const
{
    std::string output_name;
    std::string command;
    int build_result;
    int exec_result;
    ktime::monotonic_chrono monotonic_chrn;
    std::stringstream strstream;
    std::string strstream_str;
    
    output_name = std::tmpnam(nullptr);
    output_name += "runsource";
    build_result = gcc_build_cpp(output_name, false);
    
    if (build_result == 0)
    {
        command += "\"";
        command += output_name;
        command += "\"";
        command += ' ';
        
        command += prog_args_;
        command += ' ';
    
        monotonic_chrn.start();
        exec_result = system(command.c_str());
        monotonic_chrn.stop();
        remove(output_name.c_str());
        
        strstream << "Process exited after "
                  << std::setprecision(3)
                  << std::fixed
                  << monotonic_chrn
                  << " seconds with return value " << exec_result;
        
        strstream_str = strstream.str();
        
        std::cout << std::endl;
        for (std::size_t i = 0; i < strstream_str.size(); i++)
        {
            std::cout << "-";
        }
        std::cout << std::endl
                  << strstream_str
                  << std::endl;
        
        return exec_result;
    }
    else
    {
        return build_result;
    }
}


int program::execute_bash() const
{
    std::string command = "bash ";
    ktime::monotonic_chrono monotonic_chrn;
    int exec_result;
    std::stringstream strstream;
    std::string strstream_str;
    
    for (auto& x : fles_)
    {
        command += "\"";
        command += x.string();
        command += "\"";
        command += ' ';
    }
    
    command += prog_args_;
    command += ' ';
    
    monotonic_chrn.start();
    exec_result = system(command.c_str());
    monotonic_chrn.stop();
    
    if (exec_result == 0)
    {
        strstream << "Process exited after "
                  << std::setprecision(3)
                  << std::fixed
                  << monotonic_chrn
                  << " seconds with return value " << exec_result;
        
        strstream_str = strstream.str();
        
        std::cout << std::endl;
        for (std::size_t i = 0; i < strstream_str.size(); i++)
        {
            std::cout << "-";
        }
        std::cout << std::endl
                  << strstream_str
                  << std::endl;
    }
    
    return exec_result;
}


int program::execute_python() const
{
    std::string command = "python ";
    ktime::monotonic_chrono monotonic_chrn;
    int exec_result;
    std::stringstream strstream;
    std::string strstream_str;
    
    for (auto& x : fles_)
    {
        command += "\"";
        command += x.string();
        command += "\"";
        command += ' ';
    }
    
    command += prog_args_;
    command += ' ';
    
    monotonic_chrn.start();
    exec_result = system(command.c_str());
    monotonic_chrn.stop();
    
    if (exec_result == 0)
    {
        strstream << "Process exited after "
                  << std::setprecision(3)
                  << std::fixed
                  << monotonic_chrn
                  << " seconds with return value " << exec_result;
    
        strstream_str = strstream.str();
    
        std::cout << std::endl;
        for (std::size_t i = 0; i < strstream_str.size(); i++)
        {
            std::cout << "-";
        }
        std::cout << std::endl
                  << strstream_str
                  << std::endl;
    }
    
    return exec_result;
}


void program::add_c_libs_to_link_from_file(
        const stdfs::path& fle_path,
        std::vector<std::string>& libs_to_link
) const
{
    std::regex rgx_pragma(R"(^#pragma\ comment\(lib,.+\)$)");
    std::regex rgx_lib(R"(\".+\")");
    std::smatch smatch;
    std::string curr_line;
    std::ifstream ifs;
    
    ifs.open(fle_path);
    if (ifs)
    {
        while ((std::getline(ifs, curr_line), !ifs.eof()))
        {
            if (std::regex_match(curr_line, rgx_pragma) &&
                std::regex_search(curr_line, smatch, rgx_lib) &&
                std::find(libs_to_link.begin(), libs_to_link.end(), smatch.str()) ==
                        libs_to_link.end())
            {
                libs_to_link.push_back(smatch.str());
            }
        }
        
        ifs.close();
    }
}


std::unordered_set<std::string> program::c_exts_ =
        {".c"};


std::unordered_set<std::string> program::cpp_exts_ =
        {".cpp", ".cc", ".C", ".CPP", ".c++", ".cp", ".cxx"};


std::unordered_set<std::string> program::bash_exts_ =
        {".sh"};


std::unordered_set<std::string> program::python_exts_ =
        {".py"};
    
    
}
