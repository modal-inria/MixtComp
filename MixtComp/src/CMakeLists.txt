project (MixtComp CXX)

cmake_minimum_required (VERSION 3.5)

# C++11

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Boost

# set(Boost_USE_STATIC_LIBS ON) # use .a instead of .so
find_package(Boost 1.58 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

## openMP

#find_package(OpenMP REQUIRED)
#if (OPENMP_FOUND)
#    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()

## eigen, easy to install from package manager

find_package(Eigen3 REQUIRED) # TODO: add required version of compilation problems occur (putting 3.3 did not work on Ubuntu)
include_directories( ${EIGEN3_INCLUDE_DIR} )

## Google Test

# Download and unpack googletest at configure time.
# This is the recommended method of getting the library, because it ensures consistency of build flags
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
)
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(
    COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
)
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# googletest uses CMake, hence it is possible to simply add the subrirectory to the build
add_subdirectory(
    ${CMAKE_CURRENT_BINARY_DIR}/googletest-src
    ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
    EXCLUDE_FROM_ALL
)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

## json

configure_file(CMakeLists.txt.in json-download/CMakeLists.txt)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/json-src/single_include/nlohmann) # nlohmann is a header-only library

## optim

configure_file(CMakeLists.txt.in optim-download/CMakeLists.txt)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/optim-src/include) # CppNumericalSolvers is a header-only library

# compilation flags

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/json/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/")
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # to enable PIC on platforms that need it
add_definitions(-DMC_VERBOSE -DEIGEN_MATRIXBASE_PLUGIN=\"${CMAKE_CURRENT_SOURCE_DIR}/lib/LinAlg/EigenMatrixBaseAddons.h\")
# add_definitions(-DEIGEN_MATRIXBASE_PLUGIN=\"${CMAKE_CURRENT_SOURCE_DIR}/lib/LinAlg/EigenMatrixBaseAddons.h\")

# json interface
add_subdirectory(json)

# MIXTCOMP LIBRARY TO COMPILE
add_subdirectory(lib)

## unit tests
add_subdirectory(utest)


# Expose MixtComp's definitions to other subprojects through cache variable.
set(${PROJECT_NAME}_DEFINITIONS -DMC_VERBOSE -DEIGEN_MATRIXBASE_PLUGIN=\"${CMAKE_CURRENT_SOURCE_DIR}/lib/LinAlg/EigenMatrixBaseAddons.h\"
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

# Expose MixtComp's public includes to other subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS ${Boost_INCLUDE_DIRS}
                                 ${EIGEN3_INCLUDE_DIRS}
                                 "${CMAKE_CURRENT_SOURCE_DIR}/lib/"
                                 "${CMAKE_CURRENT_BINARY_DIR}/json-src/single_include/nlohmann"
                                 "${CMAKE_CURRENT_BINARY_DIR}/optim-src/include"
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
    
