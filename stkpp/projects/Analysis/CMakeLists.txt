# Add STatistiK files to STatistiK_files variable
FILE( GLOB Analysis_files
  src/*.cpp
  include/*.h
  )
# Add STatistiK files to STKSTatistiK library
ADD_LIBRARY(STKAnalysis OBJECT ${Analysis_files})

INCLUDE_DIRECTORIES("..")

# TARGET_LINK_LIBRARIES(STKAnalysis STKpp)