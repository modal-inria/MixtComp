#Build STKPP library
# set this variable to specify a common place where CMake should put all libraries
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
SET(LIBRARY_NAME STKpp)

# header files inclusion
INCLUDE_DIRECTORIES(include)

# option for zero based arrays
OPTION(ZERO_BASED_ARRAYS "Set" OFF)
IF(ZERO_BASED_ARRAYS)
    ADD_DEFINITIONS(-DSTKBASEARRAYS=0)
ENDIF(ZERO_BASED_ARRAYS)

# option for real representation
OPTION(REAL_ARE_FLOAT "Set" OFF)
IF(REAL_ARE_FLOAT)
    ADD_DEFINITIONS(-DREALAREFLOAT)
ENDIF(REAL_ARE_FLOAT)

# Set default Release/Debug mode
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Debug") # Debug should appear in other libraries/programs too
ENDIF(NOT CMAKE_BUILD_TYPE)

MESSAGE(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})

# compiler flags for each mode
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -fpic -ansi -pedantic -Wall -march=native -ftree-vectorize -funroll-loops")
SET(CMAKE_CXX_FLAGS_DEBUG   "-O1 -g -fpic -ansi -pedantic -Wall -DSTK_DEBUG -DSTK_BOUNDS_CHECK ")

ADD_SUBDIRECTORY(stkpp/projects)


#add_subdirectory(stkpp)