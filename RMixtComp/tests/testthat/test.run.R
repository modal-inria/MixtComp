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


# @author Quentin Grimonprez, Vincent Kubicki
context("Run MixtComp")

# These tests are skipped on cran to keep elapsed time < 10min

Sys.setenv(MC_DETERMINISTIC = 42)

test_that("run cluster/predict R object", {
  skip_on_cran()
  set.seed(42, kind = "Mersenne-Twister", normal.kind = "Inversion")
  
  var <- list()
  var$z_class <- RMixtCompIO:::zParam()
  var$Poisson1 <- RMixtCompIO:::poissonParam("Poisson1")
  var$Gaussian1 <- RMixtCompIO:::gaussianParam("Gaussian1")
  var$Categorical1 <- RMixtCompIO:::categoricalParam1("Categorical1")
  var$nBinom1 <- RMixtCompIO:::nBinomParamRandom("nBinom1")
  var$Weibull1 <- RMixtCompIO:::weibullParam("Weibull1")
  var$Functional1 <- RMixtCompIO:::functionalInterPolyParam("Functional1")
  var$functionalSharedAlpha1 <- RMixtCompIO:::functionalSharedAlphaInterPolyParam("functionalSharedAlpha1")
  var$Rank1 <- RMixtCompIO:::rankParam("Rank1")
  
  resGenLearn <- RMixtCompIO:::dataGeneratorNewIO(200, 0.9, var)
  
  algoLearn <- list(
    nClass = 2,
    nInd = 200,
    nbBurnInIter = 50,
    nbIter = 50,
    nbGibbsBurnInIter = 50,
    nbGibbsIter = 50,
    nInitPerClass = 200,
    nSemTry = 20,
    confidenceLevel = 0.95,
    ratioStableCriterion = 0.95,
    nStableCriterion = 10,
    mode = "learn"
  )
  
  dataLearn <- resGenLearn$data
  desc <- resGenLearn$desc
  
  resLearn <- RMixtCompIO:::rmc(algoLearn, dataLearn, desc, list()) # run RMixtComp for clustering
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(RMixtCompIO:::rand.index(getPartition(resLearn), resGenLearn$z), 0.9)
  expect_lte(norm(getTik(resLearn, log = FALSE) - getEmpiricTik(resLearn))/resLearn$algo$nInd, 0.1)
  
  
  # test that getter functions do not return warnings
  expect_warning(getPartition(resLearn), regexp = NA)
  expect_warning(getBIC(resLearn), regexp = NA)
  expect_true(!is.na(getBIC(resLearn)))
  expect_warning(getICL(resLearn), regexp = NA)
  expect_true(!is.na(getICL(resLearn)))
  expect_warning(getPartition(resLearn), regexp = NA)
  expect_warning(getCompletedData(resLearn), regexp = NA)
  expect_equal(dim(getCompletedData(resLearn)), c(200, 6))
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
  expect_warning(model <- getModel(resLearn), regexp = NA)
  expect_equal(model, list(Poisson1 = list(type = "Poisson", paramStr = ""),
                           Gaussian1 = list(type = "Gaussian", paramStr = ""),
                           Categorical1 = list(type = "Multinomial", paramStr = "nModality: 4"),
                           nBinom1 = list(type = "NegativeBinomial", paramStr = ""),
                           Weibull1 = list(type = "Weibull", paramStr = ""),
                           Functional1 = list(type = "Func_CS", paramStr = "nSub: 2, nCoeff: 2"),
                           functionalSharedAlpha1 = list(type = "Func_SharedAlpha_CS", paramStr = "nSub: 2, nCoeff: 2"),
                           Rank1 = list(type = "Rank_ISR", paramStr = "nModality: 4")))
  
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
  expect_warning(histMisclassif(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(histMisclassif(resLearn, pkg = "plotly"), regexp = NA)
  
  for(name in getVarNames(resLearn))
  {
    expect_warning(plotParamConvergence(resLearn, name), regexp = NA)
    
    if(resLearn$variable$type[[name]] != "Rank_ISR")
    {
      expect_warning(plotDataCI(resLearn, name, pkg = "plotly"), regexp = NA)
      expect_warning(plotDataCI(resLearn, name, pkg = "ggplot2"), regexp = NA)
      expect_warning(plotDataBoxplot(resLearn, name, pkg = "plotly"), regexp = NA)
      expect_warning(plotDataBoxplot(resLearn, name, pkg = "ggplot2"), regexp = NA)
    }else{
      expect_warning(plotDataCI(resLearn, name, pkg = "plotly"))
      expect_warning(plotDataCI(resLearn, name, pkg = "ggplot2"))
      expect_warning(plotDataBoxplot(resLearn, name, pkg = "plotly"))
      expect_warning(plotDataBoxplot(resLearn, name, pkg = "ggplot2"))
    }
  }
  expect_warning(plotProportion(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(plotProportion(resLearn, pkg = "plotly"), regexp = NA)
  class(resLearn) = "MixtComp"
  expect_warning(plot(resLearn, pkg = "ggplot2"), regexp = NA)
  expect_warning(plot(resLearn, pkg = "plotly"), regexp = NA)
  unlink("Rplots.pdf")
  
  resGenPredict <- RMixtCompIO:::dataGeneratorNewIO(200, 0.95, var)
  
  algoPredict <- list(
    nClass = 2,
    nInd = 200,
    nbBurnInIter = 50,
    nbIter = 50,
    nbGibbsBurnInIter = 50,
    nbGibbsIter = 50,
    nInitPerClass = 100,
    nSemTry = 20,
    confidenceLevel = 0.95,
    ratioStableCriterion = 0.95,
    nStableCriterion = 10,
    mode = "predict"
  )
  
  dataPredict <- resGenPredict$data
  
  resPredict <- RMixtCompIO:::rmc(algoPredict, dataPredict, desc, resLearn)
  
  expect_equal(resPredict$warnLog, NULL)
  if(!is.null(resPredict$warnLog))
    print(resPredict$warnLog)
  expect_gte(RMixtCompIO:::rand.index(getPartition(resPredict), resGenPredict$z), 0.85)
  expect_lte(norm(getTik(resPredict, log = FALSE) - getEmpiricTik(resPredict))/resPredict$algo$nInd, 0.1)
  
})

Sys.unsetenv("MC_DETERMINISTIC")
