FILE( GLOB Mixtures
  src/*.cpp
  include/*.h
  )

  # Add AAModels files to STKAAModels library
ADD_LIBRARY(Mixtures OBJECT ${Mixtures})
  