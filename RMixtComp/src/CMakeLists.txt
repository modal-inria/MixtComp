project (RMixtComp)

cmake_minimum_required (VERSION 3.5)

# REQUIRED LIBRARIES
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/cmakeModule/")

## Boost
#set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.58 REQUIRED)

## nlopt
find_package(NLOPT 2.4.2 REQUIRED)

## eigen
find_package(Eigen3 REQUIRED) # TODO: add required version of compilation problems occur (putting 3.3 did not work on Ubuntu)

file(WRITE Libvars
    "EIGEN3_INCLUDE_DIR=${EIGEN3_INCLUDE_DIR}\n"
    "Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}\n"
    "NLOPT_INCLUDE_DIRS=${NLOPT_INCLUDE_DIRS}\n"
    "NLOPT_LIBRARIES=${NLOPT_LIBRARIES}\n"
)
