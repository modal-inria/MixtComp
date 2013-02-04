CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# project declaration
PROJECT(STKPP)

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
    ADD_DEFINITIONS(-DREALAREFLOAT=float)
ENDIF(REAL_ARE_FLOAT)

# Set default Release/Debug mode
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Debug") # Debug should appear in other libraries/programs too
ENDIF(NOT CMAKE_BUILD_TYPE)

MESSAGE(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})

# compiler flags for each mode
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -fpic -ansi -pedantic -Wall -march=native -ftree-vectorize -funroll-loops")
SET(CMAKE_CXX_FLAGS_DEBUG   "-O1 -g -fpic -ansi -pedantic -Wall -DSTK_DEBUG -DSTK_BOUNDS_CHECK ")

# add a target to generate API documentation with Doxygen
FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  ADD_CUSTOM_TARGET(doc
                    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    COMMENT "Generating API documentation with Doxygen" VERBATIM
                   )
ENDIF(DOXYGEN_FOUND)

ADD_SUBDIRECTORY(projects)

# add test programs
INCLUDE(CTest)
ENABLE_TESTING() # must be called from the root CMakeLists, see man page
SET(CTEST_BINARY_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(tests/Algebra)
ADD_SUBDIRECTORY(tests/Arrays)
ADD_SUBDIRECTORY(tests/DManager)
