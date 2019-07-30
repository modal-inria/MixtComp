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
 

# @author Quentin Grimonprez, Vincent Kubicki
context("Run MixtComp")

Sys.setenv(MC_DETERMINISTIC = 42)

getTik <- function(outMixtComp, log = TRUE){
  logTik <- sweep(outMixtComp$mixture$lnProbaGivenClass, 
                  1, apply(outMixtComp$mixture$lnProbaGivenClass, 1, function(vec) (max(vec) + log(sum(exp(vec - max(vec)))))),
                  "-")
  if(!log)
    return(exp(logTik))
  
  return(logTik)
}


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
    ratioStableCriterion = 0.95,
    nStableCriterion = 10,
    mode = "learn"
  )

  data <- resGen$data
  desc <- resGen$desc

  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering

  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)

  expect_equal(resLearn$warnLog, NULL)
  partition <- resLearn$variable$data$z_class$completed
  expect_gte(rand.index(partition, resGen$z), 0.9)
  empiricTik <- resLearn$variable$data$z_class$stat
  expect_lte(norm(getTik(resLearn, log = FALSE) - empiricTik)/resLearn$algo$nInd, 0.1)
  
  # confMatSampled <- table(resGen$z, partition)
  # print(confMatSampled)
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
    ratioStableCriterion = 0.95,
    nStableCriterion = 10,
    mode = "learn"
  )

  data <- resGen$data
  desc <- resGen$desc

  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering

  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)

  expect_equal(resLearn$warnLog, NULL)
  partition <- resLearn$variable$data$z_class$completed
  expect_gte(rand.index(partition, resGen$z), 0.9)
  empiricTik <- resLearn$variable$data$z_class$stat
  expect_lte(norm(getTik(resLearn, log = FALSE) - empiricTik)/resLearn$algo$nInd, 0.1)
  
  # confMatSampled <- table(resGen$z, partition)
  # print(confMatSampled)
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
    ratioStableCriterion = 0.95,
    nStableCriterion = 10,
    mode = "learn"
  )

  data <- resGen$data
  desc <- resGen$desc

  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering

  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)

  expect_equal(resLearn$warnLog, NULL)
  partition <- resLearn$variable$data$z_class$completed
  expect_gte(rand.index(partition, resGen$z), 0.9)
  empiricTik <- resLearn$variable$data$z_class$stat
  expect_lte(norm(getTik(resLearn, log = FALSE) - empiricTik)/resLearn$algo$nInd, 0.1)
  
  # confMatSampled <- table(resGen$z, partition)
  # print(confMatSampled)
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
    ratioStableCriterion = 0.95,
    nStableCriterion = 10,
    mode = "learn"
  )

  data <- resGen$data
  desc <- resGen$desc

  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering

  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)

  expect_equal(resLearn$warnLog, NULL)
  partition <- resLearn$variable$data$z_class$completed
  expect_gte(rand.index(partition, resGen$z), 0.9)
  empiricTik <- resLearn$variable$data$z_class$stat
  expect_lte(norm(getTik(resLearn, log = FALSE) - empiricTik)/resLearn$algo$nInd, 0.1)
  
  # confMatSampled <- table(resGen$z, partition)
  # print(confMatSampled)
})

test_that("weibull model works",{
  set.seed(42)

  nInd <- 1200
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
    nInitPerClass = 300,
    nSemTry = 20,
    confidenceLevel = 0.95,
    ratioStableCriterion = 0.95,
    nStableCriterion = 10,
    mode = "learn"
  )

  data <- resGen$data
  desc <- resGen$desc

  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering

  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)

  expect_equal(resLearn$warnLog, NULL)
  partition <- resLearn$variable$data$z_class$completed
  expect_gte(rand.index(partition, resGen$z), 0.8)
  empiricTik <- resLearn$variable$data$z_class$stat
  expect_lte(norm(getTik(resLearn, log = FALSE) - empiricTik)/resLearn$algo$nInd, 0.1)
  
  # confMatSampled <- table(resGen$z, partition)
  # print(confMatSampled)
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
    ratioStableCriterion = 0.95,
    nStableCriterion = 10,
    mode = "learn"
  )

  data <- resGen$data
  desc <- resGen$desc

  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering

  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)

  expect_equal(resLearn$warnLog, NULL)
  partition <- resLearn$variable$data$z_class$completed
  expect_gte(rand.index(partition, resGen$z), 0.9)
  empiricTik <- resLearn$variable$data$z_class$stat
  expect_lte(norm(getTik(resLearn, log = FALSE) - empiricTik)/resLearn$algo$nInd, 0.1)
  
  # confMatSampled <- table(resGen$z, partition)
  # print(confMatSampled)
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
    ratioStableCriterion = 0.95,
    nStableCriterion = 10,
    mode = "learn"
  )

  data <- resGen$data
  desc <- resGen$desc

  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering

  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)

  expect_equal(resLearn$warnLog, NULL)
  partition <- resLearn$variable$data$z_class$completed
  expect_gte(rand.index(partition, resGen$z), 0.9)
  empiricTik <- resLearn$variable$data$z_class$stat
  expect_lte(norm(getTik(resLearn, log = FALSE) - empiricTik)/resLearn$algo$nInd, 0.1)
  
  # confMatSampled <- table(resGen$z, partition)
  # print(confMatSampled)
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
    ratioStableCriterion = 0.95,
    nStableCriterion = 10,
    mode = "learn"
  )

  data <- resGen$da
  desc <- resGen$desc

  resLearn <- rmc(algo, data, desc, list()) # run RMixtCompt for clustering

  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)

  expect_equal(resLearn$warnLog, NULL)
  partition <- resLearn$variable$data$z_class$completed
  expect_gte(rand.index(partition, resGen$z), 0.8)
  empiricTik <- resLearn$variable$data$z_class$stat
  expect_lte(norm(getTik(resLearn, log = FALSE) - empiricTik)/resLearn$algo$nInd, 0.1)
  
  # confMatSampled <- table(resGen$z, partition)
  # print(confMatSampled)
})

Sys.unsetenv("MC_DETERMINISTIC")
