context("Simple run")

Sys.setenv(MC_DETERMINISTIC = 42)

test_that("Hard coded simple test", {
  set.seed(42)
  
  algoLearn <- list(
    nClass = 2,
    nInd = 20,
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 100,
    nbGibbsIter = 100,
    nInitPerClass = 3,
    nSemTry = 20,
    confidenceLevel = 0.95,
    ratioStableCriterium = 0.9,
    nStableCriterium = 5,
    mode = "learn"
  )
  
  dataLearn <- list(
    var1 = c(
      "3.432200",
      "19.14747",
      "5.258037",
      "22.37596",
      "2.834802",
      "18.60959",
      "4.640250",
      "18.59525",
      "5.957942",
      "19.94644",
      "5.560189",
      "17.98966",
      "6.708977",
      "18.10192",
      "5.331169",
      "20.35260",
      "4.003947",
      "21.81531",
      "6.217908",
      "19.38892"
    )
  )
  
  descLearn <- list(var1 = list(type = "Gaussian",
                                paramStr = ""))
  
  zLearn <- c(1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2)
  
  resLearn <- rmc(algoLearn, dataLearn, descLearn, list())
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getPartition(resLearn), zLearn), 0.9)
  
  confMatSampledLearn <- table(zLearn, getPartition(resLearn))
  print(confMatSampledLearn)
  
  algoPredict <- list(
    nClass = 2,
    nInd = 6,
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 100,
    nbGibbsIter = 100,
    nInitPerClass = 3,
    nSemTry = 20,
    confidenceLevel = 0.95,
    mode = "predict"
  )
  
  dataPredict <- list(var1 = c(
    "4.838457",
    "19.90595",
    "4.577347",
    "21.19830",
    "5.048325",
    "20.46875"
  ))
  
  descPredict <- list(var1 = list(type = "Gaussian",
                                  paramStr = ""))
  
  zPredict <- c(1, 2, 1, 2, 1, 2)
  
  resPredict <- rmc(algoPredict, dataPredict, descPredict, resLearn)
  
  expect_equal(resPredict$mixture$warnLog, NULL)
  expect_gte(rand.index(getPartition(resPredict), zPredict), 0.8)
  
  confMatSampledPredict <- table(zPredict, getPartition(resPredict))
  print(confMatSampledPredict)
})

Sys.unsetenv("MC_DETERMINISTIC")