cmake_minimum_required (VERSION 2.6)

project (EMFramework) 

set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -std=c++0x")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++0x -Wall")

SET(CMAKE_DEFAULT_BUILD_TYPE "DEBUG")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/binaries)

SET(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(src)
include_directories(../)
link_directories(../stkpp/lib)
add_subdirectory(src)