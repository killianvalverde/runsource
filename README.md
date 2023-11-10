### runsource ###

runsource is a simple C++ program whose main feature is to compile source files, run the produced 
program, and delete it. runsource was programmed with the objective of introducing it into the 
right-click context menu of file managers. In the `./context_menu` folder, there is a configuration 
file for nemo. In the case of nemo, the configuration files are located in 
`/usr/share/nemo/actions`. For more information, use the `runsource --help` command

### Build ###

Use the folowing commands to buil and install the CMake project.

    Create a directory to hold the build output and generate the native build scripts:
            $ cmake -H. -Bbuild

    Compile the project directly from CMake using the native build scripts:
            $ cmake --build build

    Install the binary in your environment:
            $ sudo cmake --install build