# long file list initially generated with "find . -type f -follow -print"
# with "./" prefix deleted

add_executable(JMixtComp
    learn.cpp
    GraphToJson.cpp
    GraphToJson.h
)

target_link_libraries(JMixtComp
    MixtComp
    ${Boost_LIBRARIES}
    nlopt
)
