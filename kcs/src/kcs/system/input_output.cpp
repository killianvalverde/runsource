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

#ifdef __unix__
#include <termios.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif
#include <cstdio>

#include "input_output.hpp"


namespace kcs {
namespace system {


void pause(const char *message)
{
#if _POSIX_VERSION >= 200112L
    struct ::termios oldt, newt;
    if (message != nullptr)
    {
        ::printf("%s\n", message);
    }
    ::tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    ::tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ::getchar();
    ::tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#else
#error "system not supported"
#endif
}


int set_stream_text_attribute(::FILE *stream, text_attribute attribute)
{
#ifdef __unix__
    int result = 0;
    
    switch (attribute)
    {
        case text_attribute::DEFAULT:
            result = ::fprintf(stream, "\033[0m");
            break;
        case text_attribute::BLACK:
            result = ::fprintf(stream, "\033[0;30m");
            break;
        case text_attribute::RED:
            result = ::fprintf(stream, "\033[0;31m");
            break;
        case text_attribute::GREEN:
            result = ::fprintf(stream, "\033[0;32m");
            break;
        case text_attribute::BROWN:
            result = ::fprintf(stream, "\033[0;33m");
            break;
        case text_attribute::BLUE:
            result = ::fprintf(stream, "\033[0;34m");
            break;
        case text_attribute::PURPLE:
            result = ::fprintf(stream, "\033[0;35m");
            break;
        case text_attribute::CYAN:
            result = ::fprintf(stream, "\033[0;36m");
            break;
        case text_attribute::LIGHT_GRAY:
            result = ::fprintf(stream, "\033[0;37m");
            break;
        case text_attribute::DARK_GRAY:
            result = ::fprintf(stream, "\033[1;30m");
            break;
        case text_attribute::LIGHT_RED:
            result = ::fprintf(stream, "\033[1;31m");
            break;
        case text_attribute::LIGHT_GREEN:
            result = ::fprintf(stream, "\033[1;32m");
            break;
        case text_attribute::YELLOW:
            result = ::fprintf(stream, "\033[1;33m");
            break;
        case text_attribute::LIGHT_BLUE:
            result = ::fprintf(stream, "\033[1;34m");
            break;
        case text_attribute::LIGHT_PURPLE:
            result = ::fprintf(stream, "\033[1;35m");
            break;
        case text_attribute::LIGHT_CYAN:
            result = ::fprintf(stream, "\033[1;36m");
            break;
        case text_attribute::WHITE:
            result = ::fprintf(stream, "\033[1;37m");
            break;
    }
    
    if (result < 0)
    {
        return -1;
    }
    return 0;
#else
#error "system not supported"
#endif
}


int set_ostream_text_attribute(std::ostream& os, text_attribute attribute)
{
#ifdef __unix__
    int result = 0;
    
    switch (attribute)
    {
        case text_attribute::DEFAULT:
            os << "\033[0m";
            break;
        case text_attribute::BLACK:
            os << "\033[0;30m";
            break;
        case text_attribute::RED:
            os << "\033[0;31m";
            break;
        case text_attribute::GREEN:
            os << "\033[0;32m";
            break;
        case text_attribute::BROWN:
            os << "\033[0;33m";
            break;
        case text_attribute::BLUE:
            os << "\033[0;34m";
            break;
        case text_attribute::PURPLE:
            os << "\033[0;35m";
            break;
        case text_attribute::CYAN:
            os << "\033[0;36m";
            break;
        case text_attribute::LIGHT_GRAY:
            os << "\033[0;37m";
            break;
        case text_attribute::DARK_GRAY:
            os << "\033[1;30m";
            break;
        case text_attribute::LIGHT_RED:
            os << "\033[1;31m";
            break;
        case text_attribute::LIGHT_GREEN:
            os << "\033[1;32m";
            break;
        case text_attribute::YELLOW:
            os << "\033[1;33m";
            break;
        case text_attribute::LIGHT_BLUE:
            os << "\033[1;34m";
            break;
        case text_attribute::LIGHT_PURPLE:
            os << "\033[1;35m";
            break;
        case text_attribute::LIGHT_CYAN:
            os << "\033[1;36m";
            break;
        case text_attribute::WHITE:
            os << "\033[1;37m";
            break;
    }
    
    return 0;
#else
#error "system not supported"
#endif
}


}
}
