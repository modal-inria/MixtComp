# long file list initially generated with "find . -type f -follow -print"
# with "./" prefix deleted


add_library(JMixtComp
    JSONGraph.cpp
    JSONGraph.h
    jsonIO.h
    translateJSONToCPP.h
    translateCPPToJSON.h
)

add_executable(jmc
    jmc.cpp
)

target_link_libraries(jmc
    MixtComp
    JMixtComp
    ${Boost_LIBRARIES}
)
