# Add Clustering files to Clustering_files variable
FILE( GLOB Clustering_files
  src/*.cpp
  include/*.h
  )

INCLUDE_DIRECTORIES("..")

# Add Clustering files to STKClustering library
ADD_LIBRARY(STKClustering OBJECT ${Clustering_files})

