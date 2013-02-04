# Add Regress files to Regress_files variable
FILE( GLOB Regress_files
  src/*.cpp
  include/*.h
  )
# Add Regress files to STKRegress library
ADD_LIBRARY(STKRegress OBJECT ${Regress_files})


# TARGET_LINK_LIBRARIES(STKRegress STKpp)