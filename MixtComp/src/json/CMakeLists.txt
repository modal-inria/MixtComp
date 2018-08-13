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

add_executable(execJSON
    execJSON.cpp
)

target_link_libraries(execJSON
    MixtComp
    JMixtComp
    ${Boost_LIBRARIES}
    nlopt)