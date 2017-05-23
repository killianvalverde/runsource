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

#include <iostream>
#include <experimental/filesystem>

#include "kcs/argparse.hpp"
#include "kcs/system.hpp"
#include "runbuild_task.hpp"

namespace stdfs = std::experimental::filesystem;
namespace kap = kcs::argparse;
namespace ksys = kcs::system;


int main(int argc, char *argv[])
{
    kap::arg_parser ap("runsource");
    int result;
    
    ap.add_help_text("The folowind options are set by defautl: -e -c11 -c++17\nOptions:");
    ap.add_key_arg({"--exec", "-e"}, "Execute the specified source file.");
    ap.add_key_arg({"--build", "-b"}, "Build the specified source file.");
    ap.add_key_arg({"--clanguage", "-cl"}, "Force C language interpretation.");
    ap.add_key_arg({"--c++language", "-c++"}, "Force C++ language interpretation.");
    ap.add_key_arg({"-c89"}, "Use C89 standard when C language is selected.");
    ap.add_key_arg({"-c90"}, "Use C90 standard when C language is selected");
    ap.add_key_arg({"-c99"}, "Use C99 standard when C language is selected");
    ap.add_key_arg({"-c11"}, "Use C11 standard when C language is selected");
    ap.add_key_arg({"-c++98"}, "Use C++98 standard when C++ language is selected.");
    ap.add_key_arg({"-c++03"}, "Use C++03 standard when C++ language is selected.");
    ap.add_key_arg({"-c++11"}, "Use C++11 standard when C++ language is selected.");
    ap.add_key_arg({"-c++14"}, "Use C++14 standard when C++ language is selected.");
    ap.add_key_arg({"-c++17"}, "Use C++17 standard when C++ language is selected.");
    ap.add_key_value_arg({"--compiler-args", "-ca"},
                         "Forward the folowing arguments to the compiler.", {kap::avt_t::STRING},
                         1u, ~0u);
    ap.add_key_value_arg({"--program-args", "-pa"},
                         "Forward the folowing arguments to the produced program.",
                         {kap::avt_t::STRING}, 1u, ~0u);
    ap.add_help_arg({"--help"}, "Display this help and exit.");
    ap.add_gplv3_version_arg({"--version"}, "Output vesion information and exit", "1.0", "2017",
                             "Killian");
    ap.add_key_arg({"--pause", "-p"}, "Pause the program before exit.");
    ap.add_foreign_arg("SOURCE-FILE", "Source file", "The source file path.", {kap::avt_t::R_FILE},
                       1u, ~0u);
    
    ap.parse_args((unsigned int)argc, argv);
    
    runsource::runbuild_task rbtask(ap);
    result = rbtask.do_operation();
    
    if (ap.arg_found("--pause"))
    {
        ksys::pause("Press key to continue....");
    }
    
    return result;
}
