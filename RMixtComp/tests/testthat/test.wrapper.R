context("Wrappers of rmc function")

test_that("formatDesc does not change well formated data", {
  desc <- list(var1 = list(type = "Gaussian", paramStr = ""),
               var2 = list(type = "CorReg", paramStr = "ouais"),
               var3 = list(type = "Multinomial", paramStr = "CorReg"))
  
  outDesc <- formatDesc(desc)
  expect_equal(outDesc, desc)
})


test_that("formatDesc adds paramStr when missing", {
  desc <- list(var1 = list(type = "Gaussian"),
               var2 = list(type = "CorReg", paramStr = "ouais"),
               var3 = list(type = "Multinomial"))
  
  outDesc <- formatDesc(desc)
  expect_equal(outDesc$var2, desc$var2)
  expect_equal(outDesc$var1, list(type = "Gaussian", paramStr = ""))
  expect_equal(outDesc$var3, list(type = "Multinomial", paramStr = ""))
  
})


test_that("formatDesc puts type in a list format", {
  desc <- list(var1 = "Gaussian",
               var2 = list(type = "CorReg", paramStr = "ouais"),
               var3 = "Multinomial")
  
  outDesc <- formatDesc(desc)
  expect_equal(outDesc$var1, list(type = "Gaussian", paramStr = ""))
  expect_equal(outDesc$var2, desc$var2)
  expect_equal(outDesc$var3, list(type = "Multinomial", paramStr = ""))
  
})
