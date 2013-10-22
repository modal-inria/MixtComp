CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)

# project declaration
project (MixtComp) 

# set this variable to specify a common place where CMake should put all libraries
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
SET(LIBRARY_NAME MixtComp)

set(ZERO_BASED_ARRAYS "ON")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -std=c++11 -DSTKBASEARRAYS=0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -std=c++11 -DNDEBUG -DSTKBASEARRAYS=0")

SET(CMAKE_DEFAULT_BUILD_TYPE "DEBUG")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/binaries)

SET(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(src)
link_directories(./src/stkpp/lib)

#add_subdirectory(externlibs)
#add_subdirectory(examples)
