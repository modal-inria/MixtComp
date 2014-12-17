# Add Algebra files to Algebra_files variable
FILE( GLOB Algebra_files
  src/*.cpp
  include/*.h
  )

include_directories("..")

# Add Algebra files to STKAlgebra library
add_library(STKAlgebra OBJECT ${Algebra_files})

