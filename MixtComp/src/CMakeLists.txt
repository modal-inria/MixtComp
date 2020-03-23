project (MixtComp CXX)

cmake_minimum_required (VERSION 3.5)

# C++11

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## add current path as module path to load code coverage module
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

## load code coverage module only in Coverage mode
if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
	include(CodeCoverage)
	append_coverage_compiler_flags()
endif() 


## Boost

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


## optim

include(CMakeLists.txt.in)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/optim-src/include) # CppNumericalSolvers is a header-only library

# compilation flags

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/")
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # to enable PIC on platforms that need it
add_definitions(-DMC_VERBOSE -DEIGEN_MATRIXBASE_PLUGIN=\"${CMAKE_CURRENT_SOURCE_DIR}/lib/LinAlg/EigenMatrixBaseAddons.h\")
# add_definitions(-DEIGEN_MATRIXBASE_PLUGIN=\"${CMAKE_CURRENT_SOURCE_DIR}/lib/LinAlg/EigenMatrixBaseAddons.h\")


# MIXTCOMP LIBRARY TO COMPILE
add_subdirectory(lib)


# Expose MixtComp's definitions to other subprojects through cache variable.
set(${PROJECT_NAME}_DEFINITIONS -DMC_VERBOSE -DEIGEN_MATRIXBASE_PLUGIN=\"${CMAKE_CURRENT_SOURCE_DIR}/lib/LinAlg/EigenMatrixBaseAddons.h\"
    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

# Expose MixtComp's public includes to other subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS ${Boost_INCLUDE_DIRS} 
                                 ${EIGEN3_INCLUDE_DIRS} 
                                 "${CMAKE_CURRENT_SOURCE_DIR}/lib/" 
                                 "${CMAKE_CURRENT_BINARY_DIR}/optim-src/include" 
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)



## unit tests
add_subdirectory(utest EXCLUDE_FROM_ALL)

## code coverage only in Coverage mode
if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
	setup_target_for_coverage_lcov(NAME ${PROJECT_NAME}_coverage EXECUTABLE runUtest EXCLUDE "\"/usr*\"" "\"*debug*\"" "\"*release*\"" "\"*utest*\"" "${PROJECT_SOURCE_DIR}/CMakeCCompilerId*")
endif() 
