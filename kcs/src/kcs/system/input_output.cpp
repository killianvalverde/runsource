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
 * @file        kcs/system/input_output.cpp
 * @brief       input and output fonctions source.
 * @author      Killian
 * @date        2017/05/23 - 15:30
 */

#include <stdio.h>
#ifdef __unix__
#include <termios.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

#include "input_output.hpp"


namespace kcs {
namespace system {


void pause(const char *message)
{
#if _POSIX_VERSION >= 200112L
    static struct termios oldt, newt;
    if (message != nullptr)
    {
        printf("%s\n", message);
    }
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#else
#error "system not supported"
#endif
}


}
}
