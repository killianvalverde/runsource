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

#include <unistd.h>
#include <iomanip>
#include <iostream>

#include "program.hpp"

namespace stdfs = std::experimental::filesystem;
namespace rs = runsource;


namespace runsource {


program::program(
        bool execute,
        rs::language language,
        rs::c_standard c_standard,
        rs::cpp_standard cpp_standard,
        rs::tool_chain tool_chain,
        std::vector<std::string> compiler_args,
        std::vector<std::string> program_args,
        std::vector<stdfs::path> files
)
        : execute_(execute)
        , language_(language)
        , c_standard_(c_standard)
        , cpp_standard_(cpp_standard)
        , tool_chain_(tool_chain)
        , compiler_args_(std::move(compiler_args))
        , program_args_(std::move(program_args))
        , files_(std::move(files))
{
    if (language == rs::language::NIL)
    {
        if (is_c())
        {
            language_ = rs::language::C;
        }
        else if (is_cpp())
        {
            language_ = rs::language::CPP;
        }
        else if (is_python())
        {
            language_ = rs::language::PYTHON;
        }
        else if (is_bash())
        {
            language_ = rs::language::BASH;
        }
    }
}


int program::exec() const
{
    chdir(files_.front().parent_path().string().c_str());
    
    switch (language_)
    {
        case rs::language::C:
            if (tool_chain_ == tool_chain::GCC)
            {
                return execute_ ? gcc_exec_c() : gcc_build_c(std::string(), true);
            }
            break;
        
        case rs::language::CPP:
            if (tool_chain_ == tool_chain::GCC)
            {
                return execute_ ? gcc_exec_cpp() : gcc_build_cpp(std::string(), true);
            }
            break;
        
        case rs::language::BASH:
            return exec_bash();
        
        case rs::language::PYTHON:
            return exec_python();
    }
    
    return -1;
}


bool program::is_c() const noexcept
{
    for (auto& x : files_)
    {
        if (c_extensions_.count(x.extension()) == 0)
        {
            return false;
        }
    }
    
    return true;
}


bool program::is_cpp() const noexcept
{
    for (auto& x : files_)
    {
        if (cpp_extensions_.count(x.extension()) == 0)
        {
            return false;
        }
    }
    
    return true;
}


bool program::is_bash() const noexcept
{
    for (auto& x : files_)
    {
        if (bash_extensions_.count(x.extension()) == 0 && x.has_extension())
        {
            return false;
        }
    }
    
    return true;
}


bool program::is_python() const noexcept
{
    for (auto& x : files_)
    {
        if (python_extensions_.count(x.extension()) == 0)
        {
            return false;
        }
    }
    
    return true;
}


int program::gcc_build_c(std::string output_name, bool verbose) const
{
    std::clock_t start_time;
    double total_duration;
    int result;
    std::string command = "gcc ";
    
    for (auto& x : files_)
    {
        command += "\"";
        command += x.string();
        command += "\"";
        command += ' ';
    }
    
    command += "-o ";
    if (output_name.empty())
    {
        output_name = files_.front().string();
        output_name = output_name.substr(0, output_name.find_last_of("."));
    }
    command += "\"";
    command += std::move(output_name);
    command += "\"";
    command += ' ';
    
    for (auto& x : compiler_args_)
    {
        command += x;
        command += ' ';
    }
    
    if (c_standard_ != rs::c_standard::NIL)
    {
        if (c_standard_ == rs::c_standard::C89)
        {
            command += "-std=c89";
        }
        else if (c_standard_ == rs::c_standard::C90)
        {
            command += "-std=c90";
        }
        else if (c_standard_ == rs::c_standard::C99)
        {
            command += "-std=c99";
        }
        else if (c_standard_ == rs::c_standard::C11)
        {
            command += "-std=c11";
        }
    }
    
    start_time = clock();
    result = system(command.c_str());
    total_duration = (clock() - start_time) / CLOCKS_PER_SEC;
    
    if (verbose && result == 0)
    {
        std::cout << "C program build in "
                  << std::setprecision(3) << total_duration
                  << " seconds"
                  << std::endl;
    }
    
    return result;
}


int program::gcc_exec_c() const
{
    std::string output_name = std::tmpnam(nullptr);
    std::string command;
    int build_result = gcc_build_c(output_name, false);
    int exec_result;
    std::clock_t start_time;
    double total_duration;
    std::stringstream strstream;
    std::string strstream_str;
    
    if (build_result == 0)
    {
        command += "\"";
        command += output_name;
        command += "\"";
        command += ' ';
        
        for (auto& x : program_args_)
        {
            command += x;
            command += ' ';
        }
        
        start_time = clock();
        exec_result = system(command.c_str());
        total_duration = (clock() - start_time) / CLOCKS_PER_SEC;
        
        strstream << "Process exited after "
                  << std::setprecision(3) << total_duration
                  << " seconds with return value " << exec_result;
        
        strstream_str = strstream.str();
        
        std::cout << "\n";
        for (std::size_t i = 0; i < strstream_str.size(); i++)
        {
            std::cout << "-";
        }
        std::cout << "\n"
                  << strstream_str
                  << std::endl;
        
        remove(output_name.c_str());
        
        return exec_result;
    }
    else
    {
        return build_result;
    }
}


int program::gcc_build_cpp(std::string output_name, bool verbose) const
{
    std::clock_t start_time;
    double total_duration;
    int result;
    std::string command = "g++ ";
    
    for (auto& x : files_)
    {
        command += "\"";
        command += x.string();
        command += "\"";
        command += ' ';
    }
    
    command += "-o ";
    if (output_name.empty())
    {
        output_name = files_.front().string();
        output_name = output_name.substr(0, output_name.find_last_of("."));
    }
    command += "\"";
    command += std::move(output_name);
    command += "\"";
    command += ' ';
    
    for (auto& x : compiler_args_)
    {
        command += x;
        command += ' ';
    }
    
    if (cpp_standard_ != rs::cpp_standard::NIL)
    {
        if (cpp_standard_ == rs::cpp_standard::CPP98)
        {
            command += "-std=c++98";
        }
        else if (cpp_standard_ == rs::cpp_standard::CPP03)
        {
            command += "-std=c++03";
        }
        else if (cpp_standard_ == rs::cpp_standard::CPP11)
        {
            command += "-std=c++11";
        }
        else if (cpp_standard_ == rs::cpp_standard::CPP14)
        {
            command += "-std=c++14";
        }
        else if (cpp_standard_ == rs::cpp_standard::CPP17)
        {
            command += "-std=c++17";
        }
    }
    
    start_time = clock();
    result = system(command.c_str());
    total_duration = (clock() - start_time) / CLOCKS_PER_SEC;
    
    if (verbose && result == 0)
    {
        std::cout << "C++ program build in "
                  << std::setprecision(3) << total_duration
                  << " seconds"
                  << std::endl;
    }
    
    return result;
}


int program::gcc_exec_cpp() const
{
    std::string output_name = std::tmpnam(nullptr);
    std::string command;
    int build_result = gcc_build_cpp(output_name, false);
    int exec_result;
    std::clock_t start_time;
    double total_duration;
    std::stringstream strstream;
    std::string strstream_str;
    
    if (build_result == 0)
    {
        command += "\"";
        command += output_name;
        command += "\"";
        command += ' ';
        
        for (auto& x : program_args_)
        {
            command += x;
            command += ' ';
        }
        
        start_time = clock();
        exec_result = system(command.c_str());
        total_duration = (clock() - start_time) / CLOCKS_PER_SEC;
        
        strstream << "Process exited after "
                  << std::setprecision(3) << total_duration
                  << " seconds with return value " << exec_result;
        
        strstream_str = strstream.str();
        
        std::cout << "\n";
        for (std::size_t i = 0; i < strstream_str.size(); i++)
        {
            std::cout << "-";
        }
        std::cout << "\n"
                  << strstream_str
                  << std::endl;
        
        remove(output_name.c_str());
        
        return exec_result;
    }
    else
    {
        return build_result;
    }
}


int program::exec_bash() const
{
    std::string command = "bash ";
    std::clock_t start_time;
    double total_duration;
    int exec_result;
    std::stringstream strstream;
    std::string strstream_str;
    
    for (auto& x : files_)
    {
        command += "\"";
        command += x.string();
        command += "\"";
        command += ' ';
    }
    
    for (auto& x : program_args_)
    {
        command += x;
        command += ' ';
    }
    
    start_time = clock();
    exec_result = system(command.c_str());
    total_duration = (clock() - start_time) / CLOCKS_PER_SEC;
    
    if (exec_result == 0)
    {
        strstream << "Process exited after "
                  << std::setprecision(3) << total_duration
                  << " seconds with return value " << exec_result;
        
        strstream_str = strstream.str();
        
        std::cout << "\n";
        for (std::size_t i = 0; i < strstream_str.size(); i++)
        {
            std::cout << "-";
        }
        std::cout << "\n"
                  << strstream_str
                  << std::endl;
    }
    
    return exec_result;
}


int program::exec_python() const
{
    std::string command = "python ";
    std::clock_t start_time;
    double total_duration;
    int exec_result;
    std::stringstream strstream;
    std::string strstream_str;
    
    for (auto& x : files_)
    {
        command += "\"";
        command += x.string();
        command += "\"";
        command += ' ';
    }
    
    for (auto& x : program_args_)
    {
        command += x;
        command += ' ';
    }
    
    start_time = clock();
    exec_result = system(command.c_str());
    total_duration = (clock() - start_time) / CLOCKS_PER_SEC;
    
    if (exec_result == 0)
    {
        strstream << "Process exited after "
                  << std::setprecision(3) << total_duration
                  << " seconds with return value " << exec_result;
    
        strstream_str = strstream.str();
    
        std::cout << "\n";
        for (std::size_t i = 0; i < strstream_str.size(); i++)
        {
            std::cout << "-";
        }
        std::cout << "\n"
                  << strstream_str
                  << std::endl;
    }
    
    return exec_result;
}


std::unordered_set<std::string> program::c_extensions_ =
        {".c"};


std::unordered_set<std::string> program::cpp_extensions_ =
        {".cpp", ".cc", ".C", ".CPP", ".c++", ".cp", ".cxx"};


std::unordered_set<std::string> program::bash_extensions_ =
        {".sh"};


std::unordered_set<std::string> program::python_extensions_ =
        {".py"};
    
    
}
