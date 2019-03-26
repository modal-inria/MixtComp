context("Test fonctionnel")

test_that("Learn + predict", {
  pathToData <- system.file("extdata", "data.json", package = "RJMixtComp")
  pathToDescriptor <- system.file("extdata", "desc.json", package = "RJMixtComp")

  data <- as.data.frame(fromJSON(pathToData))
  model <- fromJSON(pathToDescriptor)
  algo <- createAlgo()

  expect_silent(resLearn <- JMixtCompLearn(data, model, algo, nClass = 2,
                                           inputPath = ".", outputFile = "reslearn.json"))

  expect_true(is.null(resLearn$warnLog))
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./model.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))

  file.remove("./algo.json", "./model.json", "./data.json")

  expect_silent(resPredict <- JMixtCompPredict(data, model, algo, nClass = 2,
                                               inputPath = ".", paramFile = "reslearn.json", outputFile = "respredict.json"))

  expect_true(is.null(resPredict$warnLog))
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./model.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  expect_true(file.exists("respredict.json"))

  file.remove("./algo.json", "./model.json", "./data.json", "reslearn.json", "respredict.json")
})



test_that("Learn + predict functional", {
  pathToData <- system.file("extdata", "functionalData.RData", package = "RJMixtComp")
  load(pathToData)

  expect_silent(resLearn <- JMixtCompLearn(data, model, algo, nClass = 2,
                                           inputPath = ".", outputFile = "reslearn.json"))

  expect_true(is.null(resLearn$warnLog))
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./model.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))


  expect_silent(resPredict <- JMixtCompPredict(data, model, algo, nClass = 2, inputPath = ".",
                                               paramFile = "reslearn.json", outputFile = "respredict.json"))
  expect_true(is.null(resPredict$warnLog))
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./model.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  expect_true(file.exists("respredict.json"))

  file.remove("./algo.json", "./model.json", "./data.json", "reslearn.json", "respredict.json")

})


test_that("Learn + predict rank", {
  pathToData <- system.file("extdata", "rankData.RData", package = "RJMixtComp")
  load(pathToData)
  
  expect_silent(resLearn <- JMixtCompLearn(data, desc, algo, nClass = 2,
                                           inputPath = ".", outputFile = "reslearn.json"))
  
  expect_true(is.null(resLearn$warnLog))
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./model.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  
  
  expect_silent(resPredict <- JMixtCompPredict(data, desc, algo, nClass = 2, inputPath = ".",
                                               paramFile = "reslearn.json", outputFile = "respredict.json"))
  expect_true(is.null(resPredict$warnLog))
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./model.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  expect_true(file.exists("respredict.json"))
  
  file.remove("./algo.json", "./model.json", "./data.json", "reslearn.json", "respredict.json")
  
})



test_that("can predict with only one sample in the data set", {
  pathToData <- system.file("extdata", "data.json", package = "RJMixtComp")
  pathToDescriptor <- system.file("extdata", "desc.json", package = "RJMixtComp")
  
  data <- as.data.frame(fromJSON(pathToData))
  model <- fromJSON(pathToDescriptor)
  algo <- createAlgo()
  
  expect_silent(resLearn <- JMixtCompLearn(data, model, algo, nClass = 2, inputPath = ".", outputFile = "reslearn.json"))
  expect_true(is.null(resLearn$warnLog))
  
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./model.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  
  file.remove("./algo.json", "./model.json", "./data.json")
  
  # keep only one sample
  data = data[1,]
  
  expect_silent(resPredict <- JMixtCompPredict(data, model, algo, nClass = 2,
                                               inputPath = ".", paramFile = "reslearn.json", outputFile = "respredict.json"))
  
  expect_true(is.null(resPredict$warnLog))
  expect_true(file.exists("./algo.json"))
  expect_true(file.exists("./model.json"))
  expect_true(file.exists("./data.json"))
  expect_true(file.exists("reslearn.json"))
  expect_true(file.exists("respredict.json"))
  
  file.remove("./algo.json", "./model.json", "./data.json", "reslearn.json", "respredict.json")
})
