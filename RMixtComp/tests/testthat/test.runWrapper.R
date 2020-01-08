# MixtComp version 4 - july 2019
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
context("run functions")

# These tests are skipped on cran to keep elapsed time < 10min

Sys.setenv(MC_DETERMINISTIC = 2)

test_that("mixtCompLearn works in basic mode + predict", {
  skip_on_cran()
  set.seed(42, kind = "Mersenne-Twister", normal.kind = "Inversion")
  
  ## data.frame object
  dat <- data.frame(cont = c(rnorm(150, -2, 0.8), rnorm(150, 2, 0.8)),
                    categ = c("a", "b")[c(apply(rmultinom(150, 1, c(0.5, 0.5)), 2, which.max), apply(rmultinom(150, 1, c(0.2, 0.8)), 2, which.max))],
                    poiss = c(rpois(150, 2), rpois(150, 5)))
  
  expect_warning(resLearn <- mixtCompLearn(dat, nClass = 2, nRun = 3, nCore = 1), regexp = NA)
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(RMixtCompIO:::rand.index(getPartition(resLearn), rep(1:2, each = 150)), 0.9)
  expect_lte(norm(getTik(resLearn, log = FALSE) - getEmpiricTik(resLearn))/resLearn$algo$nInd, 0.1)
  
  expect_equal(resLearn$variable$type, list(z_class = "LatentClass", cont = "Gaussian", categ = "Multinomial", poiss = "Poisson"))
  expect_true(resLearn$algo$basicMode)
  expect_false(resLearn$algo$hierarchicalMode)
  expect_equal(resLearn$algo$dictionary, list(categ = list(old = c("a", "b"), new = c("1", "2"))))
  expect_equal(resLearn$variable$data$categ$completed, as.character(dat$categ))
  expect_equal(rownames(resLearn$variable$param$categ$stat), c("k: 1, modality: a", "k: 1, modality: b", "k: 2, modality: a", "k: 2, modality: b"))
  
  expect_warning(resPredict <- mixtCompPredict(dat, resLearn = resLearn), regexp = NA)
  
  if(!is.null(resPredict$warnLog))
    print(resPredict$warnLog)
  
  expect_equal(resPredict$warnLog, NULL)
  expect_gte(RMixtCompIO:::rand.index(getPartition(resPredict), rep(1:2, each = 150)), 0.9)
  expect_equal(resPredict$algo[1:7], resLearn$algo[1:7]) # check that algo param form resLearn are used
  expect_true(resPredict$algo$basicMode)
  expect_equal(resPredict$algo$dictionary, list(categ = list(old = c("a", "b"), new = c("1", "2"))))
  expect_equal(resPredict$variable$data$categ$completed, as.character(dat$categ))
  expect_equal(rownames(resPredict$variable$param$categ$stat), c("k: 1, modality: a", "k: 1, modality: b", "k: 2, modality: a", "k: 2, modality: b"))
  
  
  ## list object
  dat <- list(cont = c(rnorm(150, -2, 0.8), rnorm(150, 2, 0.8)),
              categ1 = as.character(c(apply(rmultinom(150, 1, c(0.5, 0.5)), 2, which.max), apply(rmultinom(150, 1, c(0.2, 0.8)), 2, which.max))),
              categ2 = as.factor(c(apply(rmultinom(150, 1, c(0.5, 0.5)), 2, which.max), apply(rmultinom(150, 1, c(0.2, 0.8)), 2, which.max))),
              poiss = c(rpois(150, 2), rpois(150, 5)))
  
  expect_warning(resLearn <- mixtCompLearn(dat, nClass = 2), regexp = NA)
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(RMixtCompIO:::rand.index(getPartition(resLearn), rep(1:2, each = 150)), 0.9)
  expect_lte(norm(getTik(resLearn, log = FALSE) - getEmpiricTik(resLearn))/resLearn$algo$nInd, 0.1)
  
  expect_equal(resLearn$variable$type, list(z_class = "LatentClass", cont = "Gaussian", categ1 = "Multinomial", categ2 = "Multinomial", poiss = "Poisson"))
  expect_true(resLearn$algo$basicMode)
  expect_false(resLearn$algo$hierarchicalMode)
  expect_equal(resLearn$algo$dictionary, list(categ1 = list(old = c("1", "2"), new = c("1", "2")),
                                              categ2 = list(old = c("1", "2"), new = c("1", "2"))))
  expect_equal(resLearn$variable$data$categ1$completed, as.character(dat$categ1))
  expect_equal(resLearn$variable$data$categ2$completed, as.character(dat$categ2))
  expect_equal(rownames(resLearn$variable$param$categ1$stat), c("k: 1, modality: 1", "k: 1, modality: 2", "k: 2, modality: 1", "k: 2, modality: 2"))
  expect_equal(rownames(resLearn$variable$param$categ2$stat), c("k: 1, modality: 1", "k: 1, modality: 2", "k: 2, modality: 1", "k: 2, modality: 2"))
  
  
  
  expect_warning(resPredict <- mixtCompPredict(dat, resLearn = resLearn), regexp = NA)
  
  if(!is.null(resPredict$warnLog))
    print(resPredict$warnLog)
  
  expect_equal(resPredict$warnLog, NULL)
  expect_gte(RMixtCompIO:::rand.index(getPartition(resPredict), rep(1:2, each = 150)), 0.9)
  expect_lte(norm(getTik(resPredict, log = FALSE) - getEmpiricTik(resPredict))/resPredict$algo$nInd, 0.1)
  expect_true(resPredict$algo$basicMode)
  expect_equal(resPredict$algo$dictionary, list(categ1 = list(old = c("1", "2"), new = c("1", "2")),
                                                categ2 = list(old = c("1", "2"), new = c("1", "2"))))
  expect_equal(resPredict$variable$data$categ1$completed, as.character(dat$categ1))
  expect_equal(resPredict$variable$data$categ2$completed, as.character(dat$categ2))
  expect_equal(rownames(resPredict$variable$param$categ1$stat), c("k: 1, modality: 1", "k: 1, modality: 2", "k: 2, modality: 1", "k: 2, modality: 2"))
  expect_equal(rownames(resPredict$variable$param$categ2$stat), c("k: 1, modality: 1", "k: 1, modality: 2", "k: 2, modality: 1", "k: 2, modality: 2"))
  
  
  
  ## with z_class and without multinomial
  dat$z_class = rep(1:2, each = 150)
  dat$categ1 = NULL
  dat$categ2 = NULL
  
  expect_warning(resLearn <- mixtCompLearn(dat, nClass = 2), regexp = NA)
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(RMixtCompIO:::rand.index(getPartition(resLearn), rep(1:2, each = 150)), 0.95)
  expect_equal(resLearn$variable$type, list(z_class = "LatentClass", cont = "Gaussian", poiss = "Poisson"))
  expect_true(resLearn$algo$basicMode)
  expect_false(resLearn$algo$hierarchicalMode)
  expect_equal(resLearn$algo$dictionary, list())
  
  
  dat$z_class = NULL
  expect_warning(resPredict <- mixtCompPredict(dat, resLearn = resLearn), regexp = NA)
  expect_gte(RMixtCompIO:::rand.index(getPartition(resPredict), rep(1:2, each = 150)), 0.95)
  expect_true(resPredict$algo$basicMode)
  expect_equal(resPredict$algo$dictionary, list())
})



test_that("plot in basic mode + predict works with z_class as character", {
  skip_on_cran()
  data(iris)
  
  names(iris)[5] = "z_class"
  
  expect_warning(resLearn <- mixtCompLearn(iris, nClass = 3), regexp = NA)
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_lte(norm(getTik(resLearn, log = FALSE) - getEmpiricTik(resLearn))/resLearn$algo$nInd, 0.1)
  
  expect_equal(resLearn$variable$type, list(z_class = "LatentClass", Sepal.Length = "Gaussian", Sepal.Width = "Gaussian", Petal.Length = "Gaussian", Petal.Width = "Gaussian"))
  expect_true(resLearn$algo$basicMode)
  expect_false(resLearn$algo$hierarchicalMode)
  expect_equal(resLearn$algo$dictionary, list(z_class = list(old = c("setosa", "versicolor", "virginica"), new = c("1", "2", "3"))))
  expect_equal(resLearn$variable$data$z_class$completed, as.character(iris$z_class))
  expect_equal(rownames(resLearn$variable$param$z_class$stat), c("k: setosa", "k: versicolor", "k: virginica"))
  
  expect_warning(resPredict <- mixtCompPredict(iris[, 1:4], resLearn = resLearn), regexp = NA)
  
  if(!is.null(resPredict$warnLog))
    print(resPredict$warnLog)
  
  expect_equal(resPredict$warnLog, NULL)
  expect_equal(resPredict$algo$dictionary, list(z_class = list(old = c("setosa", "versicolor", "virginica"), new = c("1", "2", "3"))))
  expect_equal(rownames(resPredict$variable$param$z_class$stat), c("k: setosa", "k: versicolor", "k: virginica"))
  
  
  w <- capture_warnings(plotDiscrimClass(resLearn, pkg = "plotly"))# the first call generates warnings due to packages loading
  if(length(w) > 0)
    expect_match(w, "replacing previous", all = TRUE)
  expect_warning(plotDiscrimClass(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(heatmapClass(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(heatmapTikSorted(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(heatmapClass(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(heatmapTikSorted(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(histMisclassif(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(histMisclassif(resLearn, pkg = "plotly"), regexp = NA)
  
  for(name in getVarNames(resLearn))
  {
    expect_warning(plotParamConvergence(resLearn, name), regexp = NA)
    expect_warning(plotDataCI(resLearn, name, pkg = "plotly"), regexp = NA)
    expect_warning(plotDataCI(resLearn, name, pkg = "ggplot2"), regexp = NA)
    expect_warning(plotDataBoxplot(resLearn, name, pkg = "plotly"), regexp = NA)
    expect_warning(plotDataBoxplot(resLearn, name, pkg = "ggplot2"), regexp = NA)
  }
  
  expect_warning(plotProportion(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(plotProportion(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(plot(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(plot(resLearn, pkg = "plotly"), regexp = NA)
  file.remove("Rplots.pdf")
  
  
})





test_that("mixtCompLearn works + mixtCompPredict", {
  skip_on_cran()
  set.seed(42, kind = "Mersenne-Twister", normal.kind = "Inversion")
  
  nInd <- 2000
  
  var <- list()
  var$z_class <- RMixtCompIO:::zParam()
  var$z_class$param <- c(0.2, 0.3, 0.15, 0.35)
  var$Gaussian1 <- RMixtCompIO:::gaussianParam("Gaussian1")
  var$Gaussian1$param[[3]] <- list(mean = -1.5, sd = 0.4)
  var$Gaussian1$param[[4]] <- list(mean = 1.5, sd = 0.4)
  
  resGen <- RMixtCompIO:::dataGeneratorNewIO(nInd, 0.95, var)
  
  algo <- list(
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 100,
    nbGibbsIter = 100,
    nInitPerClass = 100,
    nSemTry = 20,
    nInitPerClass = 100,
    confidenceLevel = 0.95
  )
  
  data <- do.call(cbind, resGen$data)
  desc <- list(z_class = "LatentClass", Gaussian1 = "Gaussian")
  
  resLearn <- mixtCompLearn(data, desc, algo, nClass = 4, crit = "ICL", verbose = FALSE, nRun = 3, nCore = 1) 
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(RMixtCompIO:::rand.index(getPartition(resLearn), resGen$z), 0.9)
  expect_lte(norm(getTik(resLearn, log = FALSE) - getEmpiricTik(resLearn))/resLearn$algo$nInd, 0.1)
  
  confMatSampled <- table(resGen$z, getPartition(resLearn))
  print(confMatSampled)
  
  expect_equal(names(resLearn), c("mixture", "variable", "algo", "nRun", "criterion", "crit", "nClass", "res"))
  expect_equal(resLearn$criterion, "ICL")
  expect_equal(resLearn$nRun, 3)
  expect_equal(dim(resLearn$crit), c(2, 1))
  expect_equal(resLearn$nClass, 4)
  expect_equal(resLearn$algo$mode, "learn")
  expect_false(resLearn$algo$basicMode)
  expect_false(resLearn$algo$hierarchicalMode)
  expect_equal(length(resLearn$res), 1)
  expect_equal(names(resLearn$res[[1]]), c("mixture", "variable", "algo"))
  expect_warning(getPartition(resLearn), regexp = NA)
  expect_warning(getBIC(resLearn), regexp = NA)
  expect_true(!is.na(getBIC(resLearn)))
  expect_warning(getICL(resLearn), regexp = NA)
  expect_true(!is.na(getICL(resLearn)))
  expect_warning(getPartition(resLearn), regexp = NA)
  expect_warning(getCompletedData(resLearn), regexp = NA)
  expect_equivalent(getType(resLearn), "Gaussian")
  expect_equivalent(getVarNames(resLearn), "Gaussian1")
  expect_warning(getTik(resLearn), regexp = NA)
  expect_equal(dim(getEmpiricTik(resLearn)), c(2000, 4))
  expect_warning(getEmpiricTik(resLearn), regexp = NA)
  expect_equal(dim(getTik(resLearn)), c(2000, 4))
  expect_warning(disc <- computeDiscrimPowerClass(resLearn), regexp = NA)
  expect_equal(length(disc), 4)
  expect_warning(disc <- computeDiscrimPowerVar(resLearn), regexp = NA)
  expect_equal(length(disc), 1)
  expect_warning(disc <- computeSimilarityClass(resLearn), regexp = NA)
  expect_equal(dim(disc), rep(4, 2))
  expect_warning(disc <- computeSimilarityVar(resLearn), regexp = NA)
  expect_equal(dim(disc), rep(1, 2))
  for(name in getVarNames(resLearn))
    expect_warning(getParam(resLearn, name), regexp = NA)
  
  
  expect_warning(resPredict <- mixtCompPredict(data, desc, resLearn = resLearn, verbose = FALSE), regexp = NA)
  expect_lte(norm(getTik(resPredict, log = FALSE) - getEmpiricTik(resPredict))/resPredict$algo$nInd, 0.1)
  expect_equal(names(resPredict), c("mixture", "variable", "algo"))
  expect_equal(resPredict$algo$mode, "predict")
  expect_equal(resPredict$algo[1:7], resLearn$algo[1:7]) # check that algo param form resLearn are used
  
})

test_that("mixtCompLearn works with a vector for nClass + mixtCompPredict + verbose", {
  skip_on_cran()
  set.seed(42, kind = "Mersenne-Twister", normal.kind = "Inversion")
  
  nInd <- 1000
  
  var <- list()
  var$z_class <- RMixtCompIO:::zParam()
  var$z_class$param <- c(0.2, 0.3, 0.15, 0.35)
  var$Gaussian1 <- RMixtCompIO:::gaussianParam("Gaussian1")
  var$Gaussian1$param[[3]] <- list(mean = -2, sd = 0.5)
  var$Gaussian1$param[[4]] <- list(mean = 2, sd = 0.5)
  
  resGen <- RMixtCompIO:::dataGeneratorNewIO(nInd, 0.9, var)
  
  algo <- list(
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 100,
    nbGibbsIter = 100,
    nSemTry = 20,
    nInitPerClass = 100,
    confidenceLevel = 0.95
  )
  
  data <- do.call(cbind, resGen$data)
  desc <- list(z_class = list(type = "LatentClass"), Gaussian1 = list(type = "Gaussian", paramStr = ""))
  
  resLearn <- mixtCompLearn(data, desc, algo, nClass = 2:5, nRun = 5, nCore = 1, verbose = TRUE) 
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(RMixtCompIO:::rand.index(getPartition(resLearn), resGen$z), 0.85)
  expect_lte(norm(getTik(resLearn, log = FALSE) - getEmpiricTik(resLearn))/resLearn$algo$nInd, 0.1)
  
  confMatSampled <- table(resGen$z, getPartition(resLearn))
  print(confMatSampled)
  
  expect_equal(names(resLearn), c("mixture", "variable", "algo", "nRun", "criterion", "crit", "nClass", "res"))
  expect_equal(resLearn$nRun, 5)
  expect_equal(resLearn$criterion, "BIC")
  expect_equal(dim(resLearn$crit), c(2, 4))
  expect_equal(resLearn$nClass, 2:5)
  expect_equal(resLearn$algo$mode, "learn")
  expect_false(resLearn$algo$basicMode)
  expect_false(resLearn$algo$hierarchicalMode)
  expect_equal(length(resLearn$res), 4)
  expect_equal(names(resLearn$res[[1]]), c("mixture", "variable", "algo"))
  expect_warning(getPartition(resLearn), regexp = NA)
  expect_warning(getBIC(resLearn), regexp = NA)
  expect_true(!is.na(getBIC(resLearn)))
  expect_warning(getICL(resLearn), regexp = NA)
  expect_true(!is.na(getICL(resLearn)))
  expect_warning(getPartition(resLearn), regexp = NA)
  expect_warning(getCompletedData(resLearn), regexp = NA)
  expect_equivalent(getType(resLearn), "Gaussian")
  expect_equivalent(getVarNames(resLearn), "Gaussian1")
  expect_warning(getTik(resLearn), regexp = NA)
  expect_equal(dim(getEmpiricTik(resLearn)), c(1000, 4))
  expect_warning(getEmpiricTik(resLearn), regexp = NA)
  expect_equal(dim(getTik(resLearn)), c(1000, 4))
  expect_warning(disc <- computeDiscrimPowerClass(resLearn), regexp = NA)
  expect_equal(length(disc), 4)
  expect_warning(disc <- computeDiscrimPowerVar(resLearn), regexp = NA)
  expect_equal(length(disc), 1)
  expect_warning(disc <- computeSimilarityClass(resLearn), regexp = NA)
  expect_equal(dim(disc), rep(4, 2))
  expect_warning(disc <- computeSimilarityVar(resLearn), regexp = NA)
  expect_equal(dim(disc), rep(1, 2))
  for(name in getVarNames(resLearn))
    expect_warning(getParam(resLearn, name), regexp = NA)
  
  # test plot functions
  expect_warning(plotConvergence(resLearn), regexp = NA)
  w <- capture_warnings(plotDiscrimClass(resLearn, pkg = "plotly"))# the first call generates warnings due to packages loading
  if(length(w) > 0)
    expect_match(w, "replacing previous", all = TRUE)
  expect_warning(plotDiscrimVar(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(plotDiscrimClass(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(plotDiscrimVar(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(heatmapVar(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(heatmapClass(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(heatmapTikSorted(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(heatmapVar(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(heatmapClass(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(heatmapTikSorted(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(histMisclassif(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(histMisclassif(resLearn, pkg = "ggplot2"), regexp = NA)
  for(name in getVarNames(resLearn))
  {
    if(resLearn$variable$type[[name]] != "Rank_ISR")
    {
      expect_warning(plotDataCI(resLearn, name, pkg = "plotly"), regexp = NA)
      expect_warning(plotDataCI(resLearn, name, pkg = "ggplot2"), regexp = NA)
      expect_warning(plotDataBoxplot(resLearn, name, pkg = "plotly"), regexp = NA)
      expect_warning(plotDataBoxplot(resLearn, name, pkg = "ggplot2"), regexp = NA)
    }else{
      expect_warning(plotDataCI(resLearn, name, pkg = "ggplot2"))
      expect_warning(plotDataCI(resLearn, name, pkg = "plotly"))
      expect_warning(plotDataBoxplot(resLearn, name, pkg = "ggplot2"))
      expect_warning(plotDataBoxplot(resLearn, name, pkg = "plotly"))
    }
  }
  expect_warning(plotCrit(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(plotCrit(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(plotProportion(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(plotProportion(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(plot(resLearn$res[[3]], pkg = "ggplot2"), regexp = NA)
  expect_warning(plot(resLearn$res[[3]], pkg = "plotly"), regexp = NA)
  expect_warning(plot(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(plot(resLearn, pkg = "plotly"), regexp = NA)
  file.remove("Rplots.pdf")
  
  expect_warning(summary(resLearn), regexp = NA)
  expect_warning(summary(resLearn$res[[1]]), regexp = NA)
  expect_warning(print(resLearn), regexp = NA)
  expect_warning(print(resLearn$res[[1]]), regexp = NA)
  
  expect_warning(resPredict <- mixtCompPredict(data, desc, algo, resLearn, nClass = 3, verbose = TRUE), regexp = NA)
  
  if(!is.null(resPredict$warnLog))
    print(resPredict$warnLog)
  
  expect_lte(norm(getTik(resPredict, log = FALSE) - getEmpiricTik(resPredict))/resPredict$algo$nInd, 0.1)
  
  expect_equal(names(resPredict), c("mixture", "variable", "algo"))
  expect_equal(resPredict$algo$mode, "predict")
})


test_that("mixtCompLearn works in hierarchicalMode", {
  skip_on_cran()
  set.seed(42, kind = "Mersenne-Twister", normal.kind = "Inversion")
  
  data(simData)
  model <- simData$model$unsupervised[c("Gaussian1", "Functional1")]
  
  algo <- list(
    nbBurnInIter = 50,
    nbIter = 50,
    nbGibbsBurnInIter = 50,
    nbGibbsIter = 50,
    nSemTry = 10,
    nInitPerClass = 100,
    confidenceLevel = 0.95
  )
  
  
  resLearn <- mixtCompLearn(simData$dataLearn$matrix, model, algo, nClass = 3, nRun = 2, nCore = 1, verbose = TRUE) 
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(RMixtCompIO:::rand.index(getPartition(resLearn$res[[1]]), simData$dataLearn$data.frame$z_class), 0.85)
  expect_lte(norm(getTik(resLearn$res[[1]], log = FALSE) - getEmpiricTik(resLearn$res[[1]]))/resLearn$algo$nInd, 0.1)
  
  expect_equal(names(resLearn), c("mixture", "variable", "algo", "nRun", "criterion", "crit", "nClass", "res"))
  expect_equal(resLearn$nRun, 2)
  expect_equal(resLearn$criterion, "BIC")
  expect_equal(dim(resLearn$crit), c(2, 2))
  expect_equal(resLearn$nClass, 2:3)
  expect_equal(resLearn$algo$mode, "learn")
  expect_false(resLearn$algo$basicMode)
  expect_true(resLearn$algo$hierarchicalMode)
  expect_equal(length(resLearn$res), 2)
  expect_equal(names(resLearn$res[[1]]), c("mixture", "variable", "algo"))
  expect_warning(getPartition(resLearn), regexp = NA)
  expect_warning(getBIC(resLearn), regexp = NA)
  expect_true(!is.na(getBIC(resLearn)))
  expect_warning(getICL(resLearn), regexp = NA)
  expect_true(!is.na(getICL(resLearn)))
  expect_warning(getPartition(resLearn), regexp = NA)
  expect_warning(getCompletedData(resLearn), regexp = NA)
  expect_equivalent(getType(resLearn), c("Gaussian", "Func_CS"))
  expect_equivalent(getVarNames(resLearn), c("Gaussian1", "Functional1"))
  expect_warning(getTik(resLearn), regexp = NA)
  expect_warning(getEmpiricTik(resLearn), regexp = NA)
  expect_warning(disc <- computeDiscrimPowerClass(resLearn), regexp = NA)
  expect_warning(disc <- computeDiscrimPowerVar(resLearn), regexp = NA)
  expect_length(disc, 2)
  expect_warning(disc <- computeSimilarityClass(resLearn), regexp = NA)
  expect_warning(disc <- computeSimilarityVar(resLearn), regexp = NA)
  expect_equal(dim(disc), rep(2, 2))
  for(name in getVarNames(resLearn))
    expect_warning(getParam(resLearn, name), regexp = NA)
  
  # test plot functions
  expect_warning(plotConvergence(resLearn), regexp = NA)
  w <- capture_warnings(plotDiscrimClass(resLearn, pkg = "plotly"))# the first call generates warnings due to packages loading
  if(length(w) > 0)
    expect_match(w, "replacing previous", all = TRUE)
  expect_warning(plotDiscrimVar(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(plotDiscrimClass(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(plotDiscrimVar(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(heatmapVar(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(heatmapClass(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(heatmapTikSorted(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(heatmapVar(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(heatmapClass(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(heatmapTikSorted(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(histMisclassif(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(histMisclassif(resLearn, pkg = "ggplot2"), regexp = NA)
  for(name in getVarNames(resLearn))
  {
    if(resLearn$variable$type[[name]] != "Rank_ISR")
    {
      expect_warning(plotDataCI(resLearn, name, pkg = "plotly"), regexp = NA)
      expect_warning(plotDataCI(resLearn, name, pkg = "ggplot2"), regexp = NA)
      expect_warning(plotDataBoxplot(resLearn, name, pkg = "plotly"), regexp = NA)
      expect_warning(plotDataBoxplot(resLearn, name, pkg = "ggplot2"), regexp = NA)
    }else{
      expect_warning(plotDataCI(resLearn, name, pkg = "ggplot2"))
      expect_warning(plotDataCI(resLearn, name, pkg = "plotly"))
      expect_warning(plotDataBoxplot(resLearn, name, pkg = "ggplot2"))
      expect_warning(plotDataBoxplot(resLearn, name, pkg = "plotly"))
    }
  }
  expect_warning(plotCrit(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(plotCrit(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(plotProportion(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(plotProportion(resLearn, pkg = "plotly"), regexp = NA)
  expect_warning(plot(resLearn$res[[2]], pkg = "ggplot2"), regexp = NA)
  expect_warning(plot(resLearn$res[[2]], pkg = "plotly"), regexp = NA)
  expect_warning(plot(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(plot(resLearn, pkg = "plotly"), regexp = NA)
  file.remove("Rplots.pdf")
  
  expect_warning(summary(resLearn), regexp = NA)
  expect_warning(summary(resLearn$res[[1]]), regexp = NA)
  expect_warning(print(resLearn), regexp = NA)
  expect_warning(print(resLearn$res[[1]]), regexp = NA)
  
  
  expect_warning(resPredict <- mixtCompPredict(simData$dataPredict$matrix, model, algo, resLearn, nClass = 2, verbose = TRUE), regexp = NA)
  if(!is.null(resPredict$warnLog))
    print(resPredict$warnLog)
  
  expect_lte(norm(getTik(resPredict, log = FALSE) - getEmpiricTik(resPredict))/resPredict$algo$nInd, 0.1)
  expect_equal(names(resPredict), c("mixture", "variable", "algo"))
  expect_equal(resPredict$algo$mode, "predict")
})


test_that("summary returns no warnings and no errors", {
  outMC <- list(warnLog = "Crash.")
  
  expect_warning(summary(outMC), regexp = NA)
  expect_warning(print(outMC), regexp = NA)
})

test_that("summary works + run without paramStr for functional", {
  skip_on_cran()
  data("simData")
  simData$model$unsupervised$Functional1$paramStr = ""
  
  resLearn <- mixtCompLearn(simData$dataLearn$matrix, simData$model$unsupervised[-c(3:5, 7:8)], 
                            algo = createAlgo(nbBurnInIter = 25, nbIter = 25, nbGibbsBurnInIter = 25, nbGibbsIter = 25), 
                            nClass = 2, nRun = 1, nCore = 1, hierarchicalMode = "no") 
  
  expect_equal(resLearn$variable$param$Functional1$paramStr, "nSub: 2, nCoeff: 2")
  
  expect_warning(summary(resLearn), regexp = NA)
  expect_warning(summary(resLearn$res[[1]]), regexp = NA)
  expect_warning(print(resLearn), regexp = NA)
  expect_warning(print(resLearn$res[[1]]), regexp = NA)
})

Sys.unsetenv("MC_DETERMINISTIC")