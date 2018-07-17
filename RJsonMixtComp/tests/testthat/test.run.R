context("Test fonctionnel")

test_that("Learn + predict", {
  pathToData <- system.file("extdata", "data.csv", package = "RJsonMixtComp")
  pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RJsonMixtComp")
  
  resGetData <- RJsonMixtComp:::getData(c(pathToData, pathToDescriptor))
  
  resGetData$lm[[5]]$paramStr = "nSub: 3, nCoeff: 4"
  expect_error(resLearn <- JsonMixtCompCluster(dataList = resGetData$lm, mcStrategy = list(nbBurnInIter = 50,
                                                                                           nbIter = 50,
                                                                                           nbGibbsBurnInIter = 20,
                                                                                           nbGibbsIter = 20,
                                                                                           nInitPerClass = 10,
                                                                                           nSemTry = 5),
                                               nbClass = 2, confidenceLevel = 0.95, jsonInputFile = "datalearn.json", jsonOutputFile = "reslearn.json"), regexp = NA)
  
  
  expect_true(is.matrix(resLearn$variable$param$Gaussian1$NumericalParam$stat))
  
  expect_error(resPredict <- JsonMixtCompPredict(dataList = resGetData$lm, mcStrategy = list(nbBurnInIter = 50,
                                                                                             nbIter = 50,
                                                                                             nbGibbsBurnInIter = 20,
                                                                                             nbGibbsIter = 20,
                                                                                             nInitPerClass = 10,
                                                                                             nSemTry = 5),
                                                 nbClass = 2, confidenceLevel = 0.95, jsonInputFile = "datalearn.json", jsonOutputFile = "respredict.json",
                                                 jsonMixtCompLearnFile = "reslearn.json"), regexp = NA)
  
  
  file.remove(c("reslearn.json", "respredict.json", "datalearn.json", "progress"))
})

test_that("can predict with only one sample in the data set", {
  pathToData <- system.file("extdata", "data.csv", package = "RJsonMixtComp")
  pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RJsonMixtComp")
  
  resGetData <- RJsonMixtComp:::getData(c(pathToData, pathToDescriptor))
  
  
  resLearn <- JsonMixtCompCluster(dataList = resGetData$lm, mcStrategy = list(nbBurnInIter = 50,
                                                                              nbIter = 50,
                                                                              nbGibbsBurnInIter = 20,
                                                                              nbGibbsIter = 20,
                                                                              nInitPerClass = 10,
                                                                              nSemTry = 5),
                                  nbClass = 1, confidenceLevel = 0.95, jsonInputFile = "datalearn.json", jsonOutputFile = "reslearn.json")
  
  
  
  # keep only one sample
  for(i in 1:length(resGetData$lm))
  {
    resGetData$lm[[i]]$data = resGetData$lm[[i]]$data[1]
  }
  
  expect_error(resPredict <- JsonMixtCompPredict(dataList = resGetData$lm, mcStrategy = list(nbBurnInIter = 50,
                                                                                             nbIter = 50,
                                                                                             nbGibbsBurnInIter = 20,
                                                                                             nbGibbsIter = 20,
                                                                                             nInitPerClass = 10,
                                                                                             nSemTry = 5),
                                                 nbClass = 1, confidenceLevel = 0.95, jsonInputFile = "datalearn.json", jsonOutputFile = "respredict.json",
                                                 jsonMixtCompLearnFile = "reslearn.json"), regexp = NA)
  
  
  file.remove(c("reslearn.json", "respredict.json", "datalearn.json", "progress"))
})
