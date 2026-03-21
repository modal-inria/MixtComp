# google test project is built by MixtComp

add_executable(runUtestPy
    UTestPyGraph.cpp
)

target_link_libraries(runUtestPy
    PRIVATE
    gtest_main
    pyMixtCompLib
    MixtComp
    Boost::python
    Boost::numpy
    Python3::Python
)
