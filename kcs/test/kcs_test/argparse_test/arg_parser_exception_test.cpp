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
 * @file        kcs_test/argparse_test/arg_parser_exception_test.cpp
 * @brief       arg_parser_exception_test unit test.
 * @author      Killian
 * @date        2017/05/16
 */

#include <cstring>

#include "gtest/gtest.h"
#include "kcs/argparse.hpp"

namespace kap = kcs::argparse;


TEST(arg_parser_exception_test, constructor_test)
{
    try
    {
        throw kap::arg_parser_exception("kcs::argparse::arg_parser_exception");
    }
    catch (std::exception& e)
    {
        EXPECT_TRUE(strcmp(e.what(), "kcs::argparse::arg_parser_exception") == 0);
        EXPECT_TRUE(strcmp(e.what(), "kcs::argparse::arg_parser_except!@#") != 0);
    }
}

