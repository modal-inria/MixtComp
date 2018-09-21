context("IO")

test_that("Nested list generation", {
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
  
  output <- rcppTest1()

  expect_identical(input, output)
})

test_that("Nested list reading", {
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

  output <- rcppTest2(input)

  expect_identical(TRUE, output)
})