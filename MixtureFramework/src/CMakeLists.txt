FILE( GLOB src_files initialization/*.cpp models/*.cpp algorithms/*.cpp
strategy/*.cpp facade/*.cpp)
add_library(emframework ${src_files})
