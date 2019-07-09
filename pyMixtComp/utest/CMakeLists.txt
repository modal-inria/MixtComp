add_executable(runUtestPy
    UTestPyGraph.cpp
)

target_link_libraries(runUtestPy
    gtest_main
    pyMixtComp
    MixtComp
    ${Boost_LIBRARIES} 
    ${PYTHON_LIBRARIES}
)

