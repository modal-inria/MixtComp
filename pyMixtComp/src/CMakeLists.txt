project (pyMixtComp CXX)

cmake_minimum_required (VERSION 3.5)

# C++11

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


## Boost

# set(Boost_USE_STATIC_LIBS ON) # use .a instead of .so
find_package(Boost 1.58 COMPONENTS python REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

## Eigen

#find_package(Eigen3 REQUIRED) # TODO: add required version of compilation problems occur (putting 3.3 did not work on Ubuntu)
#include_directories( ${EIGEN3_INCLUDE_DIR} )

# python
find_package(PythonInterp)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})


#add_definitions(-DMC_VERBOSE -DEIGEN_MATRIXBASE_PLUGIN=\"${CMAKE_CURRENT_SOURCE_DIR}/../../MixtComp/src/lib/LinAlg/EigenMatrixBaseAddons.h\")
#include_directories(${CMAKE_CURRENT_BINARY_DIR}/MixtComp/optim-src/include)
#include_directories("../../MixtComp/src/lib")
#add_subdirectory("../../MixtComp/src/" "MixtComp")

include_directories(${MIXTCOMP_INCLUDE_DIRS})
add_definitions(${MIXTCOMP_DEFINITIONS})

add_library(pyMixtComp
  pyMixtComp.cpp
  PyGraph.h
  PyGraph.cpp
  translateCPPToPython.h
  translatePythonToCPP.h
)

target_link_libraries(pyMixtComp MixtComp ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})


set_target_properties(pyMixtComp PROPERTIES SUFFIX .so)
set_target_properties(pyMixtComp PROPERTIES PREFIX "")

