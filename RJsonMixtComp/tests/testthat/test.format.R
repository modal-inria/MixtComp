context("Test format")


test_that("convert a matrix from json", {
  # named matrice
  x <- list(colNames = c("varCategorical", "varGaussian", "varPoisson"), 
            ctype ="Matrix",
            data = matrix(1:6, nrow = 2, ncol = 3),
            dtype = "Real",
            ncol = 3,
            nrow = 2,
            rowNames = c("k: 1", "k: 2"))
  
  expectedOut <- matrix(1:6, nrow = 2, ncol = 3, dimnames = list(x$rowNames, x$colNames))
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)
  
  
  # unnamed matrice
  x$colNames = list()
  x$rowNames = list()
  
  expectedOut <- matrix(1:6, nrow = 2, ncol = 3, dimnames = list(NULL, NULL))
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)
  
  
  # void matrice
  x$nrow = 0
  x$ncol = 0
  x$data = list()
  
  expectedOut <- matrix(nrow = 0, ncol = 0, dimnames = list(NULL, NULL))
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)
})

test_that("convert a vector from json", {
  # unnamed vector
  x <- list(ctype ="Vector",
            data = 1:6,
            dtype = "Real",
            nrow = 6,
            rowNames = list())
  
  expectedOut <- 1:6
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)  
  
  
  # named vector
  x$rowNames = letters[1:6]
  names(expectedOut) <- letters[1:6]
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)  
  
  
  # void case
  x$nrow = 0
  x$data = list()
  x$rowNames = list()
  
  expectedOut <- c()
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)  
})

