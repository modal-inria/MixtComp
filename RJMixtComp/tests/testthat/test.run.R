# MixtComp version 4.0  - july 2019
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

# @author Quentin Grimonprez
context("Test fonctionnel")

Sys.setenv(MC_DETERMINISTIC = 42)

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

Sys.unsetenv("MC_DETERMINISTIC")