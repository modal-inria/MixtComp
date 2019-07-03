# MixtComp version 4.0  - july 2019
# Copyright (C) Inria - Université de Lille - CNRS

# @author Quentin Grimonprez
context("multi run")

Sys.setenv(MC_DETERMINISTIC = 42)

test_that("rmcMultiRun works", {
  set.seed(42)
  
  nInd <- 1000
  
  var <- list()
  var$z_class <- zParam()
  var$z_class$param <- c(0.2, 0.3, 0.15, 0.35)
  var$Gaussian1 <- gaussianParam("Gaussian1")
  var$Gaussian1$param[[3]] <- list(mean = -2, sd = 1)
  var$Gaussian1$param[[4]] <- list(mean = 2, sd = 1)
  
  resGen <- dataGeneratorNewIO(nInd, 0.9, var)
  
  algo <- list(
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 100,
    nbGibbsIter = 100,
    nInitPerClass = 100,
    nSemTry = 20,
    nInitPerClass = 100,
    confidenceLevel = 0.95,
    nClass = 4,
    ratioStableCriterion = 0.95,
    nStableCriterion = 10,
    mode = "learn",
    nInd = 1000
  )
  
  resLearn <- rmcMultiRun(algo, resGen$data, resGen$desc, list(), nRun = 1) 
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  
  expect_equal(names(resLearn), c("mixture", "variable", "algo"))
  
  
  resLearn <- rmcMultiRun(algo, resGen$data, resGen$desc, list(), nCore = 2, nRun = 5) 
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  
  expect_equal(names(resLearn), c("mixture", "variable", "algo"))
  
})

Sys.unsetenv("MC_DETERMINISTIC")