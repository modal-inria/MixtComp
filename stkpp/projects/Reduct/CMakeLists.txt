# Add Reduct files to Reduct_files variable
FILE( GLOB Reduct_files
  src/*.cpp
  include/*.h
  )

INCLUDE_DIRECTORIES("..")

# Add Reduct files to STKReduct library
ADD_LIBRARY(STKReduct OBJECT ${Reduct_files})
