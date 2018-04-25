add_library(LinAlgArma INTERFACE)

target_sources(LinAlgArma INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/mixt_MixtureManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mixt_Typedef.h
)

target_include_directories(LinAlgArma INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
