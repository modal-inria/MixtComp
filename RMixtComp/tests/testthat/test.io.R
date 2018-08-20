context("IO")

test_that("NumericVector",{
  input <- list(
    a = list(
      a1 = "riri",
      a2 = "fifi",
      a3 = "loulou"),
    b = list(
      b1 = list(
        b11 = "dingo"
      )
    )
  )
  
  output <- SGraphTest(input)

  expect_identical(input, output)
})