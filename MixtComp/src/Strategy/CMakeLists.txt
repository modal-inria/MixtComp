set(Strategy_src
  mixt_GibbsStrategy.cpp mixt_GibbsStrategy.h
  mixt_SEMStrategy.cpp mixt_SEMStrategy.h
  mixt_StrategyParam.h
)

add_library(Strategy ${Strategy_src})
