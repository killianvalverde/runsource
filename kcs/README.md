### kcs library ###

Welcome everybody, my name is Killian and this is my personal C++ library.
This library is open source and cross platform software protected by GPLv3+ licence, so you are free 
to change and redistribute it.

#### Features ####

Today's main feature of kcs is an argument parser.
The argparse module makes it easy to write user-friendly command-line interfaces. The program 
defines what arguments it requires, and argparse will figure out how to parse those out of argv. The 
argparse module also automatically generates help and usage messages and issues errors when users 
give the program invalid arguments. The argparse are defined in `kcs::argparse` namespace.

### Build ###

kcs comes with a CMake build script hat can be used on a wide range of platforms.
If you don't have CMake installed already, you can download it for free from 
<http://www.cmake.org/>.

CMake works by generating native makefiles or build projects that can be used in the compiler 
environment of your choice.  You can either build kcs as a standalone project or it can be 
incorporated into an existing CMake build for another project.

#### Warnings ####

In order to compile this software you have to use a C++ revision equal or highter to C++14 
(ISO/IEC 14882:2014).

#### Standalone CMake Project ####

When building kcs as a standalone project, the typical workflow starts with:

    mkdir mybuild       # Create a directory to hold the build output.
    cd mybuild
    cmake ..            # Generate native build scripts.

If you are on a \*nix system, you should now see a Makefile in the current directory.  
Just type 'make' to build kcs.

#### Incorporating Into An Existing CMake Project ####

If you want to use kcs in a project which already uses CMake, then a more robust and flexible 
approach is to build kcs as part of that project directly. 
This is done by making the kcs source code available to the main build and adding it using CMake's
`add_subdirectory()` command. 
This has the significant advantage that the same compiler and linker settings are used between kcs 
and the rest of your project, so issues associated with using incompatible libraries 
(eg debug/release), etc. are avoided.

### Documentation ###

If you want to generate doxygen documentation you can use `doxygen Doxyfile` command and in the 
./doc directory will be placed the result.
