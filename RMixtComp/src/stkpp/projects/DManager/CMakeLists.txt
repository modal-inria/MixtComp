# Add DManager files to DManager_files variable
FILE( GLOB DManager_files
  src/*.cpp
  include/*.h
  )

# Add DManager files to STKDManager library
ADD_LIBRARY(STKDManager OBJECT ${DManager_files})


# TARGET_LINK_LIBRARIES(STKDManager STKpp)