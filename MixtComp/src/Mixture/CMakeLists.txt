add_subdirectory(Functional)
add_subdirectory(Ordinal)
add_subdirectory(Rank)
add_subdirectory(Simple)

add_library(Mixture ${CMAKE_CURRENT_SOURCE_DIR}/mixt_IMixture.h)
target_link_libraries(Mixture Functional Ordinal Rank Simple)