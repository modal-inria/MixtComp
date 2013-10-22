CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)

project (MixtComp) 

set(ZERO_BASED_ARRAYS "ON")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -std=c++11 -DSTKBASEARRAYS=0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++11 -DNDEBUG -DSTKBASEARRAYS=0")

SET(CMAKE_DEFAULT_BUILD_TYPE "DEBUG")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/binaries)

SET(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(src)
include_directories(../)
link_directories(./src/stkpp/lib)
#add_subdirectory(externlibs)
add_subdirectory(src)
add_subdirectory(examples)
