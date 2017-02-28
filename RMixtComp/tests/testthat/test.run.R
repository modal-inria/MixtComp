context("Run MixtComp")

test_that("run cluster/predict file csv",{
  pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
  pathToDescriptor <- system.file("extdata", "descriptor.csv", package = "RMixtComp")
  
  resGetData <- getData(c(pathToData, pathToDescriptor))
  expect_equal(resGetData$warnLog, "")
  
  # define the algorithm's parameters
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 50,
                     nbGibbsIter = 50,
                     parameterEdgeAuthorized = FALSE)
  
  # run RMixtCompt for clustering
  res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  expect_equal(res$mixture$warnLog, "")
  
  # run RMixtCompt for predicting
  resPred <- mixtCompPredict(resGetData$lm, res$variable$param, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  expect_equal(res$mixture$warnLog, "")
})


test_that("run cluster/predict R object",{
  pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
  pathToDescriptor <- system.file("extdata", "descriptor.csv", package = "RMixtComp")
  
  data <- read.table(pathToData, header = TRUE, stringsAsFactors = FALSE, sep = ";")
  descriptor <- read.table(pathToDescriptor, header = TRUE, stringsAsFactors = FALSE, sep = ";")
  
  resGetData <- getData(list(data, descriptor))
  expect_equal(resGetData$warnLog, "")
  
  # define the algorithm's parameters
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 50,
                     nbGibbsIter = 50,
                     parameterEdgeAuthorized = FALSE)
  
  # run RMixtCompt for clustering
  res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  expect_equal(res$mixture$warnLog, "")
  
  # run RMixtCompt for predicting
  resPred <- mixtCompPredict(resGetData$lm, res$variable$param, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  expect_equal(res$mixture$warnLog, "")
  
})
