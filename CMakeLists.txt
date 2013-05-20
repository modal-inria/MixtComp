cmake_minimum_required (VERSION 2.6)

project (MixtComp) 

set(ZERO_BASED_ARRAYS "ON")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -std=c++0x -DSTKBASEARRAYS=0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++0x -DNDEBUG -DSTKBASEARRAYS=0")

SET(CMAKE_DEFAULT_BUILD_TYPE "DEBUG")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/binaries)

SET(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(src)
include_directories(externlibs)
add_subdirectory(externlibs)
add_subdirectory(src)
add_subdirectory(examples)
