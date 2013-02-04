# Add STKernel files to STKernel_files variable
FILE( GLOB STKernel_files
  src/*.cpp
  include/*.h
  )
# Add STKernel files to STKSTKernel library
ADD_LIBRARY(STKSTKernel OBJECT ${STKernel_files})


# TARGET_LINK_LIBRARIES(STKSTKernel STKpp)