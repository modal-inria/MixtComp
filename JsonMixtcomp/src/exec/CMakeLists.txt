# long file list initially generated with "find . -type f -follow -print"
# with "./" prefix deleted, and some utest deactivated

add_executable(JsonMixtComp
    main.cpp
    mixt_ParamSetterJson.h
    mixt_DataExtractorJson.cpp
    mixt_DataHandlerJson.h
    CMakeLists.txt
    mixt_ParamExtractorJson.h
    mixt_Function_Json.h
    mixt_ParamExtractorJson.cpp
    mixt_Function_Json.cpp
    mixt_DataExtractorJson.h
    mixt_ParamSetterJson.cpp
    mixt_DataHandlerJson.cpp
)

target_link_libraries(JsonMixtComp
    MixtComp
    ${Boost_LIBRARIES}
    nlopt
)
