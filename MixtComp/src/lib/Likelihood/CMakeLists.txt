set(Likelihood_src
  mixt_CategoricalLikelihood.h
  mixt_GaussianLikelihood.cpp mixt_GaussianLikelihood.h
  mixt_PoissonLikelihood.cpp mixt_PoissonLikelihood.h
  WeibullLikelihood.cpp WeibullLikelihood.h
)

add_library(Likelihood ${Likelihood_src})
