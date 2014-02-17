# Add AAModels files to AAModels_files variable
FILE( GLOB AAModels_files
  src/*.cpp
  include/*.h
  )

# Add AAModels files to STKAAModels library
ADD_LIBRARY(STKAAModels OBJECT ${AAModels_files})


# TARGET_LINK_LIBRARIES(STKAAModels STKpp)