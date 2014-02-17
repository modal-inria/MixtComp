# Add Algebra files to Algebra_files variable
FILE( GLOB Algebra_files
  src/*.cpp
  include/*.h
  )
# Add Algebra files to STKAlgebra library
ADD_LIBRARY(STKAlgebra OBJECT ${Algebra_files})


# TARGET_LINK_LIBRARIES(STKAlgebra STKpp)