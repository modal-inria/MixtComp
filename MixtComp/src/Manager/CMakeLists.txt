add_library(Manager INTERFACE)
target_sources(Manager INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/mixt_MixtureManager.h)
target_include_directories(Manager INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}"
)
