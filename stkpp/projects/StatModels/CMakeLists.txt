# Add StatModels files to StatModels_files variable
FILE( GLOB StatModels_files
  src/*.cpp
  include/*.h
  )
# Add StatModels files to STKStatModels library
ADD_LIBRARY(STKStatModels OBJECT ${StatModels_files})


# TARGET_LINK_LIBRARIES(STKStatModels STKpp)