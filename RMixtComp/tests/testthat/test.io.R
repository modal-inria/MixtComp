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

test_that("get_payload", {
  output <- UTest3()
  
  expect_identical("riri", output)
})

# test_that("Nested list reading", {
#   input <- list(
#     a = list(
#       a1 = "riri",
#       a2 = "fifi",
#       a3 = "loulou"),
#     b = list(
#       b1 = list(
#         b11 = "dingo"
#       )
#     )
#   )
# 
#   output <- rcppTest2(input)
# 
#   expect_identical(TRUE, output)
# })