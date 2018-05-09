project (RMixtComp)

cmake_minimum_required (VERSION 3.5)

# REQUIRED LIBRARIES
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/cmakeModule/")

## Boost
find_package(Boost 1.58 COMPONENTS regex random REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

## openMP
#find_package(OpenMP REQUIRED)
#if (OPENMP_FOUND)
#    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()

## nlopt
find_package(NLOPT 2.4.2 REQUIRED)
include_directories(${NLOPT_INCLUDE_DIRS})

## eigen
find_package(Eigen3 REQUIRED) # TODO: add required version of compilation problems occur (putting 3.3 did not work on Ubuntu)
include_directories( ${EIGEN3_INCLUDE_DIR} )
