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

#include "runbuild_task.hpp"


namespace runsource {


runbuild_task::runbuild_task(const kap::arg_parser& ap)
        : files_()
        , build_(ap.arg_found("--build"))
        , language_(language::NIL)
        , c_standard_()
        , cpp_standard_()
        , compiler_args_()
        , program_args_()
{
    for (auto& x : ap.get_arg_values("SOURCE-FILE"))
    {
        files_.push_back(x.type_cast<stdfs::path>());
    }
    
    tool_chain_ = tool_chain::GCC;
    
    if (ap.arg_found("--clanguage"))
    {
        language_ = language::C;
    }
    else if (ap.arg_found("--c++language"))
    {
        language_ = language::CPP;
    }
    else if (ap.arg_found("--bash"))
    {
        language_ = language::BASH;
    }
    else if (ap.arg_found("--python"))
    {
        language_ = language::PYTHON;
    }
    else if (is_c())
    {
        language_ = language::C;
    }
    else if (is_cpp())
    {
        language_ = language::CPP;
    }
    else if (is_python())
    {
        language_ = language::PYTHON;
    }
    else if (is_bash())
    {
        language_ = language::BASH;
    }
    
    if (tool_chain_ == tool_chain::GCC)
    {
        if (ap.arg_found("--c89"))
        {
            c_standard_ = "-std=c89";
        }
        else if (ap.arg_found("--c90"))
        {
            c_standard_ = "-std=c90";
        }
        else if (ap.arg_found("--c99"))
        {
            c_standard_ = "-std=c99";
        }
        else //if (ap.arg_found("-c11"))
        {
            c_standard_ = "-std=c11";
        }
    
        if (ap.arg_found("--c++98"))
        {
            cpp_standard_ = "-std=c++98";
        }
        else if (ap.arg_found("--c++03"))
        {
            cpp_standard_ = "-std=c++03";
        }
        else if (ap.arg_found("--c++11"))
        {
            cpp_standard_ = "-std=c++11";
        }
        else if (ap.arg_found("--c++14"))
        {
            cpp_standard_ = "-std=c++14";
        }
        else //if (ap.arg_found("-c++17"))
        {
            cpp_standard_ = "-std=c++17";
        }
    }
    
    for (auto& x : ap.get_arg_values("--compiler-args", std::nothrow))
    {
        compiler_args_.push_back(x.get_value());
    }
    
    for (auto& x : ap.get_arg_values("--program-args", std::nothrow))
    {
        program_args_.push_back(x.get_value());
    }
}


bool runbuild_task::is_c() const noexcept
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


bool runbuild_task::is_cpp() const noexcept
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


bool runbuild_task::is_bash() const noexcept
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


bool runbuild_task::is_python() const noexcept
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


int runbuild_task::do_operation() const
{
    switch (language_)
    {
        case language::C:
            if (tool_chain_ == tool_chain::GCC)
            {
                return build_ ? gcc_build_c(std::string(), true) : gcc_exec_c();
            }
            break;
        
        case language::CPP:
            if (tool_chain_ == tool_chain::GCC)
            {
                return build_ ? gcc_build_cpp(std::string(), true) : gcc_exec_cpp();
            }
            break;
    
        case language::BASH:
            return exec_bash();
    
        case language::PYTHON:
            return exec_python();
    }
    
    return -1;
}


int runbuild_task::gcc_build_c(std::string output_name, bool verbose) const
{
    std::clock_t start_time;
    double total_duration;
    int result;
    std::string command = "gcc ";
    
    command += c_standard_;
    command += ' ';
    
    for (auto& x : compiler_args_)
    {
        command += x;
        command += ' ';
    }
    
    for (auto& x : files_)
    {
        command += x.string();
        command += ' ';
    }
    command += "-o ";
    
    if (output_name.empty())
    {
        output_name = files_.front().string();
        output_name = output_name.substr(0, output_name.find_last_of("."));
    }
    command += std::move(output_name);
    
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


int runbuild_task::gcc_exec_c() const
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
        command = output_name;
        if (!program_args_.empty())
        {
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


int runbuild_task::gcc_build_cpp(std::string output_name, bool verbose) const
{
    std::clock_t start_time;
    double total_duration;
    int result;
    std::string command = "g++ ";
    
    command += cpp_standard_;
    command += ' ';
    
    for (auto& x : compiler_args_)
    {
        command += x;
        command += ' ';
    }
    
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
        output_name += "\"";
        output_name += files_.front().string();
        output_name = output_name.substr(0, output_name.find_last_of("."));
        output_name += "\"";
    }
    command += std::move(output_name);
    
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


int runbuild_task::gcc_exec_cpp() const
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
        command = output_name;
        if (!program_args_.empty())
        {
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


int runbuild_task::exec_bash() const
{
    std::string command = "bash ";
    std::clock_t start_time;
    double total_duration;
    int exec_result;
    std::stringstream strstream;
    std::string strstream_str;
    
    for (auto& x : program_args_)
    {
        command += x;
        command += ' ';
    }
    
    for (auto& x : files_)
    {
        command += "\"";
        command += x.string();
        command += "\"";
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


int runbuild_task::exec_python() const
{
    std::string command = "python ";
    std::clock_t start_time;
    double total_duration;
    int exec_result;
    std::stringstream strstream;
    std::string strstream_str;
    
    for (auto& x : program_args_)
    {
        command += x;
        command += ' ';
    }
    
    for (auto& x : files_)
    {
        command += "\"";
        command += x.string();
        command += "\"";
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


std::unordered_set<std::string> runbuild_task::c_extensions_ =
        {".c"};


std::unordered_set<std::string> runbuild_task::cpp_extensions_ =
        {".cpp", ".cc", ".C", ".CPP", ".c++", ".cp", ".cxx"};


std::unordered_set<std::string> runbuild_task::bash_extensions_ =
        {".sh"};


std::unordered_set<std::string> runbuild_task::python_extensions_ =
        {".py"};
    
    
}
