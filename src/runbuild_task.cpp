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
    
    if (ap.arg_found("--clanguage"))
    {
        language_ = language::C;
    }
    else if (ap.arg_found("--c++language"))
    {
        language_ = language::CPP;
    }
    else if (is_c())
    {
        language_ = language::C;
    }
    else if (is_cpp())
    {
        language_ = language::CPP;
    }
    
    if (ap.arg_found("-c89"))
    {
        c_standard_ = "-std=c89";
    }
    else if (ap.arg_found("-c90"))
    {
        c_standard_ = "-std=c90";
    }
    else if (ap.arg_found("-c99"))
    {
        c_standard_ = "-std=c99";
    }
    else //if (ap.arg_found("-c11"))
    {
        c_standard_ = "-std=c11";
    }
    
    if (ap.arg_found("-c++98"))
    {
        cpp_standard_ = "-std=c++98";
    }
    else if (ap.arg_found("-c++03"))
    {
        cpp_standard_ = "-std=c++03";
    }
    else if (ap.arg_found("-c++11"))
    {
        cpp_standard_ = "-std=c++11";
    }
    else if (ap.arg_found("-c++14"))
    {
        cpp_standard_ = "-std=c++14";
    }
    else //if (ap.arg_found("-c++17"))
    {
        cpp_standard_ = "-std=c++17";
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


int runbuild_task::do_operation() const
{
    switch (language_)
    {
        case language::C:
            return build_ ? build_c(std::string(), true) : exec_c();
        
        case language::CPP:
            return build_ ? build_cpp(std::string(), true) : exec_cpp();
    }
    
    return -1;
}


int runbuild_task::build_c(std::string output_name, bool verbose) const
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


int runbuild_task::exec_c() const
{
    std::string output_name = std::tmpnam(nullptr);
    std::string command;
    int build_result = build_c(output_name, false);
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


int runbuild_task::build_cpp(std::string output_name, bool verbose) const
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


int runbuild_task::exec_cpp() const
{
    std::string output_name = std::tmpnam(nullptr);
    std::string command;
    int build_result = build_cpp(output_name, false);
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


std::unordered_set<std::string> runbuild_task::c_extensions_ =
        {".c"};


std::unordered_set<std::string> runbuild_task::cpp_extensions_ =
        {".cpp", ".cc", ".C", ".CPP", ".c++", ".cp", ".cxx"};
 

}
