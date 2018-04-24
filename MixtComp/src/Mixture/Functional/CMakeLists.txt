set(Functional_src
  mixt_Function.cpp mixt_Function.h
  mixt_FunctionalClass.cpp mixt_FunctionalClass.h
  mixt_FunctionalComputation.cpp mixt_FunctionalComputation.h
  mixt_FunctionalMixture.h
  mixt_FunctionalParser.cpp mixt_FunctionalParser.h
  mixt_FunctionalSharedAlphaMixture.h
)

add_library(Functional ${Functional_src})
