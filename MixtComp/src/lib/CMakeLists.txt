add_subdirectory(Composer)
add_subdirectory(Data)
add_subdirectory(IO)
add_subdirectory(Likelihood)
add_subdirectory(LinAlg)
add_subdirectory(Manager)
add_subdirectory(Mixture)
add_subdirectory(Param)
add_subdirectory(Sampler)
add_subdirectory(Statistic)
add_subdirectory(Strategy)
add_subdirectory(Various)

add_library(MixtComp INTERFACE)
target_sources(MixtComp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/mixt_MixtComp.h)
target_include_directories(MixtComp INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(MixtComp INTERFACE Composer
    Data
    IO
    Likelihood
    LinAlg
    Manager
    Mixture
    Param
    Sampler
    Statistic
    Strategy
    Various
)
