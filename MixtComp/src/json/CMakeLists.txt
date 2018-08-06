# long file list initially generated with "find . -type f -follow -print"
# with "./" prefix deleted

add_executable(JMixtComp
    learn.cpp
    SGraphToJSON.cpp
    SGraphToJSON.h
    SGraphVisJSON.cpp
    SGraphVisJSON.h
)

target_link_libraries(JMixtComp
    MixtComp
    ${Boost_LIBRARIES}
    nlopt
)
