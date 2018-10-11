context("Write input JSON files for MixtComp")

test_that("Creation of algo file", {
  
  nClass <- 2
  nInd <- 15
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 100,
                     nbGibbsIter = 100, 
                     nInitPerClass = 2,
                     nSemTry = 10,
                     confidenceLevel = 0.95)
  
  mode <- "learn"
  notes <- "You can add any note if you wish in unrequired fields like this one. They will be copied to the output also."
  
  
  expectedJson <- '{
  "nClass": 2,
  "nInd": 15,
  "nbBurnInIter": 100,
  "nbIter": 100,
  "nbGibbsBurnInIter": 100,
  "nbGibbsIter": 100,
  "nInitPerClass": 2,
  "nSemTry": 10,
  "confidenceLevel": 0.95,
  "mode": "learn",
  "notes": "You can add any note if you wish in unrequired fields like this one. They will be copied to the output also."
}'
  
  algoJson <- createAlgoJson(nClass, nInd, mcStrategy, mode, notes = notes)
  expect_true(algoJson == expectedJson)
})



test_that("Conversion of descriptor file", {
  
  descriptor <- data.frame(varGaussian = c("Gaussian", "dummyStrTestGaussian"),
                           varPoisson = c("Poisson", "dummyStrTestPoisson"),
                           varCategorical = c("Categorical", ""))
  
  expectedDesc <- list(varGaussian = list(type = "Gaussian", paramStr = "dummyStrTestGaussian"),
                       varPoisson = list(type = "Poisson", paramStr = "dummyStrTestPoisson"),
                       varCategorical = list(type = "Categorical", paramStr = ""))
  
  descriptorList <- convertDescriptorToList(descriptor)

  expect_equal(descriptorList, expectedDesc)
})


test_that("Conversion of descriptor file: case without paramStr", {
  
  descriptor <- data.frame(varGaussian = "Gaussian",
                           varPoisson = "Poisson",
                           varCategorical = "Categorical")
  
  expectedDesc <- list(varGaussian = list(type = "Gaussian", paramStr = ""),
                       varPoisson = list(type = "Poisson", paramStr = ""),
                       varCategorical = list(type = "Categorical", paramStr = ""))
  
  descriptorList <- convertDescriptorToList(descriptor)
  
  expect_equal(descriptorList, expectedDesc)
})


test_that("Creation of descriptor file", {
  
  descriptor <- data.frame(varGaussian = c("Gaussian", "dummyStrTestGaussian"),
                           varPoisson = c("Poisson", "dummyStrTestPoisson"),
                           varCategorical = c("Multinomial", ""))
  
  
  expectedJson <- '{
  "varGaussian": {
    "type": "Gaussian",
    "paramStr": "dummyStrTestGaussian"
  },
  "varPoisson": {
    "type": "Poisson",
    "paramStr": "dummyStrTestPoisson"
  },
  "varCategorical": {
    "type": "Multinomial",
    "paramStr": ""
  }
}'
  
  descriptorJson <- createDescriptorJson(descriptor)
  expect_true(descriptorJson == expectedJson)
})

test_that("Creation of descriptor file: case without paramStr", {
  
  descriptor <- data.frame(varGaussian = c("Gaussian"),
                           varPoisson = c("Poisson"),
                           varCategorical = c("Multinomial"))
  
  expectedJson <- '{
  "varGaussian": {
    "type": "Gaussian",
    "paramStr": ""
  },
  "varPoisson": {
    "type": "Poisson",
    "paramStr": ""
  },
  "varCategorical": {
    "type": "Multinomial",
    "paramStr": ""
  }
}'
  
  descriptorJson <- createDescriptorJson(descriptor)
  expect_true(descriptorJson == expectedJson)
})


test_that("Creation of data file: character version", {
  
  data <- data.frame(varGaussian = c("3", "?", "2.7", "3.4", "1.7", "2.99", "12.5", "3.1", "12.8", "?", "11", "4", "12.7", "3.05", "12.01"),
                     varPoisson = c("3", "13", "2", "4", "2", "?", "13", "3", "11", "2", "12", "4", "11", "?", "12"),
                     varCategorical = c("1", "2", "1", "1", "1", "1", "?", "{1, 2}", "2", "1", "2", "1", "2", "?", "2"))
  
  
  expectedJson <- '{
  "varGaussian": ["3", "?", "2.7", "3.4", "1.7", "2.99", "12.5", "3.1", "12.8", "?", "11", "4", "12.7", "3.05", "12.01"],
  "varPoisson": ["3", "13", "2", "4", "2", "?", "13", "3", "11", "2", "12", "4", "11", "?", "12"],
  "varCategorical": ["1", "2", "1", "1", "1", "1", "?", "{1, 2}", "2", "1", "2", "1", "2", "?", "2"]
}'
  
  dataJson <- createDataJson(data)
  expect_true(dataJson == expectedJson)
})

test_that("Creation of data file: numeric version", {
  
  data <- data.frame(varGaussian = c(3, 2.7, 3.4, 1.7, 2.99, 12.5, 3.1),
                     varPoisson = c(3, 13, 2, 4, 2, 1, 3),
                     varCategorical = c(1, 2, 1, 1, 1, 1, 2))
  
  
  expectedJson <- '{
  "varGaussian": ["3", "2.7", "3.4", "1.7", "2.99", "12.5", "3.1"],
  "varPoisson": ["3", "13", "2", "4", "2", "1", "3"],
  "varCategorical": ["1", "2", "1", "1", "1", "1", "2"]
}'
  
  dataJson <- createDataJson(data)
  expect_true(dataJson == expectedJson)
})

test_that("Creation of data file: case with one individuals", {
  
  data <- data.frame(varGaussian = "3",
                     varPoisson = "3",
                     varCategorical = "1")
  
  
  expectedJson <- '{
  "varGaussian": ["3"],
  "varPoisson": ["3"],
  "varCategorical": ["1"]
}'
  
  dataJson <- createDataJson(data)
  expect_true(dataJson == expectedJson)
})


