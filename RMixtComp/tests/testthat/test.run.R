context("Run MixtComp")

# rand index from fossil
rand.index <- function (group1, group2) 
{
  x <- abs(sapply(group1, function(x) x - group1))
  x[x > 1] <- 1
  y <- abs(sapply(group2, function(x) x - group2))
  y[y > 1] <- 1
  sg <- sum(abs(x - y))/2
  bc <- choose(dim(x)[1], 2)
  ri <- 1 - sg/bc
  return(ri)
}

test_that("gaussian model works",{
  set.seed(42)
  
  var <- list()
  var$z_class <- zParam()
  
  var$Gaussian1 <- gaussianParam("Gaussian1")
  
  dat <- dataGenerator(1000, 0.9, var)
  
  resGetData <- getData(list(dat$data, dat$descriptor))
  
  # define the algorithm's parameters
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 50,
                     nbGibbsIter = 50,
                     ratioInitialization = 0.2)
  
  # run RMixtCompt for clustering
  resLearn <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), dat$z), 0.9)
  
  confMatSampled <- table(dat$z, getZ_class(resLearn))
  print(confMatSampled)
  
  file.remove("progress")
})


test_that("poisson model works",{
  set.seed(42)
  
  var <- list()
  var$z_class <- zParam()
  
  var$Poisson0 <- poissonParamRandom("Poisson0")
  var$Poisson1 <- poissonParamRandom("Poisson1")
  var$Poisson2 <- poissonParamRandom("Poisson2")
  var$Poisson3 <- poissonParamRandom("Poisson3")
  var$Poisson4 <- poissonParamRandom("Poisson4")
  
  dat <- dataGenerator(1000, 0.9, var)
  
  resGetData <- getData(list(dat$data, dat$descriptor))
  
  # define the algorithm's parameters
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 50,
                     nbGibbsIter = 50,
                     ratioInitialization = 0.2)
  
  # run RMixtCompt for clustering
  resLearn <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), dat$z), 0.9)
  
  confMatSampled <- table(dat$z, getZ_class(resLearn))
  print(confMatSampled)
  
  file.remove("progress")
})




test_that("categorical model works",{
  set.seed(42)
  
  var <- list()
  var$z_class <- zParam()
  
  var$Categorical0 <- categoricalParamRandom("Categorical0")
  var$Categorical1 <- categoricalParamRandom("Categorical1")
  var$Categorical2 <- categoricalParamRandom("Categorical2")
  var$Categorical3 <- categoricalParamRandom("Categorical3")
  var$Categorical4 <- categoricalParamRandom("Categorical4")
  var$Categorical5 <- categoricalParamRandom("Categorical5")
  var$Categorical6 <- categoricalParamRandom("Categorical6")
  var$Categorical7 <- categoricalParamRandom("Categorical7")
  var$Categorical8 <- categoricalParamRandom("Categorical8")
  var$Categorical9 <- categoricalParamRandom("Categorical9")
  var$Categorical10 <- categoricalParamRandom("Categorical10")
  var$Categorical11 <- categoricalParamRandom("Categorical11")
  var$Categorical12 <- categoricalParamRandom("Categorical12")
  var$Categorical13 <- categoricalParamRandom("Categorical13")
  var$Categorical14 <- categoricalParamRandom("Categorical14")
  var$Categorical15 <- categoricalParamRandom("Categorical15")
  var$Categorical16 <- categoricalParamRandom("Categorical16")
  var$Categorical17 <- categoricalParamRandom("Categorical17")
  var$Categorical18 <- categoricalParamRandom("Categorical18")
  var$Categorical19 <- categoricalParamRandom("Categorical19")
  
  dat <- dataGenerator(1000, 0.9, var)
  
  resGetData <- getData(list(dat$data, dat$descriptor))
  
  # define the algorithm's parameters
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 50,
                     nbGibbsIter = 50,
                     ratioInitialization = 0.2)
  
  # run RMixtCompt for clustering
  resLearn <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), dat$z), 0.9)
  
  confMatSampled <- table(dat$z, getZ_class(resLearn))
  print(confMatSampled)
  
  file.remove("progress")
})


test_that("weibull model works",{
  set.seed(42)
  
  var <- list()
  var$z_class <- zParam()
  
  var$Weibull1 <- weibullParam("Weibull1")
  
  dat <- dataGenerator(1000, 0.95, var)
  
  resGetData <- getData(list(dat$data, dat$descriptor))
  
  # define the algorithm's parameters
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 50,
                     nbGibbsIter = 50,
                     ratioInitialization = 0.2)
  
  # run RMixtCompt for clustering
  resLearn <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), dat$z), 0.9)
  
  confMatSampled <- table(dat$z, getZ_class(resLearn))
  print(confMatSampled)
  
  file.remove("progress")
})

test_that("rank model works",{
  set.seed(42)
  
  var <- list()
  var$z_class <- zParam()
  
  var$Rank1 <- rankParam("Rank1")
  
  dat <- dataGenerator(1000, 0.95, var)
  
  resGetData <- getData(list(dat$data, dat$descriptor))
  
  # define the algorithm's parameters
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 50,
                     nbGibbsIter = 50,
                     ratioInitialization = 0.2)
  
  # run RMixtCompt for clustering
  resLearn <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), dat$z), 0.8)
  
  confMatSampled <- table(dat$z, getZ_class(resLearn))
  print(confMatSampled)
  
  file.remove("progress")
})


test_that("functional model works",{
  set.seed(42)
  
  var <- list()
  var$z_class <- zParam()
  
  var$Functional1 <- functionalInterPolyParam("Functional1")
  
  dat <- dataGenerator(200, 0.95, var)
  
  resGetData <- getData(list(dat$data, dat$descriptor))
  
  # define the algorithm's parameters
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 50,
                     nbGibbsIter = 50,
                     ratioInitialization = 0.2)
  
  # run RMixtCompt for clustering
  resLearn <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), dat$z), 0.9)
  
  confMatSampled <- table(dat$z, getZ_class(resLearn))
  print(confMatSampled)
  
  file.remove("progress")
})


test_that("functional model with shared alpha works",{
  set.seed(42)
  
  var <- list()
  var$z_class <- zParam()
  
  var$functionalSharedAlpha1 <- functionalSharedAlphaInterPolyParam("functionalSharedAlpha1") 
  
  dat <- dataGenerator(400, 0.9, var)
  
  resGetData <- getData(list(dat$data, dat$descriptor))
  
  # define the algorithm's parameters
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 50,
                     nbGibbsIter = 50,
                     ratioInitialization = 0.2)
  
  # run RMixtCompt for clustering
  resLearn <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), dat$z), 0.9)
  
  confMatSampled <- table(dat$z, getZ_class(resLearn))
  print(confMatSampled)
  
  file.remove("progress")
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
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 50,
                     nbGibbsIter = 50,
                     ratioInitialization = 0.2)
  
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
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 50,
                     nbGibbsIter = 50,
                     ratioInitialization = 0.2)
  
  # run RMixtCompt for clustering
  res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  expect_equal(res$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(res), dat$z_class), 0.9)
  
  # run RMixtCompt for predicting
  resPred <- mixtCompPredict(resGetData$lm, res$variable$param, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  expect_equal(resPred$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resPred), dat$z_class), 0.9)
  
  file.remove("progress")
})


test_that("run cluster/predict R object",{
  set.seed(42)
  
  var <- list()
  var$z_class <- zParam()
  var$Poisson1 <- poissonParam("Poisson1")
  var$Gaussienne1 <- gaussianParam("Gaussian1")
  var$Categorical1 <- categoricalParam1("Categorical1")
  
  dat <- dataGenerator(200, 0.9, var) 
  
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
  expect_gte(rand.index(getZ_class(res), dat$z), 0.9)
  
  
  datPred <- dataGenerator(100, 0.9, var)
  
  resGetDataPred <- getData(list(datPred$data, datPred$descriptor))
  expect_equal(resGetDataPred$warnLog, "")
  
  # run RMixtCompt for predicting
  resPred <- mixtCompPredict(resGetDataPred$lm, res$variable$param, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
  expect_equal(resPred$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resPred), datPred$z), 0.9)
  
  file.remove("progress")
})
