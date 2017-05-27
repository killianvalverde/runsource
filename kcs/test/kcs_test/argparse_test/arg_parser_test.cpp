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
 * @file        kcs_test/argparse_test/arg_parser_test.cpp
 * @brief       arg_parser unit test.
 * @author      Killian
 * @date        2017/05/16
 */

#include "gtest/gtest.h"
#include "kcs/argparse.hpp"
#include "kcs/iostream.hpp"

namespace stdfs = std::experimental::filesystem;
namespace kap = kcs::argparse;
namespace klow = kcs::lowlevel;
namespace kios = kcs::iostream;


TEST(arg_parser_test, perfect_forwarding_constructor_test)
{
    kap::arg_parser ap1();
    kap::arg_parser ap2("kcs");
    kap::arg_parser ap3("kcs", {"-"}, {"--"}, 2u, 80u, 2u, ~0u,
                                 kap::apf_t::DEFAULT_ARG_PARSER_FLAGS);;
    
    std::string program_name = "kcs";
    std::unordered_set<std::string> short_prefixes = {"-"};
    std::unordered_set<std::string> long_prefixes = {"--"};
    std::size_t key_help_description_indentation = 2u;
    std::size_t max_description_line_length = 80u;
    std::size_t newline_indentation = 2u;
    std::size_t max_unrecognized_args = ~2u;
    kap::apf_t flags = kap::apf_t::DEFAULT_ARG_PARSER_FLAGS;
    
    kap::arg_parser ap4(program_name,
                        short_prefixes,
                        long_prefixes,
                        key_help_description_indentation,
                        max_description_line_length,
                        newline_indentation,
                        max_unrecognized_args,
                        flags);
    
    EXPECT_TRUE(!program_name.empty());
    EXPECT_TRUE(!short_prefixes.empty());
    EXPECT_TRUE(!long_prefixes.empty());
    
    kap::arg_parser ap5(std::move(program_name),
                        std::move(short_prefixes),
                        std::move(long_prefixes),
                        std::move(key_help_description_indentation),
                        std::move(max_description_line_length),
                        std::move(newline_indentation),
                        std::move(max_unrecognized_args),
                        std::move(flags));
    
    EXPECT_TRUE(program_name.empty());
    EXPECT_TRUE(short_prefixes.empty());
    EXPECT_TRUE(long_prefixes.empty());
}


TEST(arg_parser_test, parameters_constructor_test)
{
    kap::arg_parser ap(kap::arg_parser::constructor_params::get()
                                       .flags(kap::apf_t::DEFAULT_ARG_PARSER_FLAGS)
                                       .max_unrecognized_args(~0u)
                                       .newline_indentation(2)
                                       .max_description_line_length(80)
                                       .key_help_description_indentation(2)
                                       .long_prefixes({"--"})
                                       .short_prefixes({"-"})
                                       .program_name("kcs"));
}


TEST(arg_parser_test, copy_constructor_test)
{
    kap::arg_parser ap1("kcs", {"-"}, {"--"}, 2u, 80u, 2u, ~0u,
                        kap::apf_t::DEFAULT_ARG_PARSER_FLAGS);
    kap::arg_parser ap2(ap1);
}


TEST(arg_parser_test, move_constructor_test)
{
    kap::arg_parser ap1("kcs", {"-"}, {"--"}, 4u, 100u, 4u, ~2u,
                        kap::apf_t::DEFAULT_ARG_PARSER_FLAGS);
    kap::arg_parser ap2(std::move(ap1));
}


TEST(arg_parser_test, copy_assignment_operator_test)
{
    kap::arg_parser ap1;
    kap::arg_parser ap2("kcs", {"-"}, {"/"}, 2u, 80u, 2u, ~0u,
                        kap::apf_t::DEFAULT_ARG_PARSER_FLAGS);
    ap1 = ap2;
}


TEST(arg_parser_test, move_assignment_operator_test)
{
    kap::arg_parser ap1("kcs", {"-"}, {"--"}, 2u, 80u, 2u, ~0u,
                        kap::apf_t::DEFAULT_ARG_PARSER_FLAGS);
    kap::arg_parser ap2("kcs_", {"/"}, {"//"}, 4u, 100u, 4u, 1u, kap::apf_t::NULL_ARG_PARSER_FLAGS);
    ap1 = std::move(ap2);
}


TEST(arg_parser_test, add_help_text_test)
{
    kios::ios_redirect ios_redirect(std::cout);
    ios_redirect.redirect_to_embedded_stringstream();

    std::string help_text_1 = "world";
    std::string help_text_2 = "bye";
    std::string expected_result("hello\nworld\nbye\n");

    kap::arg_parser ap1(kap::arg_parser::constructor_params::get()
                                       .flags(kap::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                              ~kap::apf_t::EXIT_ON_PRINT_HELP &
                                              ~kap::apf_t::PRINT_USAGE_ON_PRINT_HELP));
    
    ap1.add_help_text("hello");
    ap1.add_help_text(help_text_1);
    ap1.add_help_text(std::move(help_text_2));
    ap1.print_help();
    EXPECT_TRUE(!help_text_1.empty());
    EXPECT_TRUE(help_text_2.empty());
    EXPECT_TRUE(ios_redirect.get_embedded_stringstream_str() == expected_result);
}


TEST(arg_parser_test, add_key_arg_test)
{
    std::vector<std::string> keys = {"-r", "--recursive"};
    std::string description = "Recursive behavior";
    std::string error_id = "error";

    kap::arg_parser ap1;
    ap1.add_key_arg(keys, description, kap::af_t::DEFAULT_ARG_FLAGS, error_id);
    EXPECT_TRUE(!keys[0].empty() && !keys[1].empty());
    EXPECT_TRUE(!description.empty());
    EXPECT_TRUE(!error_id.empty());
    EXPECT_THROW(ap1.add_key_arg({}, "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap1.add_key_arg({"-r"}, "..."), kap::arg_parser_exception);
    
    kap::arg_parser ap2;
    ap2.add_key_arg(std::move(keys), std::move(description),
                    kap::af_t::DEFAULT_ARG_FLAGS, std::move(error_id));
    EXPECT_TRUE(keys[0].empty() && keys[1].empty());
    EXPECT_TRUE(description.empty());
    EXPECT_TRUE(error_id.empty());
    EXPECT_THROW(ap2.add_key_arg({}, "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap2.add_key_arg({"-r"}, "..."), kap::arg_parser_exception);
}


TEST(arg_parser_test, add_help_arg_test)
{
    std::vector<std::string> keys = {"-h", "--help"};
    std::string description = "Display this help and exit";
    std::string error_id = "error";
    
    kap::arg_parser ap1;
    ap1.add_help_arg(keys, description, kap::af_t::DEFAULT_ARG_FLAGS, error_id);
    EXPECT_TRUE(!keys[0].empty() && !keys[1].empty());
    EXPECT_TRUE(!description.empty());
    EXPECT_TRUE(!error_id.empty());
    EXPECT_THROW(ap1.add_key_arg({}, "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap1.add_key_arg({"-h", "--help"}, "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap1.add_help_arg({"-?", "--???"}, "..."), kap::arg_parser_exception);
    
    kap::arg_parser ap2;
    ap2.add_help_arg(std::move(keys), std::move(description), kap::af_t::DEFAULT_ARG_FLAGS,
                     std::move(error_id));
    EXPECT_TRUE(keys[0].empty() && keys[1].empty());
    EXPECT_TRUE(description.empty());
    EXPECT_TRUE(error_id.empty());
    EXPECT_THROW(ap2.add_key_arg({}, "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap2.add_key_arg({"-h", "--help"}, "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap2.add_help_arg({"-?", "--???"}, "..."), kap::arg_parser_exception);
}


TEST(arg_parser_test, add_version_arg_test)
{
    std::vector<std::string> keys = {"-v", "--version"};
    std::string description = "Display version information";
    std::string version_information = "1.0.0";
    std::string error_id = "error";
    
    kap::arg_parser ap1;
    ap1.add_version_arg(keys, description, version_information, kap::af_t::DEFAULT_ARG_FLAGS,
                       error_id);
    EXPECT_TRUE(!keys[0].empty() && !keys[1].empty());
    EXPECT_TRUE(!description.empty());
    EXPECT_TRUE(!version_information.empty());
    EXPECT_TRUE(!error_id.empty());
    EXPECT_THROW(ap1.add_version_arg({}, "...", "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap1.add_version_arg({"-v", "--version"}, "...", "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap1.add_version_arg({"-?", "--???"}, "...", "..."), kap::arg_parser_exception);
    
    kap::arg_parser ap2;
    ap2.add_version_arg(std::move(keys), std::move(description), std::move(version_information),
                        kap::af_t::DEFAULT_ARG_FLAGS, std::move(error_id));
    EXPECT_TRUE(keys[0].empty() && keys[1].empty());
    EXPECT_TRUE(description.empty());
    EXPECT_TRUE(version_information.empty());
    EXPECT_TRUE(error_id.empty());
    EXPECT_THROW(ap2.add_version_arg({}, "...", "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap2.add_version_arg({"-v", "--version"}, "...", "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap2.add_version_arg({"-?", "--???"}, "...", "..."), kap::arg_parser_exception);
}


TEST(arg_parser_test, add_gplv3_version_arg_test)
{
    std::vector<std::string> keys = {"-v", "--version"};
    std::string description = "Display version information";
    std::string program_version = "1.0.0";
    std::string date = "2017";
    std::string author = "Killian";
    std::string error_id = "error";
    
    kap::arg_parser ap1;
    ap1.add_gplv3_version_arg(keys, description, program_version, date, author,
                             kap::af_t::DEFAULT_ARG_FLAGS, error_id);
    EXPECT_TRUE(!keys[0].empty() && !keys[1].empty());
    EXPECT_TRUE(!description.empty());
    EXPECT_TRUE(!program_version.empty());
    EXPECT_TRUE(!date.empty());
    EXPECT_TRUE(!author.empty());
    EXPECT_TRUE(!error_id.empty());
    EXPECT_THROW(ap1.add_gplv3_version_arg({}, "...", "...", "...", "..."),
                 kap::arg_parser_exception);
    EXPECT_THROW(ap1.add_gplv3_version_arg({"-v", "--version"}, "...", "...", "...", "..."),
                 kap::arg_parser_exception);
    EXPECT_THROW(ap1.add_gplv3_version_arg({"-?", "--???"}, "...", "...", "...", "..."),
                 kap::arg_parser_exception);
    
    kap::arg_parser ap2;
    ap2.add_gplv3_version_arg(std::move(keys), std::move(description), std::move(program_version),
                              std::move(date), std::move(author), kap::af_t::DEFAULT_ARG_FLAGS,
                              std::move(error_id));
    EXPECT_TRUE(keys[0].empty() && keys[1].empty());
    EXPECT_TRUE(description.empty());
    //EXPECT_TRUE(program_version.empty());
    //EXPECT_TRUE(date.empty());
    //EXPECT_TRUE(author.empty());
    EXPECT_TRUE(error_id.empty());
    EXPECT_THROW(ap2.add_gplv3_version_arg({}, "...", "...", "...", "..."),
                 kap::arg_parser_exception);
    EXPECT_THROW(ap2.add_gplv3_version_arg({"-v", "--version"}, "...", "...", "...", "..."),
                 kap::arg_parser_exception);
    EXPECT_THROW(ap2.add_gplv3_version_arg({"-?", "--???"}, "...", "...", "...", "..."),
                 kap::arg_parser_exception);
}


TEST(arg_parser_test, add_key_value_arg_1_test)
{
    std::vector<std::string> keys = {"-s", "--seconds"};
    std::string description = "The number of seconds";
    std::vector<kap::avt_t> values_types = {kap::avt_t::INT64};
    std::vector<std::string> regex_collection = {".*"};
    std::string error_id = "error";
    
    kap::arg_parser ap1;
    ap1.add_key_value_arg(keys, description, values_types, 1, 1, kap::af_t::DEFAULT_ARG_FLAGS,
                          regex_collection, error_id);
    EXPECT_TRUE(!keys[0].empty() && !keys[1].empty());
    EXPECT_TRUE(!description.empty());
    EXPECT_TRUE(!values_types.empty());
    EXPECT_TRUE(!regex_collection.empty());
    EXPECT_TRUE(!error_id.empty());
    EXPECT_THROW(ap1.add_key_value_arg({}, "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap1.add_key_value_arg({"-s"}, "..."), kap::arg_parser_exception);
    
    kap::arg_parser ap2;
    ap2.add_key_value_arg(std::move(keys), std::move(description), std::move(values_types), 1, 1,
                          kap::af_t::DEFAULT_ARG_FLAGS, std::move(regex_collection),
                          std::move(error_id));
    EXPECT_TRUE(keys[0].empty() && keys[1].empty());
    EXPECT_TRUE(description.empty());
    EXPECT_TRUE(values_types.empty());
    EXPECT_TRUE(regex_collection.empty());
    EXPECT_TRUE(error_id.empty());
    EXPECT_THROW(ap2.add_key_value_arg({}, "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap2.add_key_value_arg({"-s"}, "..."), kap::arg_parser_exception);
}


TEST(arg_parser_test, add_key_value_arg_2_test)
{
    kap::arg_parser ap;
    ap.add_key_value_arg(kap::arg_parser::add_key_value_arg_params::get()
                                 .description("Include all")
                                 .keys({"-a", "--all"})
                                 .regex_collection({".*"})
                                 .flags(kap::af_t::ALLOW_CHAIN));
    EXPECT_THROW(ap.add_key_value_arg({}, "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap.add_key_value_arg({"-a", "--all"}, "..."), kap::arg_parser_exception);
}


TEST(arg_parser_test, add_foreign_arg_1_test)
{
    std::string usage_key = "FILE";
    std::string help_key = "File";
    std::string description = "The file path";
    std::vector<kap::avt_t> values_types = {kap::avt_t::STRING};
    std::vector<std::string> regex_collection = {".*"};
    std::string error_id = "error";
    
    kap::arg_parser ap1;
    ap1.add_foreign_arg(usage_key, help_key, description, values_types, 1, 1,
                       kap::af_t::DEFAULT_ARG_FLAGS, regex_collection, error_id);
    EXPECT_TRUE(!usage_key.empty());
    EXPECT_TRUE(!help_key.empty());
    EXPECT_TRUE(!description.empty());
    EXPECT_TRUE(!values_types.empty());
    EXPECT_TRUE(!regex_collection.empty());
    EXPECT_TRUE(!error_id.empty());
    EXPECT_THROW(ap1.add_foreign_arg("", "", "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap1.add_foreign_arg("FILE", "...", "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap1.add_foreign_arg("...", "File", "..."), kap::arg_parser_exception);
    
    kap::arg_parser ap2;
    ap2.add_foreign_arg(std::move(usage_key), std::move(help_key), std::move(description),
                        std::move(values_types), 1, 1, kap::af_t::DEFAULT_ARG_FLAGS,
                        std::move(regex_collection), std::move(error_id));
    EXPECT_TRUE(usage_key.empty());
    EXPECT_TRUE(help_key.empty());
    EXPECT_TRUE(description.empty());
    EXPECT_TRUE(values_types.empty());
    EXPECT_TRUE(regex_collection.empty());
    EXPECT_TRUE(error_id.empty());
    EXPECT_THROW(ap2.add_foreign_arg("", "", "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap2.add_foreign_arg("FILE", "...", "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap2.add_foreign_arg("...", "File", "..."), kap::arg_parser_exception);
}


TEST(arg_parser_test, add_foreign_arg_2_test)
{
    kap::arg_parser ap;
    ap.add_foreign_arg(kap::arg_parser::add_foreign_arg_params::get()
                               .help_key("File")
                               .usage_key("FILE")
                               .regex_collection({".*"})
                               .description("The file path"));
    EXPECT_THROW(ap.add_foreign_arg("", "", "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap.add_foreign_arg("FILE", "...", "..."), kap::arg_parser_exception);
    EXPECT_THROW(ap.add_foreign_arg("...", "File", "..."), kap::arg_parser_exception);
}


TEST(arg_parser_test, get_base_arg_test)
{
    kap::arg_parser ap;
    ap.add_key_arg({"-a"}, "All");
    EXPECT_NO_THROW(ap.get_base_arg("-a"));
    EXPECT_THROW(ap.get_base_arg("-l"), kap::arg_parser_exception);
}


TEST(arg_parser_test, get_key_arg_test)
{
    kap::arg_parser ap;
    ap.add_key_arg({"-a"}, "All");
    EXPECT_NO_THROW(ap.get_base_arg("-a"));
    EXPECT_NO_THROW(ap.get_key_arg("-a"));
}


TEST(arg_parser_test, get_value_arg_test)
{
    kap::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds");
    EXPECT_NO_THROW(ap.get_base_arg("-s"));
    EXPECT_NO_THROW(ap.get_value_arg("-s"));
}


TEST(arg_parser_test, get_key_value_arg_test)
{
    kap::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds");
    EXPECT_NO_THROW(ap.get_base_arg("-s"));
    EXPECT_NO_THROW(ap.get_key_arg("-s"));
    EXPECT_NO_THROW(ap.get_value_arg("-s"));
    EXPECT_NO_THROW(ap.get_key_value_arg("-s"));
}


TEST(arg_parser_test, get_foreign_arg_test)
{
    kap::arg_parser ap;
    ap.add_foreign_arg("FILE", "File", "The file path");
    EXPECT_NO_THROW(ap.get_base_arg("FILE"));
    EXPECT_NO_THROW(ap.get_base_arg("File"));
    EXPECT_NO_THROW(ap.get_value_arg("FILE"));
    EXPECT_NO_THROW(ap.get_value_arg("File"));
    EXPECT_NO_THROW(ap.get_foreign_arg("FILE"));
    EXPECT_NO_THROW(ap.get_foreign_arg("File"));
}


TEST(arg_parser_test, parse_args_test_1)
{
    std::vector<const char*> argv = {};

    kap::arg_parser ap;
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {kap::avt_t::UINT64});
    ap.add_key_value_arg({"--mintes", "-m"}, "Set minutes", {kap::avt_t::UINT64});
    ap.add_key_value_arg({"--hours", "-h"}, "Set hours", {kap::avt_t::UINT64});
    EXPECT_NO_THROW(ap.parse_args(argv.size(), argv));
}


TEST(arg_parser_test, parse_args_test_2)
{
    std::vector<const char*> argv = {
        "kcs",
        "-s", "10",
        "-m", "20",
        "-h", "30"
    };

    kap::arg_parser ap;
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {kap::avt_t::UINT64});
    ap.add_key_value_arg({"--mintes", "-m"}, "Set minutes", {kap::avt_t::UINT64});
    ap.add_key_value_arg({"--hours", "-h"}, "Set hours", {kap::avt_t::UINT64});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.get_front_arg_value("-s").get_value() == "10");
    EXPECT_TRUE(ap.get_front_arg_value("-m").get_value() == "20");
    EXPECT_TRUE(ap.get_front_arg_value("-h").get_value() == "30");
}


TEST(arg_parser_test, parse_args_test_3)
{
    std::vector<const char*> argv = {
            "kcs",
            "-s", "10",
            "-m", "20", "200",
            "-h", "30", "300", "3000"
    };

    kap::arg_parser ap;
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {kap::avt_t::UINT64});
    ap.add_key_value_arg({"--mintes", "-m"}, "Set minutes", {kap::avt_t::UINT64}, 1, 2);
    ap.add_key_value_arg({"--hours", "-h"}, "Set hours", {kap::avt_t::UINT64}, 1, 3);
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.get_front_arg_value("-s").get_value() == "10");
    EXPECT_TRUE(ap.get_front_arg_value("-m").get_value() == "20");
    EXPECT_TRUE(ap.get_front_arg_value("-h").get_value() == "30");
    EXPECT_TRUE(ap.get_arg_value_at("-m", 1).get_value() == "200");
    EXPECT_TRUE(ap.get_arg_value_at("-h", 1).get_value() == "300");
    EXPECT_TRUE(ap.get_arg_value_at("-h", 2).get_value() == "3000");
}


TEST(arg_parser_test, parse_args_test_4)
{
    std::vector<const char*> argv = {"kcs", "-smh", "10", "20", "30"};

    kap::arg_parser ap;
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {kap::avt_t::UINT64});
    ap.add_key_value_arg({"--mintes", "-m"}, "Set minutes", {kap::avt_t::UINT64});
    ap.add_key_value_arg({"--hours", "-h"}, "Set hours", {kap::avt_t::UINT64});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.get_front_arg_value("-s").get_value() == "10");
    EXPECT_TRUE(ap.get_front_arg_value("-m").get_value() == "20");
    EXPECT_TRUE(ap.get_front_arg_value("-h").get_value() == "30");
}


TEST(arg_parser_test, parse_args_test_5)
{
    std::vector<const char*> argv = {"kcs", "-sm", "10", "-h", "30"};

    kap::arg_parser ap(kap::arg_parser::constructor_params::get()
                               .flags(kap::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                              ~kap::apf_t::PRINT_ERRORS));
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {kap::avt_t::UINT64});
    ap.add_key_value_arg({"--minutes", "-m"}, "Set minutes", {kap::avt_t::UINT64});
    ap.add_key_value_arg({"--hours", "-h"}, "Set hours", {kap::avt_t::UINT64});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.arg_found("-m"));
    EXPECT_TRUE(ap.there_are_errors());
    EXPECT_TRUE(ap.get_front_arg_value("-h").get_value() == "30");
}


TEST(arg_parser_test, parse_args_test_6)
{
    std::vector<const char*> argv = {
            "kcs",
            "-s=10",
            "-m=20",
            "-h=30"
    };
    
    kap::arg_parser ap;
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {kap::avt_t::UINT64});
    ap.add_key_value_arg({"--mintes", "-m"}, "Set minutes", {kap::avt_t::UINT64});
    ap.add_key_value_arg({"--hours", "-h"}, "Set hours", {kap::avt_t::UINT64});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.get_front_arg_value("-s").get_value() == "10");
    EXPECT_TRUE(ap.get_front_arg_value("-m").get_value() == "20");
    EXPECT_TRUE(ap.get_front_arg_value("-h").get_value() == "30");
}


TEST(arg_parser_test, parse_args_test_7)
{
    std::vector<const char*> argv = {
            "kcs",
            "/home/user/Desktop/file.cpp",
            "-s", "10"
    };
    
    kap::arg_parser ap;
    ap.add_key_value_arg({"--seconds", "-s"}, "Set seconds", {kap::avt_t::UINT64});
    ap.add_foreign_arg("FILE", "File", "The file path");
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.get_front_arg_value("File").get_value() == "/home/user/Desktop/file.cpp");
}


TEST(arg_parser_test, parse_args_test_8)
{
    std::vector<const char*> argv = {
            "kcs",
            "/home/user/Desktop/file.cpp",
            "-s", "10"
    };
    
    kap::arg_parser ap(kap::arg_parser::constructor_params::get()
                               .flags(kap::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                      ~kap::apf_t::PRINT_ERRORS));
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.error_flag_is_raised(kap::apef_t::UNRECOGNIZED_ARGS_ERROR));
}


TEST(arg_parser_test, reset_args_parse_test)
{
    std::vector<const char*> argv = {
            "kcs",
            "/home/user/Desktop/file.cpp",
            "-s", "10"
    };
    
    kap::arg_parser ap(kap::arg_parser::constructor_params::get()
                               .flags(kap::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                      ~kap::apf_t::PRINT_ERRORS));
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.error_flag_is_raised(kap::apef_t::UNRECOGNIZED_ARGS_ERROR));
    ap.reset_args_parse();
    EXPECT_TRUE(!ap.error_flag_is_raised(kap::apef_t::UNRECOGNIZED_ARGS_ERROR));
}


TEST(arg_parser_test, arg_found_test)
{
    std::vector<const char*> argv = {"kcs", "-a"};

    kap::arg_parser ap;
    ap.add_key_arg({"-a", "--all"}, "Print all");
    ap.add_key_arg({"-l", "--long"}, "Long format");
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.arg_found("-a"));
    EXPECT_TRUE(!ap.arg_found("-l"));
}


TEST(arg_parser_test, help_arg_found_test)
{
    std::vector<const char*> argv1 = {"kcs", "--help"};
    std::vector<const char*> argv2 = {"kcs", "--all"};

    kap::arg_parser ap(kap::arg_parser::constructor_params::get()
                               .flags(kap::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                              ~kap::apf_t::PRINT_HELP &
                                              ~kap::apf_t::PRINT_VERSION));
    ap.add_help_arg({"--help"}, "Display this help and exit");
    ap.add_key_arg({"--all"}, "Display version information");
    ap.parse_args(argv1.size(), argv1);
    EXPECT_TRUE(ap.help_arg_found());
    ap.parse_args(argv2.size(), argv2);
    EXPECT_TRUE(!ap.help_arg_found());
}


TEST(arg_parser_test, version_arg_found_test)
{
    std::vector<const char*> argv1 = {"kcs", "--all"};
    std::vector<const char*> argv2 = {"kcs", "--version"};

    kap::arg_parser ap(kap::arg_parser::constructor_params::get()
                               .flags(kap::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                      ~kap::apf_t::PRINT_HELP &
                                      ~kap::apf_t::PRINT_VERSION));
    ap.add_key_arg({"--all"}, "Display this help and exit");
    ap.add_version_arg({"--version"}, "Display version information", "1.0.0");
    ap.parse_args(argv1.size(), argv1);
    EXPECT_TRUE(!ap.version_arg_found());
    ap.parse_args(argv2.size(), argv2);
    EXPECT_TRUE(ap.version_arg_found());
}


TEST(arg_parser_test, get_front_arg_value_1_test)
{
    std::vector<const char*> argv = {"kcs", "-s", "30"};

    kap::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {kap::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {kap::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_front_arg_value("-h"), kap::arg_parser_exception);
    EXPECT_THROW(ap.get_front_arg_value("-m"), std::out_of_range);
    EXPECT_TRUE(!ap.get_front_arg_value("-s").there_are_errors());
}


TEST(arg_parser_test, get_front_arg_value_2_test)
{
    std::vector<const char*> argv = {"kcs", "-s", "30"};

    kap::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {kap::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {kap::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.get_front_arg_value("-h", std::nothrow).there_are_errors());
    EXPECT_TRUE(ap.get_front_arg_value("-m", std::nothrow).there_are_errors());
    EXPECT_TRUE(!ap.get_front_arg_value("-s", std::nothrow).there_are_errors());
}


TEST(arg_parser_test, get_arg_value_at_1_test)
{
    std::vector<const char*> argv = {"kcs", "-s", "30"};

    kap::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {kap::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {kap::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_arg_value_at("-h", 0), kap::arg_parser_exception);
    EXPECT_THROW(ap.get_arg_value_at("-m", 1), std::out_of_range);
    EXPECT_TRUE(!ap.get_arg_value_at("-s", 0).there_are_errors());
}


TEST(arg_parser_test, get_arg_value_at_2_test)
{
    std::vector<const char*> argv = {"kcs", "-s", "30"};

    kap::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {kap::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {kap::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.get_arg_value_at("-h", 0, std::nothrow).there_are_errors());
    EXPECT_TRUE(ap.get_arg_value_at("-m", 1, std::nothrow).there_are_errors());
    EXPECT_TRUE(!ap.get_arg_value_at("-s", 0, std::nothrow).there_are_errors());
}


TEST(arg_parser_test, get_arg_values_1_test)
{
    std::vector<const char*> argv = {"kcs", "-s", "30"};

    kap::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {kap::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {kap::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_THROW(ap.get_arg_values("-h"), kap::arg_parser_exception);
    EXPECT_TRUE(ap.get_arg_values("-s").size() == 1);
    EXPECT_TRUE(ap.get_arg_values("-s").front().get_value() == "30");
}


TEST(arg_parser_test, get_arg_values_2_test)
{
    std::vector<const char*> argv = {"kcs", "-s", "30"};

    kap::arg_parser ap;
    ap.add_key_value_arg({"-s"}, "Seconds to set", {kap::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {kap::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.get_arg_values("-h", std::nothrow).empty());
    EXPECT_TRUE(ap.get_arg_values("-s", std::nothrow).size() == 1);
    EXPECT_TRUE(ap.get_arg_values("-s", std::nothrow).front().get_value() == "30");
}


TEST(arg_parser_test, flag_is_raised_test)
{
    kap::arg_parser ap(kap::arg_parser::constructor_params::get().flags(kap::apf_t::PRINT_HELP));
    EXPECT_TRUE(ap.flag_is_raised(kap::apf_t::PRINT_HELP));
    EXPECT_TRUE(!ap.flag_is_raised(kap::apf_t::PRINT_VERSION));
}


TEST(arg_parser_test, error_flag_is_raised_test)
{
    std::vector<const char*> argv = {"kcs", "-h", "30"};

    kap::arg_parser ap(kap::arg_parser::constructor_params::get()
                               .flags(kap::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                              ~kap::apf_t::PRINT_ERRORS));
    ap.add_key_value_arg({"-s"}, "Seconds to set", {kap::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {kap::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.error_flag_is_raised(kap::apef_t::UNRECOGNIZED_ARGS_ERROR));
}


TEST(arg_parser_test, there_are_errors_test)
{
    std::vector<const char*> argv = {"kcs", "-h", "30"};

    kap::arg_parser ap(kap::arg_parser::constructor_params::get()
                               .flags(kap::apf_t::DEFAULT_ARG_PARSER_FLAGS &
                                      ~kap::apf_t::PRINT_ERRORS));
    ap.add_key_value_arg({"-s"}, "Seconds to set", {kap::avt_t::INT32});
    ap.add_key_value_arg({"-m"}, "Minutes to set", {kap::avt_t::INT32});
    ap.parse_args(argv.size(), argv);
    EXPECT_TRUE(ap.there_are_errors());
}
