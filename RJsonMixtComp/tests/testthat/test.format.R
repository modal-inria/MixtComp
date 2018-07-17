context("Test format")

test_that("conversion of IDclass", {
  pathToResult <- system.file("extdata", "reslearn.json", package = "RJsonMixtComp")

  jsonFile <- fromJSON(pathToResult)

  out <- convertIDClass(jsonFile$mixture$IDClass)
  
  expect_true(is.matrix(out))
  expect_equal(ncol(out), length(jsonFile$variable$type)-1)
  expect_equal(nrow(out), jsonFile$mixture$nbCluster)
})


test_that("data stat colnames", {
  pathToResult <- system.file("extdata", "reslearn.json", package = "RJsonMixtComp")
  
  jsonFile <- fromJSON(pathToResult)
  
  out <- convertDataStat(jsonFile$variable$data$Gaussian1$stat, "Gaussian_sjk", 0.95)
  expect_equal(colnames(out), c("indice", "median", "q 2.5%", "q 97.5%"))
  out <- convertDataStat(jsonFile$variable$data$Poisson1$stat, "Poisson_k", 0.95)
  expect_equal(colnames(out), c("indice", "median", "q 2.5%", "q 97.5%"))
  out <- convertDataStat(jsonFile$variable$data$Weibull1$stat, "Weibull", 0.95)
  expect_equal(colnames(out), c("indice", "median", "q 2.5%", "q 97.5%"))
  out <- convertDataStat(jsonFile$variable$data$z_class$stat, "LatentClass", 0.95)
  expect_equal(colnames(out), paste0("k: ", 1:jsonFile$mixture$nbCluster))

})


test_that("rownames for functional param", {

  out <- nomRowParamFunctional("alpha", 2, 3, 4)
  expect_length(out, 2*3*2)
  expect_equal(out, c("k: 0, s: 0, alpha0", "k: 0, s: 0, alpha1", "k: 0, s: 1, alpha0", "k: 0, s: 1, alpha1", "k: 0, s: 2, alpha0", "k: 0, s: 2, alpha1", "k: 1, s: 0, alpha0", "k: 1, s: 0, alpha1", "k: 1, s: 1, alpha0",
                      "k: 1, s: 1, alpha1", "k: 1, s: 2, alpha0", "k: 1, s: 2, alpha1"))
  out <- nomRowParamFunctional("beta", 2, 3, 4)
  expect_length(out, 2*3*4)
  expect_equal(out, c("k: 0, s: 0, c: 0", "k: 0, s: 0, c: 1", "k: 0, s: 0, c: 2", "k: 0, s: 0, c: 3", "k: 0, s: 1, c: 0", "k: 0, s: 1, c: 1", "k: 0, s: 1, c: 2", "k: 0, s: 1, c: 3", "k: 0, s: 2, c: 0", "k: 0, s: 2, c: 1", "k: 0, s: 2, c: 2",
                      "k: 0, s: 2, c: 3", "k: 1, s: 0, c: 0", "k: 1, s: 0, c: 1", "k: 1, s: 0, c: 2", "k: 1, s: 0, c: 3", "k: 1, s: 1, c: 0", "k: 1, s: 1, c: 1", "k: 1, s: 1, c: 2", "k: 1, s: 1, c: 3", "k: 1, s: 2, c: 0", "k: 1, s: 2, c: 1", 
                      "k: 1, s: 2, c: 2", "k: 1, s: 2, c: 3"))
  out <- nomRowParamFunctional("sd", 2, 3, 4)
  expect_length(out, 2*3)
  expect_equal(out, c("k: 0, s: 0", "k: 0, s: 1", "k: 0, s: 2", "k: 1, s: 0", "k: 1, s: 1", "k: 1, s: 2"))
  
})


test_that("rownames for categorical param", {
  pathToResult <- system.file("extdata", "reslearn.json", package = "RJsonMixtComp")
  
  jsonFile <- fromJSON(pathToResult)
  
  param <- jsonFile$variable$param$Categorical1
  param$NumericalParam$stat = do.call(cbind, param$NumericalParam$stat)
  
  out <- nomRowParamCateg(param)
  expect_equal(out, c("k: 1, modality: 1", "k: 1, modality: 2", "k: 1, modality: 3", "k: 1, modality: 4", "k: 2, modality: 1", "k: 2, modality: 2", "k: 2, modality: 3", "k: 2, modality: 4"))
})


test_that("convertParam", {
  pathToResult <- system.file("extdata", "reslearn.json", package = "RJsonMixtComp")
  
  jsonFile <- fromJSON(pathToResult)
  
  nbClass <- jsonFile$mixture$nbCluster
  nCoeff = nSub <- 2
  nModality <- 4
  
  out <- convertParam(jsonFile$variable$param$Categorical1, "Categorical_pjk", nbClass, nCoeff, nSub, mode = "learn")
  expect_equal(dim(out$NumericalParam$log), c(nbClass * nModality, jsonFile$strategy$nbIter))
  expect_true(is.matrix(out$NumericalParam$stat))
  expect_equal(dim(out$NumericalParam$stat), c(nbClass * nModality, 3))
  expect_equal(rownames(out$NumericalParam$stat), c("k: 1, modality: 1", "k: 1, modality: 2", "k: 1, modality: 3", "k: 1, modality: 4", "k: 2, modality: 1", "k: 2, modality: 2", "k: 2, modality: 3", "k: 2, modality: 4"))
  
  out <- convertParam(jsonFile$variable$param$Gaussian1, "Gaussian_sjk", nbClass, nCoeff, nSub, mode = "learn")
  expect_equal(dim(out$NumericalParam$log), c(nbClass * 2, jsonFile$strategy$nbIter))
  expect_true(is.matrix(out$NumericalParam$stat))
  expect_equal(dim(out$NumericalParam$stat), c(nbClass * 2, 3))
  expect_equal(rownames(out$NumericalParam$stat), c("k: 1, mean:", "k: 1, sd", "k: 2, mean:", "k: 2, sd"))
  
  out <- convertParam(jsonFile$variable$param$Weibull1, "Weibull", nbClass, nCoeff, nSub, mode = "learn")
  expect_equal(dim(out$NumericalParam$log), c(nbClass * 2, jsonFile$strategy$nbIter))
  expect_true(is.matrix(out$NumericalParam$stat))
  expect_equal(dim(out$NumericalParam$stat), c(nbClass * 2, 3))
  expect_equal(rownames(out$NumericalParam$stat), c("k: 1, k:", "k: 1, lambda", "k: 2, k:", "k: 2, lambda"))
  
  out <- convertParam(jsonFile$variable$param$Poisson1, "Poisson_k", nbClass, nCoeff, nSub, mode = "learn")
  expect_equal(dim(out$NumericalParam$log), c(nbClass, jsonFile$strategy$nbIter))
  expect_true(is.matrix(out$NumericalParam$stat))
  expect_equal(dim(out$NumericalParam$stat), c(nbClass, 3))
  expect_equal(rownames(out$NumericalParam$stat), c("k: 1, lambda", "k: 2, lambda"))

  out <- convertParam(jsonFile$variable$param$Functional1, "Functional", nbClass, nCoeff, nSub, mode = "learn")
  expect_equal(dim(out$alpha$log), c(2 * nbClass * nSub, jsonFile$strategy$nbIter))
  expect_equal(dim(out$beta$log), c(nCoeff * nbClass * nSub, jsonFile$strategy$nbIter))
  expect_equal(dim(out$sd$log), c(nSub * nbClass, jsonFile$strategy$nbIter))
  expect_true(is.matrix(out$alpha$stat))
  expect_true(is.matrix(out$beta$stat))
  expect_true(is.matrix(out$sd$stat))
  expect_equal(dim(out$alpha$stat), c(2 * nbClass * nSub, 3))
  expect_equal(dim(out$beta$stat), c(nCoeff * nbClass * nSub, 3))
  expect_equal(dim(out$sd$stat), c(nbClass * nSub, 3))
  expect_equal(rownames(out$alpha$stat), c("k: 0, s: 0, alpha0", "k: 0, s: 0, alpha1", "k: 0, s: 1, alpha0", "k: 0, s: 1, alpha1", "k: 1, s: 0, alpha0", "k: 1, s: 0, alpha1", "k: 1, s: 1, alpha0", "k: 1, s: 1, alpha1"))
  expect_equal(rownames(out$beta$stat), c("k: 0, s: 0, c: 0", "k: 0, s: 0, c: 1", "k: 0, s: 1, c: 0", "k: 0, s: 1, c: 1", "k: 1, s: 0, c: 0", "k: 1, s: 0, c: 1", "k: 1, s: 1, c: 0", "k: 1, s: 1, c: 1"))
  expect_equal(rownames(out$sd$stat), c("k: 0, s: 0", "k: 0, s: 1", "k: 1, s: 0", "k: 1, s: 1"))
  
})


  