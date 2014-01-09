# Add STatistiK files to STatistiK_files variable
FILE( GLOB STatistiK_files
  src/*.cpp
  include/*.h
  )
# Add STatistiK files to STKSTatistiK library
ADD_LIBRARY(STKSTatistiK OBJECT ${STatistiK_files})


# TARGET_LINK_LIBRARIES(STKSTatistiK STKpp)