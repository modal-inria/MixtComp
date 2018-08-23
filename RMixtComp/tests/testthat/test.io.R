context("IO")

test_that("CharacterVector",{
  input <- list(
    a = list(
      a1 = "riri",
      a2 = "fifi",
      a3 = "loulou"),
    b = list(
      b1 = list(
        b11 = c("mickey", "dingo")
      )
    )
  )
  
  output <- rcppTest(input)

  expect_identical(input, output)
})

test_that("Integer",{
  input <- list(
    a = 12)
  
  output <- rcppTest(input)
  
  expect_identical(input, output)
})