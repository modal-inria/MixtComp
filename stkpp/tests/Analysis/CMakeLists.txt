# This variable has names of the tests source files without extension
SET( TESTS_FILES
    testAnalysis
)

# loop over files
FOREACH(test ${TESTS_FILES})
  # Compile and link our test
  # Executable will be named after test source file
  ADD_EXECUTABLE(${test} ${test}.cpp)
  TARGET_LINK_LIBRARIES(${test} ${LIBRARY_NAME})
  # Add test
  ADD_TEST(NAME ${test} COMMAND ${test})
ENDFOREACH(test)

INSTALL(TARGETS ${TESTS_NOARGS} RUNTIME DESTINATION ${CTEST_BINARY_DIRECTORY})
