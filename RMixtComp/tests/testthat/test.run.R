context("Run MixtComp")

test_that("run cluster/predict file csv",{
  pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
  pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RMixtComp")
  
  resGetData <- getData(c(pathToData, pathToDescriptor))
  expect_equal(resGetData$warnLog, "")
  
  # define the algorithm's parameters
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 50,
                     nbGibbsIter = 50,
                     ratioInitialization = 0.2)
  
  # run RMixtCompt for clustering
  res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  expect_equal(res$mixture$warnLog, NULL)
  
  # run RMixtCompt for predicting
  resPred <- mixtCompPredict(resGetData$lm, res$variable$param, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  expect_equal(resPred$mixture$warnLog, NULL)
  
  file.remove("progress")
})


test_that("run cluster/predict R object",{
  var <- list()
  var$z_class <- zParam()
  var$Poisson1 <- poissonParam("Poisson1")
  var$Gaussienne1 <- gaussianParam("Gaussian1")
  var$Categorical1 <- categoricalParam1("Categorical1")
  
  dat <- dataGenerator(100, 0.8, var) 
  
  resGetData <- getData(list(dat$data, dat$descriptor))
  expect_equal(resGetData$warnLog, "")
  
  # define the algorithm's parameters
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 50,
                     nbGibbsIter = 50,
                     ratioInitialization = 0.2)
  
  # run RMixtCompt for clustering
  res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  expect_equal(res$mixture$warnLog, NULL)
  
  confMat <- table(dat$z, res$variable$data$z_class$completed)
  print(confMat)
  
  
  datPred <- dataGenerator(100, 0.8, var)

  resGetDataPred <- getData(list(datPred$data, datPred$descriptor))
  expect_equal(resGetDataPred$warnLog, "")

  # run RMixtCompt for predicting
  resPred <- mixtCompPredict(resGetDataPred$lm, res$variable$param, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  expect_equal(resPred$mixture$warnLog, NULL)
  
  confMat <- table(datPred$z, resPred$variable$data$z_class$completed)
  print(confMat)

  file.remove("progress")
})
