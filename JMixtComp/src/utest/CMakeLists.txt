# long file list initially generated with "find . -type f -follow -print"
# with "./" prefix deleted, and some utest deactivated

add_executable(runUtestJMC
    JSONGraph.cpp
)

target_link_libraries(runUtestJMC
    gtest_main
    JMixtComp
    MixtComp
)
