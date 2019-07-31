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
