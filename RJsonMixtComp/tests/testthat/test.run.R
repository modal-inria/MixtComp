context("Test fonctionnel")

test_that("Learn + predict", {
  pathToData <- system.file("extdata", "data.json", package = "RJsonMixtComp")
  pathToDescriptor <- system.file("extdata", "desc.json", package = "RJsonMixtComp")
  
  data <- as.data.frame(fromJSON(pathToData))
  descriptor <- as.data.frame(lapply(fromJSON(pathToDescriptor), unlist))
  
  expect_error(resLearn <- JsonMixtCompLearn(data, descriptor, nClass = 2, mcStrategy = list(nbBurnInIter = 50,
                                                                                           nbIter = 50,
                                                                                           nbGibbsBurnInIter = 20,
                                                                                           nbGibbsIter = 20,
                                                                                           nInitPerClass = 10,
                                                                                           nSemTry = 5),
                                             confidenceLevel = 0.95, inputPath = ".", outputFile = "reslearn.json"), regexp = NA)
  
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./descriptor.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  
  file.remove("./algo.json", "./descriptor.json", "./data.json")
 
  expect_error(resPredict <- JsonMixtCompPredict(data, descriptor, nClass = 2, mcStrategy = list(nbBurnInIter = 50,
                                                                                                 nbIter = 50,
                                                                                                 nbGibbsBurnInIter = 20,
                                                                                                 nbGibbsIter = 20,
                                                                                                 nInitPerClass = 10,
                                                                                                 nSemTry = 5),
                                                 confidenceLevel = 0.95, inputPath = ".", paramFile = "reslearn.json", outputFile = "respredict.json"), regexp = NA)
  
  
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./descriptor.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  expect_true(file.exists("respredict.json"))
  
  file.remove("./algo.json", "./descriptor.json", "./data.json", "reslearn.json", "respredict.json", "progress")
})

test_that("can predict with only one sample in the data set", {
  pathToData <- system.file("extdata", "data.json", package = "RJsonMixtComp")
  pathToDescriptor <- system.file("extdata", "desc.json", package = "RJsonMixtComp")
  
  data <- as.data.frame(fromJSON(pathToData))
  descriptor <- as.data.frame(lapply(fromJSON(pathToDescriptor), unlist))
  
  expect_error(resLearn <- JsonMixtCompLearn(data, descriptor, nClass = 2, mcStrategy = list(nbBurnInIter = 50,
                                                                                             nbIter = 50,
                                                                                             nbGibbsBurnInIter = 20,
                                                                                             nbGibbsIter = 20,
                                                                                             nInitPerClass = 10,
                                                                                             nSemTry = 5),
                                             confidenceLevel = 0.95, inputPath = ".", outputFile = "reslearn.json"), regexp = NA)

  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./descriptor.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  
  file.remove("./algo.json", "./descriptor.json", "./data.json")
  
  # keep only one sample
  data = data[1,]
  
  expect_error(resPredict <- JsonMixtCompPredict(data, descriptor, nClass = 2, mcStrategy = list(nbBurnInIter = 50,
                                                                                                 nbIter = 50,
                                                                                                 nbGibbsBurnInIter = 20,
                                                                                                 nbGibbsIter = 20,
                                                                                                 nInitPerClass = 10,
                                                                                                 nSemTry = 5),
                                                 confidenceLevel = 0.95, inputPath = ".", paramFile = "reslearn.json", outputFile = "respredict.json"), regexp = NA)
  
  
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./descriptor.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  expect_true(file.exists("respredict.json"))
  
  file.remove("./algo.json", "./descriptor.json", "./data.json", "reslearn.json", "respredict.json", "progress")
})
