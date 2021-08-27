
cmake_minimum_required(VERSION 3.5)

project(pyMixtComp)

# C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# boost
find_package(Boost 1.58 COMPONENTS python numpy REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# python
find_package(PythonInterp 3.6)
find_package(PythonLibs 3.6 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})


# Compile MixtComp
add_compile_options("-DMINMODALITY=0")
add_subdirectory(../../MixtComp/src MixtComp)

# add MixtComp's include directories and definitions
include_directories(${MixtComp_INCLUDE_DIRS})
add_definitions(${MixtComp_DEFINITIONS})

# add pyMixtComp include
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # to enable PIC on platforms that need it
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/")

# add python interface code
add_subdirectory(lib)

# add test
add_subdirectory(utest EXCLUDE_FROM_ALL)
