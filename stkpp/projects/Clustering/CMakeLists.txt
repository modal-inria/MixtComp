# Add Clustering files to Clustering_files variable
FILE( GLOB Clustering_files
  src/*.cpp
  include/*.h
  )

include_directories("..")

# Add Clustering files to STKClustering library
add_library(STKClustering OBJECT ${Clustering_files})

