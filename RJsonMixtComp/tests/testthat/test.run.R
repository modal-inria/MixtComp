context("Test fonctionnel")

test_that("Learn + predict", {
  pathToData <- system.file("extdata", "data2.csv", package = "RJsonMixtComp")
  pathToDescriptor <- system.file("extdata", "descriptor2.csv", package = "RJsonMixtComp")
  
  resGetData <- RJsonMixtComp:::getData(c(pathToData, pathToDescriptor)) 
  
  
  expect_error(resLearn <- JsonMixtCompCluster(dataList = resGetData$lm, mcStrategy = list(nbBurnInIter = 100, 
                                                                                           nbIter = 100, 
                                                                                           nbGibbsBurnInIter = 100, 
                                                                                           nbGibbsIter = 100, 
                                                                                           parameterEdgeAuthorized = TRUE),
                                               nbClass = 1, confidenceLevel = 0.95, jsonInputFile = "datalearn.json", jsonOutputFile = "reslearn.json"), regexp = NA)
  
  
  expect_error(resPredict <- JsonMixtCompPredict(dataList = resGetData$lm, mcStrategy = list(nbBurnInIter = 100, 
                                                                                             nbIter = 100, 
                                                                                             nbGibbsBurnInIter = 100, 
                                                                                             nbGibbsIter = 100, 
                                                                                             parameterEdgeAuthorized = TRUE),
                                                 nbClass = 1, confidenceLevel = 0.95, jsonInputFile = "datalearn.json", jsonOutputFile = "respredict.json",
                                                 jsonMixtCompLearnFile = "reslearn.json"), regexp = NA)
  
  
  file.remove(c("reslearn.json", "respredict.json", "datalearn.json"))
})

test_that("can predict with only one sample in the data set", {

  pathToData <- system.file("extdata", "data2.csv", package = "RJsonMixtComp")
  pathToDescriptor <- system.file("extdata", "descriptor2.csv", package = "RJsonMixtComp")

  resGetData <- RJsonMixtComp:::getData(c(pathToData, pathToDescriptor))


  resLearn <- JsonMixtCompCluster(dataList = resGetData$lm, mcStrategy = list(nbBurnInIter = 100,
                                                                              nbIter = 100,
                                                                              nbGibbsBurnInIter = 100,
                                                                              nbGibbsIter = 100,
                                                                              parameterEdgeAuthorized = TRUE),
                                  nbClass = 1, confidenceLevel = 0.95, jsonInputFile = "datalearn.json", jsonOutputFile = "reslearn.json")



  # keep only one sample
  for(i in 1:length(resGetData$lm))
  {
    resGetData$lm[[i]]$data = resGetData$lm[[i]]$data[1]
  }

  expect_error(resPredict <- JsonMixtCompPredict(dataList = resGetData$lm, mcStrategy = list(nbBurnInIter = 100,
                                                                                             nbIter = 100,
                                                                                             nbGibbsBurnInIter = 100,
                                                                                             nbGibbsIter = 100,
                                                                                             parameterEdgeAuthorized = TRUE),
                                                 nbClass = 1, confidenceLevel = 0.95, jsonInputFile = "datalearn.json", jsonOutputFile = "respredict.json",
                                                 jsonMixtCompLearnFile = "reslearn.json"), regexp = NA)


  file.remove(c("reslearn.json", "respredict.json", "datalearn.json"))
})
