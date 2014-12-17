# Add STatistiK files to STatistiK_files variable
FILE( GLOB STatistiK_files
  src/*.cpp
  include/*.h
  )
  
include_directories("..")

# Add STatistiK files to STKSTatistiK library
add_library(STKSTatistiK OBJECT ${STatistiK_files})
