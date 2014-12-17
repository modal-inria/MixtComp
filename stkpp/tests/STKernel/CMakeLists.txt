# This variable has names of the tests source files without extension
set( TESTS_FILES
    testBase
    testFunctors
)

include_directories("../../projects")

# loop over files
foreach(test ${TESTS_FILES})
  # Compile and link our test
  # Executable will be named after test source file
  add_executable(${test} ${test}.cpp)
  TARGET_LINK_LIBRARIES(${test} ${LIBRARY_NAME})
  
  # Add test
  add_test(NAME ${test} COMMAND ${test})
endforeach(test)

install(TARGETS ${TESTS_NOARGS} RUNTIME DESTINATION ${CTEST_BINARY_DIRECTORY})
#ADD_CUSTOM_TARGET(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${TESTS_NOARGS})