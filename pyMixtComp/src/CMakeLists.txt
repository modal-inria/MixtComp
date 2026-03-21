cmake_policy(SET CMP0148 NEW) # Suppress FindPythonInterp/Libs warnings
cmake_policy(SET CMP0167 NEW) # Suppress FindBoost warnings

cmake_minimum_required(VERSION 3.10)

project(pyMixtComp)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# boost
find_package(Boost 1.58 COMPONENTS python numpy REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# python
find_package(Python3 3.6 COMPONENTS Interpreter Development REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})

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
