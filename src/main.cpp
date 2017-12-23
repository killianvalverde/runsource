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

#include <experimental/filesystem>

#include <kboost/kboost.hpp>

#include "program.hpp"

namespace stdfs = std::experimental::filesystem;
namespace rs = runsource;


int main(int argc, char *argv[])
{
    int res;
    
    kap::arg_parser ap("runsource");
    
    ap.add_help_text("The folowind options are set by defautl: --exec --monotonic-chrono --gcc "
                             "--c11 --c++17",
                     {"--help"});
    ap.add_help_text("Options:", {"--help"});
    ap.add_key_arg({"--exec", "-e"}, "Execute the specified source file.");
    ap.add_key_arg({"--build", "-b"}, "Build the specified source file.");
    ap.add_key_value_arg({"--compiler-args", "-ca"},
                         "Forward the folowing arguments to the compiler.",
                         {kap::avt_t::STRING});
    ap.add_key_value_arg({"--program-args", "-pa"},
                         "Forward the folowing arguments to the produced program.",
                         {kap::avt_t::STRING});
    ap.add_key_arg({"--pause", "-p"}, "Pause the program before exit.");
    ap.add_key_arg({"--monotonic-chrono", "-mc"}, "Use a monotonic chrono.");
    ap.add_key_arg({"--process-chrono", "-pc"}, "Use the process chrono.");
    ap.add_key_arg({"--gcc"}, "Use gcc tool chain for C and C++.");
    ap.add_key_arg({"--c"}, "Force C language interpretation.");
    ap.add_key_arg({"--c++"}, "Force C++ language interpretation.");
    ap.add_key_arg({"--bash"}, "Force bash language interpretation.");
    ap.add_key_arg({"--python"}, "Force python language interpretation.");
    ap.add_key_arg({"--c89"}, "Use C89 standard when C language is selected.");
    ap.add_key_arg({"--c90"}, "Use C90 standard when C language is selected");
    ap.add_key_arg({"--c99"}, "Use C99 standard when C language is selected");
    ap.add_key_arg({"--c11"}, "Use C11 standard when C language is selected");
    ap.add_key_arg({"--c++98"}, "Use C++98 standard when C++ language is selected.");
    ap.add_key_arg({"--c++03"}, "Use C++03 standard when C++ language is selected.");
    ap.add_key_arg({"--c++11"}, "Use C++11 standard when C++ language is selected.");
    ap.add_key_arg({"--c++14"}, "Use C++14 standard when C++ language is selected.");
    ap.add_key_arg({"--c++17"}, "Use C++17 standard when C++ language is selected.");
    ap.add_key_arg({"--optimize"}, "Use the maximum optimization level available.");
    ap.add_help_arg({"--help"}, "Display this help and exit.");
    ap.add_gplv3_version_arg({"--version"}, "Output version information and exit", "1.0", "2017",
                             "Killian");
    ap.add_foreign_arg("FILE", "File", "", {kap::avt_t::R_FILE}, 1u, ~0u);
    
    ap.parse_args((unsigned int)argc, argv);
    
    rs::language lang = rs::language::NIL;
    if (ap.arg_found("--c"))
    {
        lang = rs::language::C;
    }
    else if (ap.arg_found("--c++"))
    {
        lang = rs::language::CPP;
    }
    else if (ap.arg_found("--bash"))
    {
        lang = rs::language::BASH;
    }
    else if (ap.arg_found("--python"))
    {
        lang = rs::language::PYTHON;
    }

    rs::c_standard c_std = rs::c_standard::C11;
    if (ap.arg_found("--c89"))
    {
        c_std = rs::c_standard::C89;
    }
    else if (ap.arg_found("--c90"))
    {
        c_std = rs::c_standard::C90;
    }
    else if (ap.arg_found("--c99"))
    {
        c_std = rs::c_standard::C99;
    }
    else if (ap.arg_found("--c11"))
    {
        c_std = rs::c_standard::C11;
    }
    
    rs::cpp_standard cpp_std = rs::cpp_standard::CPP17;
    if (ap.arg_found("--c++98"))
    {
        cpp_std = rs::cpp_standard::CPP98;
    }
    else if (ap.arg_found("--c++03"))
    {
        cpp_std = rs::cpp_standard::CPP03;
    }
    else if (ap.arg_found("--c++11"))
    {
        cpp_std = rs::cpp_standard::CPP11;
    }
    else if (ap.arg_found("--c++14"))
    {
        cpp_std = rs::cpp_standard::CPP14;
    }
    else if (ap.arg_found("--c++17"))
    {
        cpp_std = rs::cpp_standard::CPP17;
    }
    
    rs::tool_chain tool_chn = rs::tool_chain::GCC;
    if (ap.arg_found("--gcc"))
    {
        tool_chn = rs::tool_chain::GCC;
    }

    rs::program prog(
            !ap.arg_found("--build"),
            lang,
            c_std,
            cpp_std,
            ap.arg_found("--optimize"),
            tool_chn,
            ap.get_front_arg_value_as<std::string>("--compiler-args", ""),
            ap.get_front_arg_value_as<std::string>("--program-args", ""),
            ap.arg_found("--monotonic-chrono"),
            ap.get_arg_values_as<stdfs::path>("FILE")
    );
    res = prog.execute();
    
    if (ap.arg_found("--pause"))
    {
        kios::fpurge(stdin);
        ksys::kbhit("Press key to continue...\n");
    }
    
    return res;
}
