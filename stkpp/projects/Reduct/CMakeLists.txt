# Add Reduct files to Reduct_files variable
FILE( GLOB Reduct_files
  src/*.cpp
  include/*.h
  )
# Add Reduct files to STKReduct library
ADD_LIBRARY(STKReduct OBJECT ${Reduct_files})


# TARGET_LINK_LIBRARIES(STKReduct STKpp)