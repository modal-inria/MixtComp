# MixtComp version 4.0  - july 2019
# Copyright (C) Inria - Université de Lille - CNRS

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# 
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>

# @author Quentin Grimonprez
context("Test format")

Sys.setenv(MC_DETERMINISTIC = 42)

test_that("convert a matrix from json", {
  # named matrice
  x <- list(colNames = c("varCategorical", "Gaussian1", "varPoisson"), 
            ctype ="Matrix",
            data = matrix(1:6, nrow = 2, ncol = 3),
            dtype = "Real",
            ncol = 3,
            nrow = 2,
            rowNames = c("k: 1", "k: 2"))
  
  expectedOut <- matrix(1:6, nrow = 2, ncol = 3, dimnames = list(x$rowNames, x$colNames))
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)
  
  
  # unnamed matrice
  x$colNames = list()
  x$rowNames = list()
  
  expectedOut <- matrix(1:6, nrow = 2, ncol = 3, dimnames = list(NULL, NULL))
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)
  
  
  # void matrice
  x$nrow = 0
  x$ncol = 0
  x$data = list()
  
  expectedOut <- matrix(nrow = 0, ncol = 0, dimnames = list(NULL, NULL))
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)
})

test_that("convert a vector from json", {
  # unnamed vector
  x <- list(ctype ="Vector",
            data = 1:6,
            dtype = "Real",
            nrow = 6,
            rowNames = list())
  
  expectedOut <- 1:6
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)  
  
  
  # named vector
  x$rowNames = letters[1:6]
  names(expectedOut) <- letters[1:6]
  
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)  
  
  
  # void case
  x$nrow = 0
  x$data = list()
  x$rowNames = list()
  
  expectedOut <- c()
  out <- convertJsonElement(x)
  expect_equal(out, expectedOut)  
})



test_that("convertOutput converts well", {
  pathToData <- system.file("extdata", "data.json", package = "RJMixtComp")
  pathToDescriptor <- system.file("extdata", "desc.json", package = "RJMixtComp")
  
  data <- as.data.frame(fromJSON(pathToData))
  descriptor <- fromJSON(pathToDescriptor)
  algo <- createAlgo()
  
  expect_silent(res <- JMixtCompLearn(data, descriptor, algo, nClass = 2, inputPath = ".", outputFile = "reslearn.json"))
  
  res <- fromJSON("reslearn.json")
  out <- convertOutput(res)
  
  # warnLog
  expect_true(is.null(res$warnLog))
  
  # mixture 
  expect_equal(out$mixture$BIC, res$mixture$BIC)
  expect_equal(out$mixture$ICL, res$mixture$ICL)
  expect_equal(out$mixture$runTime, res$mixture$runTime)
  expect_equal(out$mixture$lnCompletedLikelihood, res$mixture$lnCompletedLikelihood)
  expect_equal(out$mixture$lnObservedLikelihood, res$mixture$lnObservedLikelihood)
  expect_equal(out$mixture$nbFreeParameters, res$mixture$nbFreeParameters)
  expect_equal(class(out$mixture$IDClass)[1], "matrix")
  expect_equal(rownames(out$mixture$IDClass), res$mixture$IDClass$rowNames)
  expect_equal(colnames(out$mixture$IDClass), res$mixture$IDClass$colNames)
  expect_equal(class(out$mixture$delta)[1], "matrix")
  expect_equal(class(out$mixture$lnProbaGivenClass)[1], "matrix")
  expect_length(out$mixture$completedProbabilityLogBurnIn, res$mixture$completedProbabilityLogBurnIn$nrow)
  expect_length(out$mixture$completedProbabilityLogRun, res$mixture$completedProbabilityLogRun$nrow)
  
  # variable$type
  expect_equal(out$variable$type, res$variable$type)
  
  # variable$param
  expect_equal(class(out$variable$param$z_class$stat)[1], "matrix")
  expect_equal(rownames(out$variable$param$z_class$stat), res$variable$param$z_class$stat$rowNames)
  expect_equal(colnames(out$variable$param$z_class$stat), res$variable$param$z_class$stat$colNames)
  expect_equal(out$variable$param$z_class$paramStr, res$variable$param$z_class$paramStr)
  expect_equal(class(out$variable$param$Gaussian1$stat)[1], "matrix")
  expect_equal(rownames(out$variable$param$Gaussian1$stat), res$variable$param$Gaussian1$stat$rowNames)
  expect_equal(colnames(out$variable$param$Gaussian1$stat), res$variable$param$Gaussian1$stat$colNames)
  expect_equal(out$variable$param$Gaussian1$paramStr, res$variable$param$Gaussian1$paramStr)
  
  # variable$data
  expect_equivalent(out$variable$data$z_class$stat, res$variable$data$z_class$stat$data)
  expect_equal(out$variable$data$z_class$completed, res$variable$data$z_class$completed$data)
  expect_equal(out$variable$data$Gaussian1$completed, res$variable$data$Gaussian1$completed$data)
  
  file.remove("./algo.json", "./descriptor.json", "./data.json", "reslearn.json", "progress")
})

Sys.unsetenv("MC_DETERMINISTIC")