# Add Reduct files to Reduct_files variable
FILE( GLOB Reduct_files
  src/*.cpp
  include/*.h
  )

include_directories("..")

# Add Reduct files to STKReduct library
add_library(STKReduct OBJECT ${Reduct_files})
