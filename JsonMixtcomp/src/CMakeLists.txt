project (MixtComp CXX)

cmake_minimum_required (VERSION 3.5)

# REQUIRED LIBRARIES
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/cmakeModule/")

## Boost
# set(Boost_USE_STATIC_LIBS ON) # use .a instead of .so
find_package(Boost 1.58 COMPONENTS regex REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

## openMP
#find_package(OpenMP REQUIRED)
#if (OPENMP_FOUND)
#    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()

## nlopt
IF (WIN32 OR CYGWIN)
	set(NLOPT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/nlopt/api/")
	set(NLOPT_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/nlopt/libnlopt.a")
	include_directories(${NLOPT_INCLUDE_DIRS})
ELSE()
	find_package(NLOPT 2.4.2 REQUIRED)
	include_directories(${NLOPT_INCLUDE_DIRS})
ENDIF()

## eigen
find_package(Eigen3 REQUIRED) # TODO: add required version of compilation problems occur (putting 3.3 did not work on Ubuntu)
include_directories( ${EIGEN3_INCLUDE_DIR} )

## json
configure_file(CMakeLists.txt.in json-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/json-download )
if(result)
  message(FATAL_ERROR "CMake step for json failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/json-download )
if(result)
  message(FATAL_ERROR "Build step for json failed: ${result}")
endif()

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
include_directories(${CMAKE_BINARY_DIR}/json-src/single_include/nlohmann)

# COMPILATION FLAGS

## C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## -I
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../MixtComp/src/lib/")

## others cxx flags
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # to enable PIC on platforms that need it
add_definitions(-DEIGEN_MATRIXBASE_PLUGIN=\"${CMAKE_CURRENT_SOURCE_DIR}/../../MixtComp/src/lib/LinAlg/mixt_EigenMatrixBaseAddons.h\")

# MIXTCOMP LIBRARY TO COMPILE
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../MixtComp/src/lib/" "MixtComp")

# JSONMIXTCOMP EXECUTABLE TO COMPILE
add_subdirectory("exec")
