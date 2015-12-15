uTest <- function() {
  stopifnot(requireNamespace("RUnit"))
  
  test.suite <- defineTestSuite('test',
                               dirs = file.path('R/test'))
  test.result <- runTestSuite(test.suite)
  printTextProtocol(test.result)
}
