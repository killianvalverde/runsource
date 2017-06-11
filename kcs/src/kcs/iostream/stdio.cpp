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
 * @file        kcs/iostream/stdio.cpp
 * @brief       stdio functions source.
 * @author      Killian
 * @date        2016/08/24 - 09:50
 */

#include <cstdarg>
#include <cstdio>
#include <cwchar>


namespace kcs {
namespace iostream {


int printf(const char* format, ...) noexcept
{
    int done;
    va_list args;
    va_start(args, format);
    done = vfprintf(stdout, format, args);
    va_end(args);
    return done;
}


int printf(const wchar_t* format, ...) noexcept
{
    int done;
    va_list args;
    va_start(args, format);
    done = vfwprintf(stdout, format, args);
    va_end(args);
    return done;
}

}
}
