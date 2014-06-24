# Add STKernel files to STKernel_files variable
FILE( GLOB STKernel_files
  src/*.cpp
  include/*.h
  )
  
INCLUDE_DIRECTORIES("..")

# Add STKernel files to STKSTKernel library
ADD_LIBRARY(STKSTKernel OBJECT ${STKernel_files})
