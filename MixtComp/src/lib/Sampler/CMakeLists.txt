set(Sampler_src
  mixt_CategoricalSampler.cpp mixt_CategoricalSampler.h
  mixt_ClassSampler.cpp mixt_ClassSampler.h
  mixt_GaussianSampler.cpp mixt_GaussianSampler.h
  mixt_PoissonSampler.cpp mixt_PoissonSampler.h
  WeibullSampler.cpp WeibullSampler.h
)

add_library(Sampler ${Sampler_src})
