CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)

# project declaration
PROJECT(STKPP)

# set this variable to specify a common place where CMake should put all libraries
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

SET(LIBRARY_NAME STKpp)
SET(LIBRARY_TEST STKtest)

# header files inclusion
INCLUDE_DIRECTORIES(include)

# option for zero based arrays
# enable with  cmake ../CMakeLists.txt -DZERO_BASED_ARRAYS=on
OPTION(ZERO_BASED_ARRAYS "Set" OFF)
IF(ZERO_BASED_ARRAYS)
    ADD_DEFINITIONS(-DSTKBASEARRAYS=0)
ENDIF(ZERO_BASED_ARRAYS)

# option for real representation
OPTION(REAL_ARE_FLOAT "Set" OFF)
IF(REAL_ARE_FLOAT)
    ADD_DEFINITIONS(-DSTKREALAREFLOAT)
ENDIF(REAL_ARE_FLOAT)

# Set default Release/Debug mode
# enable with  cmake ../CMakeLists.txt -DCMAKE_BUILD_TYPE=Release
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "Debug") # Debug should appear in other libraries/programs too
ENDIF(NOT CMAKE_BUILD_TYPE)

MESSAGE(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})

# compiler flags for each mode
SET(CMAKE_CXX_FLAGS_RELEASE "-ansi -pedantic -Wall -O3 -ftree-vectorize -march=native -funroll-loops")
SET(CMAKE_CXX_FLAGS_DEBUG   "-ansi -pedantic -Wall -g -Os -DSTK_DEBUG -DSTK_BOUNDS_CHECK ")

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

ADD_SUBDIRECTORY(tests/STKernel)
ADD_SUBDIRECTORY(tests/Algebra)
ADD_SUBDIRECTORY(tests/Arrays)
ADD_SUBDIRECTORY(tests/DManager)
ADD_SUBDIRECTORY(tests/StatModels)
ADD_SUBDIRECTORY(tests/Regress)
ADD_SUBDIRECTORY(tests/AAModels)
ADD_SUBDIRECTORY(tests/STatistiK)
ADD_SUBDIRECTORY(tests/Clustering)

