# Add AAModels files to AAModels_files variable
FILE( GLOB AAModels_files
  src/*.cpp
  include/*.h
  )

include_directories("..")

# Add AAModels files to STKAAModels library
add_library(STKAAModels OBJECT ${AAModels_files})


