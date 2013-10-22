FILE( GLOB Framework
  *.cpp
  *.h
  )

# Add AAModels files to STKAAModels library
ADD_LIBRARY(Framework OBJECT ${Framework})
  