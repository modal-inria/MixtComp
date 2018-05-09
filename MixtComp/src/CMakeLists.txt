project (MixtComp CXX)

cmake_minimum_required (VERSION 3.5)

# REQUIRED LIBRARIES
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/cmakeModule/")

## Boost
find_package(Boost 1.58 COMPONENTS regex random REQUIRED)
include_directories(${BOOST_INCLUDE_DIRS})

## openMP
#find_package(OpenMP REQUIRED)
#if (OPENMP_FOUND)
#    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()

## nlopt
find_package(NLOPT 2.4.2 REQUIRED)
include_directories(${NLOPT_INCLUDE_DIRS})

## eigen
find_package(Eigen3 REQUIRED) # TODO: add required version of compilation problems occur (putting 3.3 did not work on Ubuntu)
include_directories( ${EIGEN3_INCLUDE_DIR} )

## Google Test
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# COMPILATION FLAGS

## C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## -I
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib/")

## others cxx flags
SET(MATRIXBASE_PLUGIN_FLAGS -DEIGEN_MATRIXBASE_PLUGIN=\\"${CMAKE_CURRENT_SOURCE_DIR}/lib/LinAlg/mixt_EigenMatrixBaseAddons.h\\")
SET(OTHER_CXX_FLAGS "-fPIC")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MATRIXBASE_PLUGIN_FLAGS} ${OTHER_CXX_FLAGS}")

# MIXTCOMP LIBRARY TO COMPILE
add_subdirectory(lib)

# EXPORT THE OBJECT FILES

# EXECUTABLES

## unit tests
file(GLOB UTEST_SRC utest/*/*.cpp)
add_executable(runUtest ${UTEST_SRC} utest/UTest.h)
target_link_libraries(runUtest gtest_main MixtComp ${Boost_LIBRARIES} ${NLOPT_LIBRARIES} ${ARMADILLO_LIBRARIES})
