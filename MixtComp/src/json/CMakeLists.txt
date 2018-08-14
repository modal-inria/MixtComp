# long file list initially generated with "find . -type f -follow -print"
# with "./" prefix deleted

add_library(JMixtComp
    jsonIO.h
    JSONToSGraph.cpp
    JSONToSGraph.h
    SGraphToJSON.cpp
    SGraphToJSON.h
    SGraphVisJSON.cpp
    SGraphVisJSON.h
)

add_executable(jmc
    jmc.cpp
)

target_link_libraries(jmc
    MixtComp
    JMixtComp
    ${Boost_LIBRARIES}
    nlopt)
