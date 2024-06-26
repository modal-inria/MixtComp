
cmake_minimum_required(VERSION 3.8)

project(JMixtComp)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compile MixtComp
add_subdirectory(../../MixtComp/src MixtComp)

# add MixtComp's include directories and definitions
include_directories(${MixtComp_INCLUDE_DIRS})
add_definitions(${MixtComp_DEFINITIONS})


# json
include(CMakeLists.txt.in)
include_directories(${CMAKE_BINARY_DIR}/json-src/single_include/nlohmann) # nlohmann is a header-only library


# compilation flags
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/")
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # to enable PIC on platforms that need it

# add json interface code
add_subdirectory(lib)

# add test
add_subdirectory(utest EXCLUDE_FROM_ALL)
