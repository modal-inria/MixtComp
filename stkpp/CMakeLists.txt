cmake_minimum_required(VERSION 2.8.8)

# project declaration
project(STKpp)

# set this variable to specify a common place where CMake should put all libraries
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(LIBRARY_NAME STKpp)
set(LIBRARY_TEST STKtest)

# header files inclusion
include_directories(include)

# option for zero based arrays
# disable with  cmake ../CMakeLists.txt -DZERO_BASED_ARRAYS=off
option(ZERO_BASED_ARRAYS "Set" ON)
if(NOT ZERO_BASED_ARRAYS)
  add_definitions(-DSTKBASEARRAYS=1)
endif(NOT ZERO_BASED_ARRAYS)

# option for real representation
option(REAL_ARE_FLOAT "Set" OFF)
if(REAL_ARE_FLOAT)
   add_definitions(-DSTKREALAREFLOAT)
endif(REAL_ARE_FLOAT)

# Set default Release/Debug mode
# enable with  cmake ../CMakeLists.txt -DCMAKE_BUILD_TYPE=Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug) # Debug should appear in other libraries/programs too
endif(NOT CMAKE_BUILD_TYPE)

message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})

# compiler flags for each mode
set(CMAKE_CXX_FLAG "-ansi -pedantic -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ftree-vectorize -march=native -funroll-loops")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0 -DSTK_DEBUG -DSTK_BOUNDS_CHECK ")

#------------------------------------------------------------------------------- 
# Check for required libraries
# LAPACK
find_library(LAPACK_LIB
             NAMES lapack
            )

if(LAPACK_LIB)
  set(STKUSELAPACK 1 )
  add_definitions(-DSTKUSELAPACK=1)
  set(LAPACK_LIBRARIES ${LAPACK_LIB} )
  message(STATUS "Found LAPACK library --> " ${LAPACK_LIBRARIES})
else(LAPACK_LIB)
  set( STKUSELAPACK 0 )
  add_definitions(-DSTKUSELAPACK=0)
  message(STATUS "Warning --> library LAPACK not installed!")
endif(LAPACK_LIB)

# OPENMP
include(FindOpenMP OPTIONAL)
if(OPENMP_FOUND)
  message("OpenMP parallelization enabled")
  add_definitions("${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_CXX_FLAGS}")
  message(STATUS "Found OPENMP library --> " ${OpenMP_CXX_FLAGS})
else(OPENMP_FOUND)
  add_definitions(-Wno-unknown-pragmas)
  message("OpenMP parallelization disabled")
endif()

#------------------------------------------------------------------------------- 
# create stkpp library
add_subdirectory(projects)
add_library(${LIBRARY_NAME}
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
set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ${LIBRARY_NAME} ARCHIVE DESTINATION ${LIBRARY_OUTPUT_PATH})

#------------------------------------------------------------------------------- 
# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc
                    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    COMMENT "Generating API documentation with Doxygen" VERBATIM
                   )
endif(DOXYGEN_FOUND)

#------------------------------------------------------------------------------- 
# add test programs
include(CTest)
enable_testing() # must be called from the root CMakeLists, see man page
set(CTEST_BINARY_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

add_subdirectory(tests)

