
add_library(pyMixtCompLib
  pmc.cpp
  pmc.h
  PyGraph.h
  PyGraph.cpp
  pythonIO.h
  translateCPPToPython.h
  translatePythonToCPP.h)


# Define the wrapper library that wraps our library
add_library(pyMixtCompBridge SHARED pyMixtComp.cpp)

target_link_libraries(pyMixtCompBridge
  PRIVATE
  pyMixtCompLib
  MixtComp
  Boost::python
  Boost::numpy
  Python3::Python
)

# don't prepend wrapper library name with lib
set_target_properties(pyMixtCompBridge PROPERTIES PREFIX "" SUFFIX ".so")
