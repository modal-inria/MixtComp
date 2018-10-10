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


test_that("formatData converts data.frame into a list format", {
  dat <- data.frame(x1 = 1:10, x2 = 10:1)
  dataOut <- formatData(dat)
  
  expect_equal(names(dataOut), colnames(dat))
  expect_equivalent(sapply(dataOut, length), rep(nrow(dat), ncol(dat)))
  expect_true(all(sapply(dataOut, is.character)))
})

test_that("formatData converts matrix into a list format", {
  dat <- matrix(c(1:10, 10:1), ncol = 2, dimnames = list(NULL, c("x1", "x2")))
  dataOut <- formatData(dat)
  
  expect_equal(names(dataOut), colnames(dat))
  expect_equivalent(sapply(dataOut, length), rep(nrow(dat), ncol(dat)))
  expect_true(all(sapply(dataOut, is.character)))
})

test_that("formatData keeps list in list format", { 
  dat <- list(x1 = 1:10, x2 = 10:1)
  dataOut <- formatData(dat)
  
  expect_equal(dat, dataOut)
})

test_that("checkNClass works with mixtCompCluster object", {
  resLearn <- list(algo = list(nClass = 2))
  class(resLearn) = "MixtCompCluster"
  
  nClass <- NULL
  expect_silent(out <- checkNClass(nClass, resLearn))
  expect_equal(out, 2)
  
  nClass <- 3
  expect_warning(out <- checkNClass(nClass, resLearn))
  expect_equal(out, 2)
  
  nClass <- 2:4
  expect_warning(out <- checkNClass(nClass, resLearn))
  expect_equal(out, 2)
  
  nClass <- 3:4
  expect_warning(out <- checkNClass(nClass, resLearn))
  expect_equal(out, 2)
})

test_that("checkNClass works with mixtCompLearn object", {
  resLearn <- list(algo = list(nClass = 2), nClass = 2:5)
  class(resLearn) = c("MixtCompLearn", "MixtCompCluster")
  
  nClass <- NULL
  expect_silent(out <- checkNClass(nClass, resLearn))
  expect_equal(out, 2)
  
  nClass <- 3
  expect_silent(out <- checkNClass(nClass, resLearn))
  expect_equal(out, 3)
  
  nClass <- 3:4
  expect_warning(out <- checkNClass(nClass, resLearn))
  expect_equal(out, 3)
  
  nClass <- 6:8
  expect_warning(out <- checkNClass(nClass, resLearn))
  expect_equal(out, 2)
})
