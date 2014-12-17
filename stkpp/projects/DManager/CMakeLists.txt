# Add DManager files to DManager_files variable
FILE( GLOB DManager_files
  src/*.cpp
  include/*.h
  )

include_directories("..")

# Add DManager files to STKDManager library
add_library(STKDManager OBJECT ${DManager_files})
