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
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(rand.index(getPartition(resLearn), resGen$z), 0.9)

  confMatSampled <- table(resGen$z, getPartition(resLearn))
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
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(rand.index(getPartition(resLearn), resGen$z), 0.9)

  confMatSampled <- table(resGen$z, getPartition(resLearn))
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
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(rand.index(getPartition(resLearn), resGen$z), 0.9)

  confMatSampled <- table(resGen$z, getPartition(resLearn))
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

  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(rand.index(getPartition(resLearn), resGen$z), 0.9)

  confMatSampled <- table(resGen$z, getPartition(resLearn))
  print(confMatSampled)
})

test_that("weibull model works",{
  set.seed(42)

  nInd <- 1000
  ratioPresent <- 0.95

  var <- list()
  var$z_class <- zParam()

  var$Weibull1 <- weibullParam("Weibull1")

  resGen <- dataGeneratorNewIO(nInd, ratioPresent, var)

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

  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(rand.index(getPartition(resLearn), resGen$z), 0.9)

  confMatSampled <- table(resGen$z, getPartition(resLearn))
  print(confMatSampled)
})

test_that("functional model works",{
  set.seed(42)

  nInd <- 200
  ratioPresent <- 0.95

  var <- list()
  var$z_class <- zParam()

  var$Functional1 <- functionalInterPolyParam("Functional1")

  resGen <- dataGeneratorNewIO(nInd, ratioPresent, var)

  algo <- list(
    nClass = 2,
    nInd = nInd,
    nbBurnInIter = 50,
    nbIter = 50,
    nbGibbsBurnInIter = 50,
    nbGibbsIter = 50,
    nInitPerClass = 50,
    nSemTry = 20,
    confidenceLevel = 0.95,
    mode = "learn"
  )

  data <- resGen$data
  desc <- resGen$desc

  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(rand.index(getPartition(resLearn), resGen$z), 0.9)

  confMatSampled <- table(resGen$z, getPartition(resLearn))
  print(confMatSampled)
})


test_that("functional model with shared alpha works",{
  set.seed(42)

  nInd <- 400
  ratioPresent <- 0.9

  var <- list()
  var$z_class <- zParam()

  var$functionalSharedAlpha1 <- functionalSharedAlphaInterPolyParam("functionalSharedAlpha1")

  resGen <- dataGeneratorNewIO(nInd, ratioPresent, var)

  algo <- list(
    nClass = 2,
    nInd = nInd,
    nbBurnInIter = 50,
    nbIter = 50,
    nbGibbsBurnInIter = 50,
    nbGibbsIter = 50,
    nInitPerClass = 100,
    nSemTry = 20,
    confidenceLevel = 0.95,
    mode = "learn"
  )

  data <- resGen$data
  desc <- resGen$desc

  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(rand.index(getPartition(resLearn), resGen$z), 0.9)

  confMatSampled <- table(resGen$z, getPartition(resLearn))
  print(confMatSampled)
})

test_that("rank model works",{
  set.seed(42)

  nInd <- 1000
  ratioPresent <- 0.95

  var <- list()
  var$z_class <- zParam()

  var$Rank1 <- rankParam("Rank1")

  resGen <- dataGeneratorNewIO(nInd, ratioPresent, var)

  algo <- list(
    nClass = 2,
    nInd = nInd,
    nbBurnInIter = 50,
    nbIter = 50,
    nbGibbsBurnInIter = 50,
    nbGibbsIter = 50,
    nInitPerClass = 50,
    nSemTry = 20,
    confidenceLevel = 0.95,
    mode = "learn"
  )

  data <- resGen$data
  desc <- resGen$desc

  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(rand.index(getPartition(resLearn), resGen$z), 0.8)

  confMatSampled <- table(resGen$z, getPartition(resLearn))
  print(confMatSampled)
})

test_that("run cluster/predict R object",{
  set.seed(42)

  var <- list()
  var$z_class <- zParam()
  var$Poisson1 <- poissonParam("Poisson1")
  var$Gaussian1 <- gaussianParam("Gaussian1")
  var$Categorical1 <- categoricalParam1("Categorical1")
  var$nBinom1 <- nBinomParamRandom("nBinom1")
  var$Weibull1 <- weibullParam("Weibull1")
  var$Functional1 <- functionalInterPolyParam("Functional1")
  var$functionalSharedAlpha1 <- functionalSharedAlphaInterPolyParam("functionalSharedAlpha1")
  var$Rank1 <- rankParam("Rank1")

  resGenLearn <- dataGeneratorNewIO(200, 0.9, var)

  algoLearn <- list(
    nClass = 2,
    nInd = 200,
    nbBurnInIter = 50,
    nbIter = 50,
    nbGibbsBurnInIter = 50,
    nbGibbsIter = 50,
    nInitPerClass = 100,
    nSemTry = 20,
    confidenceLevel = 0.95,
    mode = "learn"
  )

  dataLearn <- resGenLearn$data
  desc <- resGenLearn$desc

  resLearn <- rmc(algoLearn, dataLearn, desc, list()) # run RMixtCompt for clustering

  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(rand.index(getPartition(resLearn), resGenLearn$z), 0.9)

  
  # test that getter functions do not return warnings
  expect_warning(getPartition(resLearn), regexp = NA)
  expect_warning(getBIC(resLearn), regexp = NA)
  expect_true(!is.na(getBIC(resLearn)))
  expect_warning(getICL(resLearn), regexp = NA)
  expect_true(!is.na(getICL(resLearn)))
  expect_warning(getPartition(resLearn), regexp = NA)
  expect_warning(getCompletedData(resLearn), regexp = NA)
  expect_equivalent(getType(resLearn), setdiff(sapply(desc, function(x){x$type}), "LatentClass"))
  expect_equivalent(getVarNames(resLearn), setdiff(names(desc), "z_class"))
  expect_warning(getTik(resLearn), regexp = NA)
  expect_equal(dim(getEmpiricTik(resLearn)), c(algoLearn$nInd, algoLearn$nClass))
  expect_warning(getEmpiricTik(resLearn), regexp = NA)
  expect_equal(dim(getTik(resLearn)), c(algoLearn$nInd, algoLearn$nClass))
  expect_warning(disc <- computeDiscrimPowerClass(resLearn), regexp = NA)
  expect_equal(length(disc), algoLearn$nClass)
  expect_warning(disc <- computeDiscrimPowerVar(resLearn), regexp = NA)
  expect_equal(length(disc), length(desc)-1)
  expect_warning(disc <- computeSimilarityClass(resLearn), regexp = NA)
  expect_equal(dim(disc), rep(algoLearn$nClass, 2))
  expect_warning(disc <- computeSimilarityVar(resLearn), regexp = NA)
  expect_equal(dim(disc), rep(length(desc)-1, 2))
  for(name in getVarNames(resLearn))
    expect_warning(getParam(resLearn, name), regexp = NA)
  
  
  # test plot functions
  expect_warning(plotConvergence(resLearn), regexp = NA)
  w <- capture_warnings(plotDiscrimClass(resLearn))# the first call generates warnings due to packages loading
  if(length(w) > 0)
    expect_match(w, "replacing previous", all = TRUE)
  expect_warning(plotDiscrimVbles(resLearn), regexp = NA)
  expect_warning(heatmapVbles(resLearn), regexp = NA)
  expect_warning(heatmapClass(resLearn), regexp = NA)
  expect_warning(heatmapTikSorted(resLearn), regexp = NA)
  expect_warning(histMisclassif(resLearn), regexp = NA)
  for(name in getVarNames(resLearn))
  {
    if(resLearn$variable$type[[name]] != "Rank_ISR")
    {
      expect_warning(plotDataCI(resLearn, name), regexp = NA)
      expect_warning(plotDataBoxplot(resLearn, name), regexp = NA)
    }else{
      expect_warning(plotDataCI(resLearn, name))
      expect_warning(plotDataBoxplot(resLearn, name))
    }
  }
  file.remove("Rplots.pdf")
  
  resGenPredict <- dataGeneratorNewIO(100, 0.9, var)

  algoPredict <- list(
    nClass = 2,
    nInd = 100,
    nbBurnInIter = 50,
    nbIter = 50,
    nbGibbsBurnInIter = 50,
    nbGibbsIter = 50,
    nInitPerClass = 100,
    nSemTry = 20,
    confidenceLevel = 0.95,
    mode = "predict"
  )

  dataPredict <- resGenPredict$data

  resPredict <- rmc(algoPredict, dataPredict, desc, resLearn) # run RMixtCompt for clustering

  expect_equal(resPredict$warnLog, NULL)
  if(!is.null(resPredict$warnLog))
    print(resPredict$warnLog)
  expect_gte(rand.index(getPartition(resPredict), resGenPredict$z), 0.85)
})

Sys.unsetenv("MC_DETERMINISTIC")
