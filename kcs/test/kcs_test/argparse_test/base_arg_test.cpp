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
 * @file        kcs_test/argparse_test/base_arg_test.cpp
 * @brief       base_arg unit test.
 * @author      Killian
 * @date        2017/05/16
 */

#include "gtest/gtest.h"
#include "kcs/argparse.hpp"
#include "kcs/iostream.hpp"

namespace kap = kcs::argparse;
namespace klow = kcs::lowlevel;
namespace kios = kcs::iostream;


TEST(base_arg_test, was_parsed_test)
{
    kap::base_arg base_arg("description", "error_id", kap::af_t::DEFAULT_ARG_FLAGS);
    EXPECT_TRUE(!base_arg.was_parsed());
}


TEST(base_arg_test, was_found_test)
{
    kap::base_arg base_arg("description", "error_id", kap::af_t::DEFAULT_ARG_FLAGS);
    EXPECT_TRUE(!base_arg.was_found());
}


TEST(base_arg_test, set_parsed_test)
{
    kap::base_arg base_arg("description", "error_id", kap::af_t::DEFAULT_ARG_FLAGS);
    base_arg.set_parsed(true);
    EXPECT_TRUE(base_arg.was_parsed());
}


TEST(base_arg_test, set_found_test)
{
    kap::base_arg base_arg("description", "error_id", kap::af_t::APPEAR_JUST_ONCE);
    base_arg.set_found(true);
    EXPECT_TRUE(base_arg.was_found());
    EXPECT_TRUE(!base_arg.there_are_errors());
    base_arg.set_found(true);
    EXPECT_TRUE(base_arg.was_found());
    EXPECT_TRUE(base_arg.there_are_errors());
    EXPECT_TRUE(base_arg.error_flag_is_raised(kap::aef_t::APPEAR_JUST_ONCE_ERROR));
    base_arg.set_found(false);
    EXPECT_TRUE(!base_arg.was_found());
    EXPECT_TRUE(!base_arg.there_are_errors());
}


TEST(base_arg_test, description_is_empty_test)
{
    kap::base_arg base_arg("", "error_id", kap::af_t::DEFAULT_ARG_FLAGS);
    EXPECT_TRUE(base_arg.description_is_empty());
    base_arg = kap::base_arg("description", "error_id", kap::af_t::DEFAULT_ARG_FLAGS);
    EXPECT_TRUE(!base_arg.description_is_empty());
}


TEST(base_arg_test, error_id_is_empty_test)
{
    kap::base_arg base_arg("description", "", kap::af_t::DEFAULT_ARG_FLAGS);
    EXPECT_TRUE(base_arg.error_id_is_empty());
    base_arg = kap::base_arg("description", "error_id", kap::af_t::DEFAULT_ARG_FLAGS);
    EXPECT_TRUE(!base_arg.error_id_is_empty());
}


TEST(base_arg_test, flag_is_raised_test)
{
    kap::base_arg base_arg("description", "error_id", kap::af_t::DEFAULT_ARG_FLAGS);
    EXPECT_TRUE(base_arg.flag_is_raised(kap::af_t::DEFAULT_ARG_FLAGS));
}


TEST(base_arg_test, erase_flag_test)
{
    kap::base_arg base_arg("description", "error_id", kap::af_t::DEFAULT_ARG_FLAGS);
    base_arg.erase_flag(kap::af_t::DEFAULT_ARG_FLAGS);
    EXPECT_TRUE(!base_arg.flag_is_raised(kap::af_t::DEFAULT_ARG_FLAGS));
}


TEST(base_arg_test, error_flag_is_raised_test)
{
    kap::base_arg base_arg("description", "error_id", kap::af_t::APPEAR_JUST_ONCE);
    base_arg.set_found(true);
    base_arg.set_found(true);
    EXPECT_TRUE(base_arg.error_flag_is_raised(kap::aef_t::APPEAR_JUST_ONCE_ERROR));
}


TEST(base_arg_test, raise_error_flag_test)
{
    kap::base_arg base_arg("description", "error_id", kap::af_t::APPEAR_JUST_ONCE);
    EXPECT_TRUE(!base_arg.there_are_errors());
    base_arg.raise_error_flag(kap::aef_t::APPEAR_JUST_ONCE_ERROR);
    EXPECT_TRUE(base_arg.error_flag_is_raised(kap::aef_t::APPEAR_JUST_ONCE_ERROR));
}


TEST(base_arg_test, erase_error_flag_test)
{
    kap::base_arg base_arg("description", "error_id", kap::af_t::APPEAR_JUST_ONCE);
    base_arg.raise_error_flag(kap::aef_t::APPEAR_JUST_ONCE_ERROR);
    EXPECT_TRUE(base_arg.error_flag_is_raised(kap::aef_t::APPEAR_JUST_ONCE_ERROR));
    base_arg.erase_error_flag(kap::aef_t::APPEAR_JUST_ONCE_ERROR);
    EXPECT_TRUE(!base_arg.error_flag_is_raised(kap::aef_t::APPEAR_JUST_ONCE_ERROR));
}


TEST(base_arg_test, clear_error_flags_test)
{
    kap::base_arg base_arg("description", "error_id", kap::af_t::APPEAR_JUST_ONCE);
    base_arg.raise_error_flag(kap::aef_t::APPEAR_JUST_ONCE_ERROR);
    EXPECT_TRUE(base_arg.error_flag_is_raised(kap::aef_t::APPEAR_JUST_ONCE_ERROR));
    base_arg.clear_error_flags();
    EXPECT_TRUE(!base_arg.error_flag_is_raised(kap::aef_t::APPEAR_JUST_ONCE_ERROR));
}


TEST(base_arg_test, update_error_flags_test)
{
    kap::base_arg base_arg("description", "error_id", kap::af_t::ALLWAYS_REQUIRED);
    base_arg.set_found(false);
    base_arg.set_parsed(true);
    base_arg.update_error_flags();
    EXPECT_TRUE(base_arg.there_are_errors());
}


TEST(base_arg_test, there_are_errors_test)
{
    kap::base_arg base_arg("description", "error_id", kap::af_t::APPEAR_JUST_ONCE);
    base_arg.raise_error_flag(kap::aef_t::APPEAR_JUST_ONCE_ERROR);
    EXPECT_TRUE(base_arg.there_are_errors());
}


TEST(base_arg_test, print_help_text_test)
{
    kios::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_embedded_stringstream();
    
    std::string description = "Usage: ls [OPTION]... [FILE]...\n"
            "List information about the FILEs (the current directory by default).\n"
            "Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.";
    
    std::string expected_result = "Usage: ls [OPTION]... [FILE]...\n"
            "List information about the FILEs (the current directory by default).\n"
            "Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.\n";
    
    kap::base_arg base_arg(description, "error_id", kap::af_t::DEFAULT_ARG_FLAGS);
    base_arg.print_help_text();
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
    
    ios_redirect.clear_embedded_stringstream();
    
    base_arg = kap::base_arg("", "error_id", kap::af_t::DEFAULT_ARG_FLAGS);
    base_arg.print_help_text();
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str().empty());
}


TEST(base_arg_test, print_description_test)
{
    kios::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_embedded_stringstream();
    
    std::string description = "Usage: ls [OPTION]... [FILE]...\n"
            "List information about the FILEs (the current directory by default).\n"
            "Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.";
    
    std::string expected_result = "Usage: ls [OPTION]... [FILE]...\n"
            "      List information about the FILEs (the current directory by default).\n"
            "      Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.\n";
    
    kap::base_arg base_arg(description, "error_id", kap::af_t::DEFAULT_ARG_FLAGS);
    base_arg.print_description(2, 4);
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
    
    ios_redirect.clear_embedded_stringstream();
    
    base_arg = kap::base_arg("", "error_id", kap::af_t::DEFAULT_ARG_FLAGS);
    base_arg.print_description(2, 4);
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str().empty());
}


TEST(base_arg_test, print_errors_test)
{
    kios::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_embedded_stringstream();
    
    std::string expected_result = "kcs: error: Option is allways required\n"
            "kcs: error: Option has appeared more than once\n";
    
    kap::base_arg base_arg("description", "error", kap::af_t::DEFAULT_ARG_FLAGS);
    base_arg.raise_error_flag(kap::aef_t::ALLWAYS_REQUIRED_ERROR |
                                      kap::aef_t::APPEAR_JUST_ONCE_ERROR);
    base_arg.print_errors("kcs");
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
    
    ios_redirect.clear_embedded_stringstream();
    
    base_arg.clear_error_flags();
    base_arg.print_errors("kcs");
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str().empty());
}


TEST(base_arg_test, print_error_message_test)
{
    kios::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_embedded_stringstream();
    
    std::string expected_result = "kcs: error: Warning\n";
    
    kap::base_arg base_arg("description", "error", kap::af_t::DEFAULT_ARG_FLAGS);
    base_arg.print_error_message("Warning", "kcs");
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
}


TEST(base_arg_test, equal_operator_test)
{
    kap::base_arg base_arg_1("description", "error", kap::af_t::DEFAULT_ARG_FLAGS);
    kap::base_arg base_arg_2("description", "error", kap::af_t::DEFAULT_ARG_FLAGS);
    EXPECT_TRUE(base_arg_1 == base_arg_2);
}


TEST(base_arg_test, not_equal_operator_test)
{
    kap::base_arg base_arg_1("description", "error", kap::af_t::DEFAULT_ARG_FLAGS);
    kap::base_arg base_arg_2("descript!@#", "error", kap::af_t::DEFAULT_ARG_FLAGS);
    EXPECT_TRUE(base_arg_1 != base_arg_2);
}
