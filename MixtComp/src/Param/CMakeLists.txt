add_library(Param INTERFACE)
target_sources(Param INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/mixt_ConfIntParamStat.h)
target_include_directories(Param INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}"
)
