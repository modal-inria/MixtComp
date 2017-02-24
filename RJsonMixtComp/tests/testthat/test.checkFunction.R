context("Test the check functions")

test_that("checkPath", {
  expect_error(checkPath("aaaaaaa", "C:/aaaaaaa"))
  
  expect_error(checkPath("aaaaaaa", "~/aaaaaaa"))
  
  expect_equal(checkPath("aaaaaaa", "bbbbb"), TRUE)
})


test_that("checkModel", {
  expect_error(checkModel("aaaaaaa", "Rank"))
  
  expect_error(checkModel("aaaaaaa", "Functional"))
  
  expect_equal(checkModel(c("aaaaaaa", "bbbbb")), TRUE)
})


