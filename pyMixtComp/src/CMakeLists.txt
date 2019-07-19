
cmake_minimum_required(VERSION 3.5)

# boost
find_package(Boost 1.58  COMPONENTS python numpy REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# python
find_package(PythonInterp)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})


set(CMAKE_POSITION_INDEPENDENT_CODE ON) # to enable PIC on platforms that need it

# Compile MixtComp
add_subdirectory(../../MixtComp/src MixtComp) 

# add MixtComp's include directories and definitions
include_directories(${MixtComp_INCLUDE_DIRS})
add_definitions(${MixtComp_DEFINITIONS})


include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/")

# add python interface code
add_subdirectory(lib) 

# add test (google test is included via MixtComp)
add_subdirectory(utest) 


