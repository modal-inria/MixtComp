# Add STatistiK files to STatistiK_files variable
FILE( GLOB Analysis_files
  src/*.cpp
  include/*.h
  )
# Add STatistiK files to STKSTatistiK library
add_library(STKAnalysis OBJECT ${Analysis_files})

include_directories("..")

# TARGET_LINK_LIBRARIES(STKAnalysis STKpp)