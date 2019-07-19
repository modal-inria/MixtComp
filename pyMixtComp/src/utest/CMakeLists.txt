add_executable(runUtestPy
    UTestPyGraph.cpp
)

target_link_libraries(runUtestPy
    gtest_main
    pyMixtCompLib
    MixtComp
    ${Boost_LIBRARIES} 
    ${PYTHON_LIBRARIES}
)

