# Add Arrays files to Arrays_files variable
FILE( GLOB Arrays_files
  src/*.cpp
  include/*.h
  )
  
# Add Arrays files to STKArrays library
ADD_LIBRARY(STKArrays OBJECT ${Arrays_files})


# TARGET_LINK_LIBRARIES(STKArrays STKpp)