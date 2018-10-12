context("Test fonctionnel")

test_that("Learn + predict", {
  pathToData <- system.file("extdata", "data.json", package = "RJMixtComp")
  pathToDescriptor <- system.file("extdata", "desc.json", package = "RJMixtComp")
  
  data <- as.data.frame(fromJSON(pathToData))
  descriptor <- fromJSON(pathToDescriptor)
  algo <- list(nbBurnInIter = 50,
               nbIter = 50,
               nbGibbsBurnInIter = 20,
               nbGibbsIter = 20,
               nInitPerClass = 10,
               nSemTry = 5,
               confidenceLevel = 0.95)
  
  expect_silent(resLearn <- JsonMixtCompLearn(data, descriptor, algo, nClass = 2,
                                              inputPath = ".", outputFile = "reslearn.json"))
  
  expect_true(is.null(resLearn$warnLog))
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./descriptor.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  
  file.remove("./algo.json", "./descriptor.json", "./data.json")
  
  expect_silent(resPredict <- JsonMixtCompPredict(data, descriptor, algo, nClass = 2,
                                                  inputPath = ".", paramFile = "reslearn.json", outputFile = "respredict.json"))
  
  expect_true(is.null(resPredict$warnLog))
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./descriptor.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  expect_true(file.exists("respredict.json"))
  
  file.remove("./algo.json", "./descriptor.json", "./data.json", "reslearn.json", "respredict.json", "progress")
})

test_that("can predict with only one sample in the data set", {
  pathToData <- system.file("extdata", "data.json", package = "RJMixtComp")
  pathToDescriptor <- system.file("extdata", "desc.json", package = "RJMixtComp")
  
  data <- as.data.frame(fromJSON(pathToData))
  descriptor <- fromJSON(pathToDescriptor)
  algo <- list(nbBurnInIter = 50,
               nbIter = 50,
               nbGibbsBurnInIter = 20,
               nbGibbsIter = 20,
               nInitPerClass = 10,
               nSemTry = 5,
               confidenceLevel = 0.95)
  
  expect_silent(resLearn <- JsonMixtCompLearn(data, descriptor, algo, nClass = 2, inputPath = ".", outputFile = "reslearn.json"))
  expect_true(is.null(resLearn$warnLog))
  
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./descriptor.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  
  file.remove("./algo.json", "./descriptor.json", "./data.json")
  
  # keep only one sample
  data = data[1,]
  
  expect_silent(resPredict <- JsonMixtCompPredict(data, descriptor, algo, nClass = 2,
                                                  inputPath = ".", paramFile = "reslearn.json", outputFile = "respredict.json"))
  
  expect_true(is.null(resPredict$warnLog))
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./descriptor.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  expect_true(file.exists("respredict.json"))
  
  file.remove("./algo.json", "./descriptor.json", "./data.json", "reslearn.json", "respredict.json", "progress")
})
