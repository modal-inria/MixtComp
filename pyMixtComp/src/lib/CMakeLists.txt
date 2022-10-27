
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
  pyMixtCompLib
  MixtComp
  ${Boost_LIBRARIES}
  ${PYTHON_LIBRARIES})

# don't prepend wrapper library name with lib
set_target_properties(pyMixtCompBridge PROPERTIES PREFIX "" SUFFIX ".so")

