# MixtComp version 4.0  - july 2019
# Copyright (C) Inria - Université de Lille - CNRS

# @author Vincent Kubicki
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
    ratioStableCriterion = 0.95,
    nStableCriterion = 10,
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
  
  expect_equal(resLearn$warnLog, NULL)
  partition <- resLearn$variable$data$z_class$completed
  expect_gte(rand.index(partition, zLearn), 0.9)
  
  confMatSampledLearn <- table(zLearn, partition)
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
    ratioStableCriterion = 0.95,
    nStableCriterion = 10,
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
  
  expect_equal(resPredict$warnLog, NULL)
  partition <- resPredict$variable$data$z_class$completed
  expect_gte(rand.index(partition, zPredict), 0.8)
  
  confMatSampledPredict <- table(zPredict, partition)
  print(confMatSampledPredict)
  
  # test with a bad nClass in desc
  algoPredict$nClass = 3
  resPredict <- rmc(algoPredict, dataPredict, descPredict, resLearn)
  expect_named(resPredict, "warnLog")
  expect_equal(resPredict$warnLog, "The nClass parameter provides in algo is different from the one in resLearn.")
  
})

Sys.unsetenv("MC_DETERMINISTIC")