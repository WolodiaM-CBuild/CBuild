# TODO
## In some version ...
 * Import repos
    * VCPKG
    * Conan
	...
## In some more later version
 * Import CMake projects

## Unimplemented and not seen in near future
mingw debugger

# Depends
### Libs:
 * libc6
 * libstdc++
### Shell commands:
 * For main run
    * rm
    * debuger, default:
        * gdb
    * compiler, default:
        * gcc
        * g++
        * g++-mingw-w64-x86-64-win32
        * gcc-mingw-w64-x86-64-win32
 * For build.run executable for building cbuild
    * Required
        * cp
        * mv
        * cd
    * Deb packet
        * gpg
        * dpkg-scanpackages
        * apt-ftparchive
        * gzip
        * dpkg-deb
# Sizes of files
 * All (except .git/ .vscode/ build/ cache/ src/ compile_command.json Makefile scripts/ CBuild.run) - 18M
 * Source code (hpp + cpp + rebuild.sh) - 228K
 * Build script (build.sh + build.run + buildscript-CBuild/ ) - 928K
 * libCBuild.so (with -g) - 4.8M    ! updated not as frequently as other values to to not compiling with this flag
 * libCBuild.so (without -g) - 1.8M
 * deb folder - 1.7M
 * ppa/ubuntu folder - 7.1M
 * doxygen folder + doxygen.conf - 5.9M  
All sizes are generated using `du -hcs <folder/file0> <folder/file1> ...`

