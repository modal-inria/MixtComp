project (pyMixtComp CXX)

cmake_minimum_required (VERSION 3.5)

# C++11

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# output a compile_commands.json used for editor tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## Boost

# set(Boost_USE_STATIC_LIBS ON) # use .a instead of .so
find_package(Boost 1.62 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED) # TODO: add required version of compilation problems occur (putting 3.3 did not work on Ubuntu)
include_directories( ${EIGEN3_INCLUDE_DIR} )

# python
include_directories("/usr/include/python3.6")
find_package(PythonLibs)
link_libraries(boost_python3 ${PYTHON_LIBRARIES})

add_definitions(-DMC_VERBOSE -DEIGEN_MATRIXBASE_PLUGIN=\"/home/leo/workspace/MixtComp/MixtComp/src/lib/LinAlg/mixt_EigenMatrixBaseAddons.h\")
include_directories(${CMAKE_CURRENT_BINARY_DIR}/MixtComp/optim-src/include)

include_directories("../../MixtComp/src/lib")
add_subdirectory("../../MixtComp/src/" "MixtComp")

python_add_module(pyMixtComp
  pyMixtComp.cpp
  PyGraph.hpp
  PyGraph.cpp
  translateCPPToPython.hpp
  translatePythonToCPP.hpp
)
