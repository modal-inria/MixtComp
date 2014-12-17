# Add Regress files to Regress_files variable
FILE( GLOB Regress_files
  src/*.cpp
  include/*.h
  )

include_directories("..")

# Add Regress files to STKRegress library
add_library(STKRegress OBJECT ${Regress_files})
