set(Simple_src
  Categorical.cpp Categorical.h
  Gaussian.cpp Gaussian.h
  Poisson.cpp Poisson.h
  SimpleMixture.h
  Weibull.cpp Weibull.h
)

add_library(Simple ${Simple_src})
