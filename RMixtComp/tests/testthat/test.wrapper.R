context("Wrappers of rmc function")

Sys.setenv(MC_DETERMINISTIC = 42)

test_that("formatDesc does not change well formated data", {
  desc <- list(var1 = list(type = "Gaussian", paramStr = ""),
               var2 = list(type = "CorReg", paramStr = "ouais"),
               var3 = list(type = "Multinomial", paramStr = "CorReg"))
  
  outDesc <- formatDesc(desc)
  expect_equal(outDesc, desc)
})


test_that("formatDesc adds paramStr when missing", {
  desc <- list(var1 = list(type = "Gaussian"),
               var2 = list(type = "CorReg", paramStr = "ouais"),
               var3 = list(type = "Multinomial"))
  
  outDesc <- formatDesc(desc)
  expect_equal(outDesc$var2, desc$var2)
  expect_equal(outDesc$var1, list(type = "Gaussian", paramStr = ""))
  expect_equal(outDesc$var3, list(type = "Multinomial", paramStr = ""))
  
})


test_that("formatDesc puts type in a list format", {
  desc <- list(var1 = "Gaussian",
               var2 = list(type = "CorReg", paramStr = "ouais"),
               var3 = "Multinomial")
  
  outDesc <- formatDesc(desc)
  expect_equal(outDesc$var1, list(type = "Gaussian", paramStr = ""))
  expect_equal(outDesc$var2, desc$var2)
  expect_equal(outDesc$var3, list(type = "Multinomial", paramStr = ""))
  
})


test_that("formatData converts data.frame into a list format", {
  dat <- data.frame(x1 = 1:10, x2 = 10:1)
  dataOut <- formatData(dat)
  
  expect_equal(names(dataOut), colnames(dat))
  expect_equivalent(sapply(dataOut, length), rep(nrow(dat), ncol(dat)))
  expect_true(all(sapply(dataOut, is.character)))
})

test_that("formatData converts matrix into a list format", {
  dat <- matrix(c(1:10, 10:1), ncol = 2, dimnames = list(NULL, c("x1", "x2")))
  dataOut <- formatData(dat)
  
  expect_equal(names(dataOut), colnames(dat))
  expect_equivalent(sapply(dataOut, length), rep(nrow(dat), ncol(dat)))
  expect_true(all(sapply(dataOut, is.character)))
})

test_that("formatData keeps list in list format", { 
  dat <- list(x1 = 1:10, x2 = 10:1)
  dataOut <- formatData(dat)
  
  expect_true(is.list(dataOut))
  expect_equal(names(dataOut), names(dat))
  expect_equal(class(dataOut$x1), "character")
  expect_equal(class(dataOut$x2), "character")
})

test_that("checkNClass works with mixtComp object", {
  resLearn <- list(algo = list(nClass = 2))
  class(resLearn) = "MixtComp"
  
  nClass <- NULL
  expect_warning(out <- checkNClass(nClass, resLearn), regexp = NA)
  expect_equal(out, 2)
  
  nClass <- 3
  expect_warning(out <- checkNClass(nClass, resLearn))
  expect_equal(out, 2)
  
  nClass <- 2:4
  expect_warning(out <- checkNClass(nClass, resLearn))
  expect_equal(out, 2)
  
  nClass <- 3:4
  expect_warning(out <- checkNClass(nClass, resLearn))
  expect_equal(out, 2)
})

test_that("checkNClass works with mixtCompLearn object", {
  resLearn <- list(algo = list(nClass = 2), nClass = 2:5)
  class(resLearn) = c("MixtCompLearn", "MixtComp")
  
  nClass <- NULL
  expect_warning(out <- checkNClass(nClass, resLearn), regexp = NA)
  expect_equal(out, 2)
  
  nClass <- 3
  expect_warning(out <- checkNClass(nClass, resLearn), regexp = NA)
  expect_equal(out, 3)
  
  nClass <- 3:4
  expect_warning(out <- checkNClass(nClass, resLearn))
  expect_equal(out, 3)
  
  nClass <- 6:8
  expect_warning(out <- checkNClass(nClass, resLearn))
  expect_equal(out, 2)
})



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
    mode = "learn",
    nInd = 1000
  )
  
  resLearn <- rmcMultiRun(algo, resGen$data, resGen$desc, list(), nRun = 1) 
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  
  expect_equal(names(resLearn), c("mixture", "variable", "algo"))
  
  
  resLearn <- rmcMultiRun(algo, resGen$data, resGen$desc, list(), nRun = 5) 
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  
  expect_equal(names(resLearn), c("mixture", "variable", "algo"))
  
})



test_that("mixtCompLearn works + mixtCompPredict", {
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
    confidenceLevel = 0.95
  )
  
  data <- do.call(cbind, resGen$data)
  desc <- list(z_class = "LatentClass", Gaussian1 = "Gaussian")
  
  resLearn <- mixtCompLearn(data, desc, algo, nClass = 4, crit = "ICL") 
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(rand.index(getPartition(resLearn), resGen$z), 0.85)
  
  confMatSampled <- table(resGen$z, getPartition(resLearn))
  print(confMatSampled)
  
  expect_equal(names(resLearn), c("mixture", "variable", "algo", "nRun", "criterion", "crit", "nClass", "res"))
  expect_equal(resLearn$criterion, "ICL")
  expect_equal(resLearn$nRun, 1)
  expect_equal(dim(resLearn$crit), c(2, 1))
  expect_equal(resLearn$nClass, 4)
  expect_equal(resLearn$algo$mode, "learn")
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
  

  expect_warning(resPredict <- mixtCompPredict(data, desc, algo, resLearn), regexp = NA)
  expect_equal(names(resPredict), c("mixture", "variable", "algo"))
  expect_equal(resPredict$algo$mode, "predict")
})

test_that("mixtCompLearn works with a vector for nClass + mixtCompPredict", {
  set.seed(42)
  
  nInd <- 1000
  
  var <- list()
  var$z_class <- zParam()
  var$z_class$param <- c(0.2, 0.3, 0.15, 0.35)
  var$Gaussian1 <- gaussianParam("Gaussian1")
  var$Gaussian1$param[[3]] <- list(mean = -2, sd = 0.5)
  var$Gaussian1$param[[4]] <- list(mean = 2, sd = 0.5)
  
  resGen <- dataGeneratorNewIO(nInd, 0.9, var)
  
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
  
  resLearn <- mixtCompLearn(data, desc, algo, nClass = 2:5, nRun = 2) 
  
  if(!is.null(resLearn$warnLog))
    print(resLearn$warnLog)
  
  expect_equal(resLearn$warnLog, NULL)
  expect_gte(rand.index(getPartition(resLearn), resGen$z), 0.85)
  
  confMatSampled <- table(resGen$z, getPartition(resLearn))
  print(confMatSampled)
  
  expect_equal(names(resLearn), c("mixture", "variable", "algo", "nRun", "criterion", "crit", "nClass", "res"))
  expect_equal(resLearn$nRun, 2)
  expect_equal(resLearn$criterion, "BIC")
  expect_equal(dim(resLearn$crit), c(2, 4))
  expect_equal(resLearn$nClass, 2:5)
  expect_equal(resLearn$algo$mode, "learn")
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
      expect_warning(plotDataBoxplot(resLearn, name), regexp = NA)
    }else{
      expect_warning(plotDataCI(resLearn, name, pkg = "ggplot2"))
      expect_warning(plotDataCI(resLearn, name, pkg = "plotly"))
      expect_warning(plotDataBoxplot(resLearn, name))
    }
  }
  plotProportion(resLearn, pkg = "ggplot2")
  plotProportion(resLearn, pkg = "plotly")
  expect_warning(plot(resLearn$res[[3]], pkg = "ggplot2"), regexp = NA)
  expect_warning(plot(resLearn$res[[3]], pkg = "plotly"), regexp = NA)
  file.remove("Rplots.pdf")
  
  expect_warning(resPredict <- mixtCompPredict(data, desc, algo, resLearn, nClass = 3), regexp = NA)
  expect_warning(summary(resLearn), regexp = NA)
  expect_warning(summary(resLearn$res[[1]]), regexp = NA)
  expect_warning(print(resLearn), regexp = NA)
  expect_warning(print(resLearn$res[[1]]), regexp = NA)

  if(!is.null(resPredict$warnLog))
    print(resPredict$warnLog)

  expect_equal(names(resPredict), c("mixture", "variable", "algo"))
  expect_equal(resPredict$algo$mode, "predict")
})

test_that("summary returns no warnings and no errors", {
  outMC <- list(warnLog = "Crash.")
  
  expect_warning(summary(outMC), regexp = NA)
  expect_warning(print(outMC), regexp = NA)
})

test_that("summary works", {
  data("simData")
  resLearn <- mixtCompLearn(simDataLearn$matrix, simDesc$unsupervised, algo = createAlgo(), nClass = 2) 
  
  expect_warning(summary(resLearn), regexp = NA)
  expect_warning(summary(resLearn$res[[1]]), regexp = NA)
  expect_warning(print(resLearn), regexp = NA)
  expect_warning(print(resLearn$res[[1]]), regexp = NA)
})

Sys.unsetenv("MC_DETERMINISTIC")
