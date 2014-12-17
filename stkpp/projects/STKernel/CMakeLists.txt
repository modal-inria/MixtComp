# Add STKernel files to STKernel_files variable
FILE( GLOB STKernel_files
  src/*.cpp
  include/*.h
  )
  
include_directories("..")

# Add STKernel files to STKSTKernel library
add_library(STKSTKernel OBJECT ${STKernel_files})
