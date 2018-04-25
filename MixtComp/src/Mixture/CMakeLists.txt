add_subdirectory(Functional)
add_subdirectory(Ordinal)
add_subdirectory(Rank)
add_subdirectory(Simple)

add_library(Mixture INTERFACE)
target_sources(Mixture INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/mixt_IMixture.h)
target_include_directories(Mixture INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}" 
)

target_link_libraries(Mixture INTERFACE Functional 
										Ordinal 
										Rank 
										Simple)
