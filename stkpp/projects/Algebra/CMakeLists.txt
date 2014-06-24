# Add Algebra files to Algebra_files variable
FILE( GLOB Algebra_files
  src/*.cpp
  include/*.h
  )

INCLUDE_DIRECTORIES("..")

# Add Algebra files to STKAlgebra library
ADD_LIBRARY(STKAlgebra OBJECT ${Algebra_files})

