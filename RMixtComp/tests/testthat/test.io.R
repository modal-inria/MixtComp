context("IO")

test_that("add_payload", {
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
  
  output <- UTest1()

  expect_identical(input, output)
})

test_that("exist_payload", {
  output <- UTest2()
  
  expect_identical(TRUE, output)
})

test_that("exist_payload, negative case", {
  output <- UTest6()
  
  expect_identical(FALSE, output)
})

test_that("get_payload", {
  output <- UTest3()
  
  expect_identical("riri", output)
})

test_that("NumericVector IO", {
  l <- list()
  l$This$is$Sparta <- c(a = 12, b = 12, c = 42.5)
  
  output <- UTest4(l)
  
  expect_identical(l, output)
})

test_that("NumericMatrix IO", {
  l <- list()
  l$mat <- matrix(c(1, 2, 3, 4, 5, 6), nrow = 2)
  
  output <- UTest5(l)
  
  expect_identical(l, output)
})
