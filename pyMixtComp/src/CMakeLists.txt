project (pyMixtComp CXX)

cmake_minimum_required (VERSION 3.5)

# C++11

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Boost

find_package(Boost 1.58 COMPONENTS python REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})


# python
find_package(PythonInterp)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})


# MixtComp
include_directories(${MixtComp_INCLUDE_DIRS})
add_definitions(${MixtComp_DEFINITIONS})

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
