context("Various tests")

test_that("validDim", {
  
  testFile <- "mat.csv"
  
  generateDummyData(testFile, 10, 10)
  
  expect_equal(nchar(validDim(testFile)), 0)
  expect_equal(nchar(validDim(testFile, 12, 12)), 0)
  expect_gt(nchar(validDim(testFile, 5, 12)), 0)
  expect_gt(nchar(validDim(testFile, 12, 5)), 0)
  expect_gt(nchar(validDim(testFile, 5, 5)), 0)

  file.remove("mat.csv")
})
