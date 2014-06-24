# Add Arrays files to Arrays_files variable
FILE( GLOB Arrays_files
  src/*.cpp
  include/*.h
  )
  
INCLUDE_DIRECTORIES("..")

# Add Arrays files to STKArrays library
ADD_LIBRARY(STKArrays OBJECT ${Arrays_files})
