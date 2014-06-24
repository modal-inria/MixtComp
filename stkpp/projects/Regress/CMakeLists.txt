# Add Regress files to Regress_files variable
FILE( GLOB Regress_files
  src/*.cpp
  include/*.h
  )

INCLUDE_DIRECTORIES("..")

# Add Regress files to STKRegress library
ADD_LIBRARY(STKRegress OBJECT ${Regress_files})
