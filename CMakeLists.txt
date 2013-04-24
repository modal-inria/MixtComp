cmake_minimum_required (VERSION 2.6)

project (MixtComp) 

set(CMAKE_CXX_FLAGS_DEBUG "-g -ftest-coverage -fprofile-arcs -Wall -std=c++0x")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++0x -DNDEBUG")

SET(CMAKE_DEFAULT_BUILD_TYPE "DEBUG")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/binaries)

SET(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(src)
include_directories(externlibs)
add_subdirectory(externlibs)
add_subdirectory(src)
add_subdirectory(examples)
#add_subdirectory(examples)