CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)

# project declaration
PROJECT(STKPP)

# set this variable to specify a common place where CMake should put all libraries
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

# define the libraries generated
SET(LIBRARY_NAME STKpp)
SET(LIBRARY_TEST STKtest)

# header files inclusion
INCLUDE_DIRECTORIES(include)

# option for zero based arrays
# disable with  cmake ../CMakeLists.txt -DZERO_BASED_ARRAYS=off
OPTION(ZERO_BASED_ARRAYS "Set" ON)
IF(NOT ZERO_BASED_ARRAYS)
  ADD_DEFINITIONS(-DSTKBASEARRAYS=1)
ENDIF(NOT ZERO_BASED_ARRAYS)

# option for real representation
OPTION(REAL_ARE_FLOAT "Set" OFF)
IF(REAL_ARE_FLOAT)
   ADD_DEFINITIONS(-DSTKREALAREFLOAT)
ENDIF(REAL_ARE_FLOAT)

# 
# Check for required libraries
# FIND_PACKAGE( xxx REQUIRED ) : REQUIRED removed so that MESSAGE is written
#

# LAPACK
FIND_LIBRARY(LAPACK_LAPACK_LIBRARY
  NAMES lapack
  PATHS ${SYSTEM_LIB_DIRS}
        ${SOURCE_BASE_DIR}/
        ${SOURCE_BASE_DIR}/lapack
  ) 
IF(LAPACK_LAPACK_LIBRARY)
  SET( STKUSELAPACK 1 )
  SET( LAPACK_LIBRARIES ${LAPACK_LAPACK_LIBRARY} )
  ADD_DEFINITIONS(-DSTKUSELAPACK=1)
  MESSAGE(STATUS "Found LAPACK library: " ${LAPACK_LIBRARIES})
ELSE(LAPACK_LAPACK_LIBRARY)
  SET( STKUSELAPACK 0 )
  ADD_DEFINITIONS(-DSTKUSELAPACK=0)
  MESSAGE(STATUS "Warning --> Package LAPACK not installed!")
ENDIF(LAPACK_LAPACK_LIBRARY)

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

#-----------------
# create stkpp library
ADD_SUBDIRECTORY(projects)
ADD_LIBRARY(${LIBRARY_NAME}
 $<TARGET_OBJECTS:STKSTKernel>
 $<TARGET_OBJECTS:STKArrays>
 $<TARGET_OBJECTS:STKSTatistiK>
 $<TARGET_OBJECTS:STKAlgebra>
 $<TARGET_OBJECTS:STKAnalysis>
 $<TARGET_OBJECTS:STKClustering>
 $<TARGET_OBJECTS:STKDManager>
 $<TARGET_OBJECTS:STKReduct>
 $<TARGET_OBJECTS:STKRegress>
 $<TARGET_OBJECTS:STKStatModels>
 $<TARGET_OBJECTS:STKAAModels>
)
SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)
INSTALL(TARGETS ${LIBRARY_NAME} ARCHIVE DESTINATION ${LIBRARY_OUTPUT_PATH})

#-----------------
# add test programs
INCLUDE(CTest)
ENABLE_TESTING() # must be called from the root CMakeLists, see man page
SET(CTEST_BINARY_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

ADD_SUBDIRECTORY(tests)

