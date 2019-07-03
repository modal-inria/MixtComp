# MixtComp version 4 - july 2019
# Copyright (C) Inria - Université de Lille - CNRS 

# @author Quentin Grimonprez
Sys.setenv(MC_DETERMINISTIC = 42)

context("Launch hierarchical MixtComp")


test_that("simple case", {
  
  set.seed(42)
  data <- matrix(nrow = 200, ncol = 2)
  data[,1] = c(rnorm(50, 1, 0.5), rnorm(50, -4, 0.5), rnorm(30, 3, 0.5), rnorm(70, -1, 0.4))
  data[,2] = c(rnorm(100, -2, 1), rnorm(100, 3, 0.4))
  colnames(data) = c("gauss1", "gauss2")
  truePartition <- rep(1:4, c(50, 50, 30, 70))
  
  model <- list(gauss1 = "Gaussian", gauss2 = "Gaussian")
  
  algo <- list(nbBurnInIter = 50,
               nbIter = 50,
               nbGibbsBurnInIter = 50,
               nbGibbsIter = 50,
               nInitPerClass = 10,
               nSemTry = 20,
               ratioStableCriterion = 0.95,
               nStableCriterion = 10,
               confidenceLevel = 0.95)
  
  expect_warning(res <- hierarchicalMixtCompLearn(data, model, algo, nClass = 4, criterion = "BIC", minClassSize = 5), regexp = NA)
  
  partition = res$variable$data$z_class$completed
  
  expect_true(is.null(res$warnLog))
  expect_gt(RMixtCompIO:::rand.index(partition, truePartition), 0.9)
})

Sys.unsetenv("MC_DETERMINISTIC")
