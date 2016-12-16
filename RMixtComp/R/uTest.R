library(RUnit)

uTest <- function() {
  test.suite <- defineTestSuite('test',
                               dirs = file.path('R/test'))
  test.result <- runTestSuite(test.suite)
  printTextProtocol(test.result)
}
