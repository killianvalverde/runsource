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
#include "kcs/iostream.hpp"
#include "kcs/system.hpp"
#include "runbuild_task.hpp"

namespace stdfs = std::experimental::filesystem;
namespace kap = kcs::argparse;
namespace ksys = kcs::system;
namespace kstr = kcs::stringutils;
namespace kios = kcs::iostream;


int main(int argc, char *argv[])
{
    kap::arg_parser ap(kap::arg_parser::constructor_params::get()
                               .program_name("runsource")
                               .flags(kap::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                      ~kap::apf_t::EXIT_ON_PRINT_ARGS_ERRORS));
    int result;
    
    ap.add_help_text("The folowind options are set by defautl: -gcc -e -c11 -c++17\nOptions:");
    ap.add_key_arg({"--gcc", "-gcc"}, "Use gcc tool chain for C and C++.");
    ap.add_key_arg({"--exec", "-e"}, "Execute the specified source file.");
    ap.add_key_arg({"--build", "-b"}, "Build the specified source file.");
    ap.add_key_arg({"--clanguage", "-cln"}, "Force C language interpretation.");
    ap.add_key_arg({"--c++language", "-c++"}, "Force C++ language interpretation.");
    ap.add_key_arg({"--bash", "-bsh"}, "Force bash language interpretation.");
    ap.add_key_arg({"--python", "-py"}, "Force python language interpretation.");
    ap.add_key_arg({"--c89"}, "Use C89 standard when C language is selected.");
    ap.add_key_arg({"--c90"}, "Use C90 standard when C language is selected");
    ap.add_key_arg({"--c99"}, "Use C99 standard when C language is selected");
    ap.add_key_arg({"--c11"}, "Use C11 standard when C language is selected");
    ap.add_key_arg({"--c++98"}, "Use C++98 standard when C++ language is selected.");
    ap.add_key_arg({"--c++03"}, "Use C++03 standard when C++ language is selected.");
    ap.add_key_arg({"--c++11"}, "Use C++11 standard when C++ language is selected.");
    ap.add_key_arg({"--c++14"}, "Use C++14 standard when C++ language is selected.");
    ap.add_key_arg({"--c++17"}, "Use C++17 standard when C++ language is selected.");
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
    ap.add_foreign_arg("FILE", "File", "The source file path.", {kap::avt_t::R_FILE},
                       1u, ~0u);
    
    kios::ios_redirect ior(std::cout);
    ior.redirect_to_embedded_stringstream();
    
    ap.parse_args((unsigned int)argc, argv);
    
    if (ap.there_are_errors())
    {
        if (ap.get_foreign_arg("FILE").there_are_errors())
        {
            std::vector<std::string> argv_vec = {argv[0]};
            std::string argv_builder;
            
            for (auto& x : ap.get_arg_values("FILE"))
            {
                auto current_values = kstr::strsplit(x.get_value(), ' ');
                
                for (auto& y : current_values)
                {
                    argv_builder += y;
                    
                    if (stdfs::exists(argv_builder))
                    {
                        argv_vec.push_back(std::move(argv_builder));
                    }
                    else
                    {
                        argv_builder += ' ';
                    }
                }
                
                if (!argv_builder.empty())
                {
                    goto exit;
                }
            }
    
            for (int i = 1; i < argc; i++)
            {
                if (argv[i][0] == '-')
                {
                    argv_vec.push_back(argv[i]);
                }
            }
            
            ap.parse_args(argv_vec.size(), argv_vec);
        }
    }
    exit: ;
    
    if (!ap.there_are_errors())
    {
        ior.unredirect();
        runsource::runbuild_task rbtask(ap);
        result = rbtask.do_operation();
    }
    else
    {
        std::string s = ior.get_embedded_stringstream_str();
        ior.unredirect();
        std::cout << s;
    }
    
    if (ap.arg_found("--pause"))
    {
        ksys::pause("Press key to continue....");
    }
    
    return result;
}
