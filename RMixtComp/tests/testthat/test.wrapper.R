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


test_that("formatData converts data into a list format", {
  dat <- data.frame(x1 = 1:10, x2 = 10:1)
  dataOut <- convertData(dat)
  
  expect_equal(names(dataOut), colnames(dat))
  expect_equivalent(sapply(dataOut, length), rep(nrow(dat), ncol(dat)))
  expect_true(all(sapply(dataOut, is.character)))
  
  
  dat <- matrix(c(1:10, 10:1), ncol = 2, dimnames = list(NULL, c("x1", "x2")))
  dataOut <- convertData(dat)
  
  expect_equal(names(dataOut), colnames(dat))
  expect_equivalent(sapply(dataOut, length), rep(nrow(dat), ncol(dat)))
  expect_true(all(sapply(dataOut, is.character)))
})

