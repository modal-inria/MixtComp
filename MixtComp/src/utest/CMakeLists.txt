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




# long file list initially generated with "find . -type f -follow -print"
# with "./" prefix deleted, and some utest deactivated

add_executable(runUtest
    IO/MisValParser.cpp
    LinAlgStat/names.cpp
    LinAlgStat/UTestMath.cpp
    LinAlgStat/UTestStatistics.cpp
    LinAlgStat/UTestLinAlg.cpp
    Functional/UTestOptim.cpp
    Simple/UTestPoisson.cpp
    Simple/UTestCategorical.cpp
    Simple/UTestWeibull.cpp
    Simple/UTestGaussian.cpp
    Simple/UTestNegativeBinomial.cpp
    Various/UTestVarious.cpp
    Various/UTestMisValParser.cpp
    Various/UTestStandardLibrary.cpp
    Various/UTestRegex.cpp
    Statistic/UTestNegativeBinomialStatistic.cpp
    Statistic/UTestWeibullStatistic.cpp
    Statistic/UTestNormalStatistic.cpp
    Statistic/UTestUniformIntStatistic.cpp
    Statistic/UTestPoissonStatistic.cpp
    Statistic/UTestRNG.cpp
    Rank/UTestRankIndividual.cpp
    Rank/UTestRankParamStat.cpp
    Rank/UTestRankParser.cpp
    Rank/UTestRankVal.cpp
    Rank/UTestRankClass.cpp
    Rank/UTestRankLikelihood.cpp
    Functional/UTestFunctionalComputation.cpp
    Functional/UTestFunction.cpp
    Functional/UTestFunctionalClass.cpp
    Functional/UTestFunctionalMixture.cpp
    Functional/UTestFunctionalParser.cpp
)

target_link_libraries(runUtest
    gtest_main
    MixtComp
)

