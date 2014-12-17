# Add StatModels files to StatModels_files variable
FILE( GLOB StatModels_files
  src/*.cpp
  include/*.h
  )

include_directories("..")

# Add StatModels files to STKStatModels library
add_library(STKStatModels OBJECT ${StatModels_files})
