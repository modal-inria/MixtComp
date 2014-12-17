# This variable has names of the tests source files without extension
set( TESTS_FILES
    testAllGammaMixtureModels
    testCategoricalMixtureModels
    testAllGaussianMixtureModels
)

FILE( GLOB Util_Files
  StrategiesUtil.cpp
  SimulUtil.cpp
 )
 
include_directories("../../projects")
 
add_library(TestUtil OBJECT ${Util_Files})
 
 # create stktest library
add_library(${LIBRARY_TEST}
$<TARGET_OBJECTS:TestUtil>)
 
SET_TARGET_PROPERTIES(${LIBRARY_TEST} PROPERTIES LINKER_LANGUAGE CXX)
install(TARGETS ${LIBRARY_TEST} ARCHIVE DESTINATION ${LIBRARY_OUTPUT_PATH})
 
 
 
# loop over files
foreach(test ${TESTS_FILES})
  # Compile and link our test
  # Executable will be named after test source file
  add_executable(${test} ${test}.cpp)
  TARGET_LINK_LIBRARIES(${test} ${LIBRARY_TEST} ${LIBRARY_NAME})
  # Add test
  add_test(NAME ${test} COMMAND ${test})
endforeach(test)

install(TARGETS ${TESTS_NOARGS} RUNTIME DESTINATION ${CTEST_BINARY_DIRECTORY})


#ADD_CUSTOM_TARGET(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${TESTS_NOARGS})