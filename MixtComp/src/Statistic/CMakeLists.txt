set(Statistic_src	
  Functions.h 
  mixt_ExponentialStatistic.cpp mixt_ExponentialStatistic.h
  mixt_MultinomialStatistic.cpp mixt_MultinomialStatistic.h
  mixt_NormalStatistic.cpp mixt_NormalStatistic.h
  mixt_PoissonStatistic.cpp mixt_PoissonStatistic.h
  mixt_RNG.cpp mixt_RNG.h
  mixt_Statistic.h
  mixt_UniformStatistic.cpp mixt_UniformStatistic.h
  WeibullStatistic.cpp WeibullStatistic.h  
)

add_library(Statistic ${Statistic_src})
