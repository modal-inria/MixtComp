# Add Arrays files to Arrays_files variable
FILE( GLOB Arrays_files
  src/*.cpp
  include/*.h
  )
  
include_directories("..")

# Add Arrays files to STKArrays library
add_library(STKArrays OBJECT ${Arrays_files})
