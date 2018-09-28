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

Sys.unsetenv("MC_DETERMINISTIC")
