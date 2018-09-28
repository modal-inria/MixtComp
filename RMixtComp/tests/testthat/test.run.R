context("Run MixtComp")

Sys.setenv(MC_DETERMINISTIC = 42)

test_that("gaussian model works",{
  set.seed(42)
  
  nInd <- 1000
  
  var <- list()
  var$z_class <- zParam()
  
  var$Gaussian1 <- gaussianParam("Gaussian1")
  
  resGen <- dataGeneratorNewIO(nInd, 0.9, var)
  
  algo <- list(
    nClass = 2,
    nInd = nInd,
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 100,
    nbGibbsIter = 100,
    nInitPerClass = 100,
    nSemTry = 20,
    confidenceLevel = 0.95,
    mode = "learn"
  )
  
  data <- resGen$data
  desc <- resGen$desc
  
  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), resGen$z), 0.9)
  
  confMatSampled <- table(resGen$z, getZ_class(resLearn))
  print(confMatSampled)
})

test_that("poisson model works",{
  set.seed(42)
  
  nInd <- 1000
  
  var <- list()
  var$z_class <- zParam()
  
  var$Poisson0 <- poissonParamRandom("Poisson0")
  var$Poisson1 <- poissonParamRandom("Poisson1")
  var$Poisson2 <- poissonParamRandom("Poisson2")
  var$Poisson3 <- poissonParamRandom("Poisson3")
  var$Poisson4 <- poissonParamRandom("Poisson4")
  
  resGen <- dataGeneratorNewIO(nInd, 0.9, var)
  
  algo <- list(
    nClass = 2,
    nInd = nInd,
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 100,
    nbGibbsIter = 100,
    nInitPerClass = 100,
    nSemTry = 20,
    confidenceLevel = 0.95,
    mode = "learn"
  )
  
  data <- resGen$data
  desc <- resGen$desc
  
  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), resGen$z), 0.9)
  
  confMatSampled <- table(resGen$z, getZ_class(resLearn))
  print(confMatSampled)
})

test_that("NegativeBinomial model works",{
  set.seed(42)
  
  nInd <- 10000
  
  var <- list()
  var$z_class <- zParam()
  
  var$nBinom0 <- nBinomParamRandom("nBinom0")
  var$nBinom1 <- nBinomParamRandom("nBinom1")
  var$nBinom2 <- nBinomParamRandom("nBinom2")
  var$nBinom3 <- nBinomParamRandom("nBinom3")
  var$nBinom4 <- nBinomParamRandom("nBinom4")
  
  resGen <- dataGeneratorNewIO(nInd, 0.9, var)
  
  algo <- list(
    nClass = 2,
    nInd = nInd,
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 100,
    nbGibbsIter = 100,
    nInitPerClass = 100,
    nSemTry = 20,
    confidenceLevel = 0.95,
    mode = "learn"
  )
  
  data <- resGen$data
  desc <- resGen$desc
  
  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), resGen$z), 0.9)
  
  confMatSampled <- table(resGen$z, getZ_class(resLearn))
  print(confMatSampled)
})

test_that("multinomial model works",{
  set.seed(42)
  
  nInd <- 1000
  
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
  
  resGen <- dataGeneratorNewIO(nInd, 0.9, var)
  
  algo <- list(
    nClass = 2,
    nInd = nInd,
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 100,
    nbGibbsIter = 100,
    nInitPerClass = 100,
    nSemTry = 20,
    confidenceLevel = 0.95,
    mode = "learn"
  )
  
  data <- resGen$data
  desc <- resGen$desc
  
  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), resGen$z), 0.9)
  
  confMatSampled <- table(resGen$z, getZ_class(resLearn))
  print(confMatSampled)
})

test_that("weibull model works",{
  set.seed(42)
  
  nInd <- 1000
  
  var <- list()
  var$z_class <- zParam()
  
  var$Weibull1 <- weibullParam("Weibull1")
  
  resGen <- dataGeneratorNewIO(nInd, 0.95, var)
  
  algo <- list(
    nClass = 2,
    nInd = nInd,
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 100,
    nbGibbsIter = 100,
    nInitPerClass = 100,
    nSemTry = 20,
    confidenceLevel = 0.95,
    mode = "learn"
  )
  
  data <- resGen$data
  desc <- resGen$desc
  
  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), resGen$z), 0.9)
  
  confMatSampled <- table(resGen$z, getZ_class(resLearn))
  print(confMatSampled)
})

Sys.unsetenv("MC_DETERMINISTIC")
