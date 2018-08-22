# long file list initially generated with "find . -type f -follow -print"
# with "./" prefix deleted, and some utest deactivated

add_executable(runUtest
    json/JSONGraph.cpp
    lib/LinAlgStat/UTestMath.cpp
    lib/LinAlgStat/UTestStatistics.cpp
    lib/LinAlgStat/UTestLinAlg.cpp
    lib/Simple/UTestPoisson.cpp
    lib/Simple/UTestCategorical.cpp
    lib/Simple/UTestWeibull.cpp
    lib/Simple/UTestGaussian.cpp
    lib/Simple/UTestNegativeBinomial.cpp
    lib/Various/UTestVarious.cpp
    lib/Various/UTestMisValParser.cpp
    lib/Various/UTestStandardLibrary.cpp
    lib/Various/UTestRegex.cpp
    lib/Statistic/UTestNegativeBinomialStatistic.cpp
    lib/Statistic/UTestWeibullStatistic.cpp
    lib/Statistic/UTestNormalStatistic.cpp
    lib/Statistic/UTestRNG.cpp
    lib/Rank/UTestRankIndividual.cpp
    lib/Rank/UTestRankParamStat.cpp
    lib/Rank/UTestRankParser.cpp
    lib/Rank/UTestRankVal.cpp
    lib/Rank/UTestRankClass.cpp
    lib/Rank/UTestRankLikelihood.cpp
    lib/Functional/UTestFunctionalComputation.cpp
    lib/Functional/UTestFunction.cpp
    lib/Functional/UTestFunctionalClass.cpp
    lib/Functional/UTestFunctionalMixture.cpp
    lib/Functional/UTestFunctionalParser.cpp
)

target_link_libraries(runUtest
    gtest_main
    JMixtComp
    MixtComp
    ${Boost_LIBRARIES}
    nlopt
)
