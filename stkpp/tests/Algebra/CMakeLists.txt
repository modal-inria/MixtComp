# This variable has names of the tests source files without extension
set( TESTS_FILES
    testLapackSymmEigen
    testSymmEigen
    testLapackQr
    testQr
    testQrShift
    testCG
    testSvd
)

include_directories("../../projects")

# loop over files
foreach(test ${TESTS_FILES})
  # Compile and link our test
  # Executable will be named after test source file
  add_executable(${test} ${test}.cpp)
  if(STKUSELAPACK)
    target_link_libraries(${test} ${LIBRARY_NAME} ${LAPACK_LIB})
  else(STKUSELAPACK)
    target_link_libraries(${test} ${LIBRARY_NAME})
  endif(STKUSELAPACK)
  # Add test
  add_test(NAME ${test} COMMAND ${test})
endforeach(test)

install(TARGETS ${TESTS_NOARGS} RUNTIME DESTINATION ${CTEST_BINARY_DIRECTORY})
