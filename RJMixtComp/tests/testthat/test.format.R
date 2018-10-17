context("Test format")


test_that("convert a matrix from json", {
  # named matrice
  x <- list(colNames = c("varCategorical", "varGaussian", "varPoisson"), 
            ctype ="Matrix",
            data = matrix(1:6, nrow = 2, ncol = 3),
            dtype = "Real",
            ncol = 3,
            nrow = 2,
            rowNames = c("k: 1", "k: 2"))
  
  expectedOut <- matrix(1:6, nrow = 2, ncol = 3, dimnames = list(x$rowNames, x$colNames))
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)
  
  
  # unnamed matrice
  x$colNames = list()
  x$rowNames = list()
  
  expectedOut <- matrix(1:6, nrow = 2, ncol = 3, dimnames = list(NULL, NULL))
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)
  
  
  # void matrice
  x$nrow = 0
  x$ncol = 0
  x$data = list()
  
  expectedOut <- matrix(nrow = 0, ncol = 0, dimnames = list(NULL, NULL))
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)
})

test_that("convert a vector from json", {
  # unnamed vector
  x <- list(ctype ="Vector",
            data = 1:6,
            dtype = "Real",
            nrow = 6,
            rowNames = list())
  
  expectedOut <- 1:6
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)  
  
  
  # named vector
  x$rowNames = letters[1:6]
  names(expectedOut) <- letters[1:6]
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)  
  
  
  # void case
  x$nrow = 0
  x$data = list()
  x$rowNames = list()
  
  expectedOut <- c()
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)  
})



test_that("convertOutput converts well", {
  pathToData <- system.file("extdata", "data.json", package = "RJMixtComp")
  pathToDescriptor <- system.file("extdata", "desc.json", package = "RJMixtComp")
  
  data <- as.data.frame(fromJSON(pathToData))
  descriptor <- fromJSON(pathToDescriptor)
  algo <- list(nbBurnInIter = 50,
               nbIter = 50,
               nbGibbsBurnInIter = 20,
               nbGibbsIter = 20,
               nInitPerClass = 10,
               nSemTry = 5,
               confidenceLevel = 0.95)
  
  expect_silent(res <- JsonMixtCompLearn(data, descriptor, algo, nClass = 2, inputPath = ".", outputFile = "reslearn.json"))
  
  res <- fromJSON("reslearn.json")
  out <- convertOutput(res)
  
  # warnLog
  expect_true(is.null(res$warnLog))
  
  # mixture 
  expect_equal(out$mixture$BIC, res$mixture$BIC)
  expect_equal(out$mixture$ICL, res$mixture$ICL)
  expect_equal(out$mixture$runTime, res$mixture$runTime)
  expect_equal(out$mixture$lnCompletedLikelihood, res$mixture$lnCompletedLikelihood)
  expect_equal(out$mixture$lnObservedLikelihood, res$mixture$lnObservedLikelihood)
  expect_equal(out$mixture$nbFreeParameters, res$mixture$nbFreeParameters)
  expect_equal(class(out$mixture$IDClass), "matrix")
  expect_equal(rownames(out$mixture$IDClass), res$mixture$IDClass$rowNames)
  expect_equal(colnames(out$mixture$IDClass), res$mixture$IDClass$colNames)
  expect_equal(class(out$mixture$delta), "matrix")
  expect_equal(class(out$mixture$lnProbaGivenClass), "matrix")
  expect_length(out$mixture$completedProbabilityLogBurnIn, res$mixture$completedProbabilityLogBurnIn$nrow)
  expect_length(out$mixture$completedProbabilityLogRun, res$mixture$completedProbabilityLogRun$nrow)
  
  # variable$type
  expect_equal(out$variable$type, res$variable$type)
  
  # variable$param
  expect_equal(class(out$variable$param$z_class$stat), "matrix")
  expect_equal(rownames(out$variable$param$z_class$stat), res$variable$param$z_class$stat$rowNames)
  expect_equal(colnames(out$variable$param$z_class$stat), res$variable$param$z_class$stat$colNames)
  expect_equal(out$variable$param$z_class$paramStr, res$variable$param$z_class$paramStr)
  expect_equal(class(out$variable$param$varGaussian$stat), "matrix")
  expect_equal(rownames(out$variable$param$varGaussian$stat), res$variable$param$varGaussian$stat$rowNames)
  expect_equal(colnames(out$variable$param$varGaussian$stat), res$variable$param$varGaussian$stat$colNames)
  expect_equal(out$variable$param$varGaussian$paramStr, res$variable$param$varGaussian$paramStr)
  
  # variable$data
  expect_equivalent(out$variable$data$z_class$stat, res$variable$data$z_class$stat$data)
  expect_equal(out$variable$data$z_class$completed, res$variable$data$z_class$completed$data)
  expect_equal(out$variable$data$varGaussian$completed, res$variable$data$varGaussian$completed$data)
  
  file.remove("./algo.json", "./descriptor.json", "./data.json", "reslearn.json", "progress")
})
