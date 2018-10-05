context("Run MixtComp")

Sys.setenv(MC_DETERMINISTIC = 42)

test_that("rank model works",{
  set.seed(42)

  var <- list()
  var$z_class <- zParam()

  var$Rank1 <- rankParam("Rank1")

  dat <- dataGenerator(1000, 0.95, var)

  resGetData <- getData(list(dat$data, dat$descriptor))

  # define the algorithm's parameters
  mcStrategy <- createMcStrategy(nInitPerClass = 100)

  # run RMixtCompt for clustering
  resLearn <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)

  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), dat$z), 0.8)

  confMatSampled <- table(dat$z, getZ_class(resLearn))
  print(confMatSampled)
})

test_that("ordinal model works",{
  skip("ordinal model is not working")
  set.seed(42)

  var <- list()
  var$z_class <- zParam()

  var$Ordinal1 <- ordinalParamRandom("Ordinal1") # validated
  var$Ordinal2 <- ordinalParamRandom("Ordinal2")
  var$Ordinal3 <- ordinalParamRandom("Ordinal3")

  dat <- dataGenerator(1000, 0.9, var)

  resGetData <- getData(list(dat$data, dat$descriptor))

  # define the algorithm's parameters
  mcStrategy <- createMcStrategy(nInitPerClass = 100)

  # run RMixtCompt for clustering
  resLearn <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)

  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), dat$z), 0.8)

  confMatSampled <- table(dat$z, getZ_class(resLearn))
  print(confMatSampled)

  file.remove("progress")
})



test_that("run cluster/predict file csv",{
  pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
  pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RMixtComp")

  dat <- read.table(pathToData, sep = ";", header = TRUE)

  resGetData <- getData(c(pathToData, pathToDescriptor))
  expect_equal(resGetData$warnLog, "")

  # define the algorithm's parameters
  mcStrategy <- createMcStrategy(nInitPerClass = 20)

  # run RMixtCompt for clustering
  res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  expect_equal(res$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(res), dat$z_class), 0.9)

  # run RMixtCompt for predicting
  resPred <- mixtCompPredict(resGetData$lm, res$variable$param, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  expect_equal(resPred$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resPred), dat$z_class), 0.9)
})

Sys.unsetenv("MC_DETERMINISTIC")
