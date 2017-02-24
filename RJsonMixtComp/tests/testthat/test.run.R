context("Test fonctionnel")

test_that("Learn + predict", {
  # skip("On le skip cette fois")
  
  pathToData <- system.file("extdata", "data2.csv", package = "RJsonMixtComp")
  pathToDescriptor <- system.file("extdata", "descriptor2.csv", package = "RJsonMixtComp")

  resGetData <- RJsonMixtComp:::getData(c(pathToData, pathToDescriptor)) 
  
  
  resLearn <- JsonMixtCompCluster(dataList = resGetData$lm, mcStrategy = list(nbBurnInIter = 100, 
                                                                              nbIter = 100, 
                                                                              nbGibbsBurnInIter = 100, 
                                                                              nbGibbsIter = 100, 
                                                                              parameterEdgeAuthorized = TRUE),
                                  nbClass = 1, confidenceLevel = 0.95, jsonInputFile = "datalearn.json", jsonOutputFile = "reslearn.json")
  
  
  resPredict <- JsonMixtCompPredict(dataList = resGetData$lm, mcStrategy = list(nbBurnInIter = 100, 
                                                                                nbIter = 100, 
                                                                                nbGibbsBurnInIter = 100, 
                                                                                nbGibbsIter = 100, 
                                                                                parameterEdgeAuthorized = TRUE),
                                    nbClass = 1, confidenceLevel = 0.95, jsonInputFile = "datalearn.json", jsonOutputFile = "respredict.json",
                                    jsonMixtCompLearnFile = "reslearn.json")
  
  
  file.remove(c("reslearn.json", "respredict.json", "datalearn.json"))
})
