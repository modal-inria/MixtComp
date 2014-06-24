# Add StatModels files to StatModels_files variable
FILE( GLOB StatModels_files
  src/*.cpp
  include/*.h
  )

INCLUDE_DIRECTORIES("..")

# Add StatModels files to STKStatModels library
ADD_LIBRARY(STKStatModels OBJECT ${StatModels_files})
