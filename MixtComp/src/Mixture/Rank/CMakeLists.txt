set(Rank_src
  mixt_RankClass.cpp mixt_RankClass.h
  mixt_RankIndividual.cpp mixt_RankIndividual.h
  mixt_RankLikelihood.cpp mixt_RankLikelihood.h
  mixt_RankMixture.h
  mixt_RankParser.cpp mixt_RankParser.h
  mixt_RankStat.cpp mixt_RankStat.h
  mixt_RankVal.cpp mixt_RankVal.h
)

add_library(Rank ${Rank_src})
