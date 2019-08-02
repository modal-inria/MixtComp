
add_executable(runUtestJMC
    JSONGraph.cpp
)

target_link_libraries(runUtestJMC
    gtest_main
    JMixtComp
    MixtComp
)
