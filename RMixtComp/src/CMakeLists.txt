project (RMixtComp)

cmake_minimum_required (VERSION 3.5)

## Boost
#set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.58 REQUIRED)

## eigen
find_package(Eigen3 REQUIRED) # TODO: add required version of compilation problems occur (putting 3.3 did not work on Ubuntu)

file(WRITE Libvars
    "EIGEN3_INCLUDE_DIR=${EIGEN3_INCLUDE_DIR}\n"
    "Boost_INCLUDE_DIRS=${Boost_INCLUDE_DIRS}\n"
)
