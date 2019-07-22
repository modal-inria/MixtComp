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
context("Pretreatment functions of mixtCompLearn's parameters")

Sys.setenv(MC_DETERMINISTIC = 42)

test_that("imputModelIntern returns Gaussian when a numeric is given", {
  data <- rnorm(100)
  
  outModel <- imputModelIntern(data, name = "var")
  expect_equal(outModel, "Gaussian")
})

test_that("imputModelIntern returns Poisson when an integer vector is given", {
  data <- 1:100
  
  outModel <- imputModelIntern(data, name = "var")
  expect_equal(outModel, "Poisson")
})

test_that("imputModelIntern returns Multinomial when a character/factor is given", {
  data <- letters
  
  outModel <- imputModelIntern(data, name = "var")
  expect_equal(outModel, "Multinomial")
  
  data <- as.factor(letters)
  
  outModel <- imputModelIntern(data, name = "var")
  expect_equal(outModel, "Multinomial")
})

test_that("imputModelIntern returns LatentClass when the variable is named z_class", {
  data <- 1:100
  
  outModel <- imputModelIntern(data, name = "z_class")
  expect_equal(outModel, "LatentClass")
})


test_that("imputModelIntern returns an error when a bad type is given", {
  data <- list()
  
  expect_error(outModel <- imputModelIntern(data, name = "var"))
})


test_that("imputModel works with data.frame", {
  data <- data.frame(a = 1:3,
                     b = rnorm(3),
                     c = letters[1:3],
                     z_class = 1:3)
  
  expectedModel <- list(a = list(type = "Poisson", paramStr = ""), b = list(type = "Gaussian", paramStr = ""), c = list(type = "Multinomial", paramStr = ""), z_class = list(type = "LatentClass", paramStr = ""))
  
  outModel <- imputModel(data)
  expect_equal(outModel, expectedModel)
})


test_that("imputModel works with list", {
  data <- list(a = 1:3,
               b = rnorm(3),
               c = letters[1:3],
               z_class = 1:3)
  
  expectedModel <- list(a = list(type = "Poisson", paramStr = ""), b = list(type = "Gaussian", paramStr = ""), c = list(type = "Multinomial", paramStr = ""), z_class = list(type = "LatentClass", paramStr = ""))
  
  outModel <- imputModel(data)
  expect_equal(outModel, expectedModel)
})

test_that("imputModel returns an error with a matrix", {
  data <- matrix(rnorm(50), ncol = 5, dimnames = list(NULL, letters[1:5]))
  
  expect_error(outModel <- imputModel(data))
})



test_that("completeModel adds hyperparameters for functional data",{
  
  model <- list(gauss = list(type = "Gaussian", paramStr = ""), func1 = list(type = "Func_CS", paramStr = "nSub: 3, nCoeff: 3"),
                func2 = list(type = "Func_SharedAlpha_CS", paramStr = "nSub: 3, nCoeff: 3"), func3 = list(type = "Func_CS", paramStr = ""),
                func4 = list(type = "Func_SharedAlpha_CS", paramStr = ""))
  
  nInd <- 200
  ratioPresent <- 0.95
  
  var <- list()
  var$z_class <- RMixtCompIO:::zParam()
  
  var$func1 <- RMixtCompIO:::functionalInterPolyParam("func1")
  var$func2 <- RMixtCompIO:::functionalInterPolyParam("func2")
  var$func3 <- RMixtCompIO:::functionalInterPolyParam("func3")
  var$func4 <- RMixtCompIO:::functionalInterPolyParam("func4")
  
  data <- RMixtCompIO:::dataGeneratorNewIO(nInd, ratioPresent, var)$data
  
  expect_warning(out <- completeModel(model, data))
  expect_equal(out, list(gauss = list(type = "Gaussian", paramStr = ""), func1 = list(type = "Func_CS", paramStr = "nSub: 3, nCoeff: 3"),
                         func2 = list(type = "Func_SharedAlpha_CS", paramStr = "nSub: 3, nCoeff: 3"), func3 = list(type = "Func_CS", paramStr = "nSub: 2, nCoeff: 2"),
                         func4 = list(type = "Func_SharedAlpha_CS", paramStr = "nSub: 2, nCoeff: 2")))
  
})


test_that("formatDataBasicMode works with data.frame", {
  dat <- data.frame(a = rnorm(20), b = as.character(rep(letters[1:2], 10)), c = as.factor(rep(letters[1:2], 10)), d = 1:20, z_class = letters[1:20])
  dat[1,] = NA
  model <- list(a = list(type = "Gaussian"), b = list(type = "Multinomial"), c = list(type = "Multinomial"), d = list(type = "Poisson"), z_class = list(type = "LatentClass"))
  
  out <- formatDataBasicMode(dat, model)
  expect_length(out, 2)
  expect_named(out, c("data", "dictionary"))
  expect_type(out$data, "list")
  expect_named(out$data, c("a", "b", "c", "d", "z_class"))
  expect_equal(out$data$a, c("?", as.character(dat$a[-1])))
  expect_equal(out$data$b, c("?", rep(1:2, 9), 1))
  expect_equal(out$data$c, c("?", rep(1:2, 9), 1))
  expect_equal(out$data$d, c("?", as.character(dat$d[-1])))
  expect_equal(out$data$z_class, c("?", 1:19))
  expect_type(out$dictionary, "list")
  expect_length(out$dictionary, 3)
  expect_named(out$dictionary, c("b", "c", "z_class"))
  expect_equal(out$dictionary$b, list(old = letters[2:1], new = c("1", "2")))
  expect_equal(out$dictionary$c, list(old = letters[2:1], new = c("1", "2")))
  expect_equal(out$dictionary$z_class, list(old = letters[2:20], new = as.character(1:19)))
  
})

test_that("formatDataBasicMode works with list", {
  dat <- list(a = rnorm(20), b = as.character(rep(letters[1:2], 10)), c = as.factor(rep(letters[1:2], 10)), d = 1:20, z_class = letters[1:20])
  dat$a[1] = NA
  dat$b[1] = NA
  dat$c[1] = NA
  dat$d[1] = NA
  dat$z_class[1] = NA
  model <- list(a = list(type = "Gaussian"), b = list(type = "Multinomial"), c = list(type = "Multinomial"), d = list(type = "Poisson"), z_class = list(type = "LatentClass"))
  
  out <- formatDataBasicMode(dat, model)
  expect_length(out, 2)
  expect_named(out, c("data", "dictionary"))
  expect_type(out$data, "list")
  expect_named(out$data, c("a", "b", "c", "d", "z_class"))
  expect_equal(out$data$a, c("?", as.character(dat$a[-1])))
  expect_equal(out$data$b, c("?", rep(1:2, 9), 1))
  expect_equal(out$data$c, c("?", rep(1:2, 9), 1))
  expect_equal(out$data$d, c("?", as.character(dat$d[-1])))
  expect_equal(out$data$z_class, c("?", as.character(1:19)))
  expect_type(out$dictionary, "list")
  expect_length(out$dictionary, 3)
  expect_named(out$dictionary, c("b", "c", "z_class"))
  expect_equal(out$dictionary$b, list(old = letters[2:1], new = c("1", "2")))
  expect_equal(out$dictionary$c, list(old = letters[2:1], new = c("1", "2")))
  expect_equal(out$dictionary$z_class, list(old = letters[2:20], new = as.character(1:19)))
})

test_that("formatDataBasicMode works with a dictionary", {
  dat <- list(a = rnorm(20), b = as.character(rep(letters[1:2], 10)), c = as.factor(rep(letters[1:2], 10)), d = 1:20, z_class = 1:20)
  dat$a[1] = NA
  dat$b[1] = NA
  dat$c[1] = NA
  dat$d[1] = NA
  dat$z_class[1] = NA
  model <- list(a = list(type = "Gaussian"), b = list(type = "Multinomial"), c = list(type = "Multinomial"), d = list(type = "Poisson"), z_class = list(type = "LatentClass"))
  dictionary <- list(b = list(old = c("a", "b"), new = c("1", "2")),
                     c = list(old = c("a", "b"), new = c("1", "2")))
  
  out <- formatDataBasicMode(dat, model, dictionary)
  expect_length(out, 2)
  expect_named(out, c("data", "dictionary"))
  expect_type(out$data, "list")
  expect_named(out$data, c("a", "b", "c", "d", "z_class"))
  expect_equal(out$data$a, c("?", as.character(dat$a[-1])))
  expect_equal(out$data$b, c("?", "2", rep(c("1", "2"), 9)))
  expect_equal(out$data$c, c("?", "2", rep(c("1", "2"), 9)))
  expect_equal(out$data$d, c("?", as.character(dat$d[-1])))
  expect_equal(out$data$z_class, c("?", as.character(dat$z_class[-1])))
  expect_equal(out$dictionary, dictionary)
  
  
  dictionary$b = NULL
  expect_error(out <- formatDataBasicMode(dat, model, dictionary))
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

test_that("performHierarchical works", {
  model <- list("a" = list(type = "Gaussian"))
  mode <- "basic"
  
  for(hierarchicalMode in c("yes", "no", "auto"))
  {
    out <- performHierarchical(hierarchicalMode, mode, model)
    expect_false(out)
  }
  
  
  
  mode = "expert"
  out <- performHierarchical(hierarchicalMode = "yes", mode, model)
  expect_true(out)
  
  for(hierarchicalMode in c("no", "auto"))
  {
    out <- performHierarchical(hierarchicalMode, mode, model)
    expect_false(out)
  }
  
  model$b = list(type = "Func_CS")
  out <- performHierarchical(hierarchicalMode = "no", mode, model)
  expect_false(out)
  
  for(hierarchicalMode in c("yes", "auto"))
  {
    out <- performHierarchical(hierarchicalMode, mode, model)
    expect_true(out)
  }
  
})



test_that("changeClassNames works", {
  rowNames <- c("k: 1, lambda", "k: 2, mean:", "k: 2, sd", "k: 3, modality: 1", "k: 3, modality: 2", "k: 4, n:", "k: 4, p", "k: 5, k:", "k: 6, s: 0, alpha0",
                "k: 7, s: 0, alpha1", "k: 8, s: 0, c: 0", "k: 9, s: 0", "k: 10, pi")
  
  dictionary <- list(z_class = list(old = paste0("G", 1:10), new = 1:10))
  out <- changeClassName(rowNames, dictionary)
  expect_equal(out, c("k: G1, lambda", "k: G2, mean:", "k: G2, sd", "k: G3, modality: 1", "k: G3, modality: 2", "k: G4, n:", "k: G4, p", "k: G5, k:", "k: G6, s: 0, alpha0",
                           "k: G7, s: 0, alpha1", "k: G8, s: 0, c: 0", "k: G9, s: 0", "k: G10, pi"))
})



test_that("formatOutputBasicMode works", {
  dictionary <- list(z_class = list(old = c("setosa", "versicolor", "virginica"),
                                    new = c("1", "2", "3")),
                     categ1 = list(old = c("a", "b"), new = c("1", "2")))
  
  res0 <- list(algo = list(),
               variable = list(type = list(z_class = "LatentClass", categ1 = "Multinomial"),
                               data = list(z_class = list(completed = c(3, 3, 1, 2),
                                                          stat = matrix(NA, nrow = 1, ncol = 3, dimnames = list(NULL, c("k: 1", "k: 2", "k: 3")))),
                                           categ1 = list(completed = c(2, 1, 2))),
                               param = list(z_class = list(stat = matrix(NA, nrow = 3, ncol = 3, dimnames = list(c("k: 1", "k: 2", "k: 3"), NULL)),
                                                           log = matrix(NA, nrow = 3, ncol = 2, dimnames = list(c("k: 1", "k: 2", "k: 3"), NULL))),
                                            categ1 = list(stat = matrix(NA, nrow = 6, ncol = 3, dimnames = list(c("k: 1, modality: 1", "k: 1, modality: 2", 
                                                                                                                  "k: 2, modality: 1", "k: 2, modality: 2", 
                                                                                                                  "k: 3, modality: 1", "k: 3, modality: 2"), NULL)),
                                                          log = matrix(NA, nrow = 6, ncol = 2, dimnames = list(c("k: 1, modality: 1", "k: 1, modality: 2", 
                                                                                                                 "k: 2, modality: 1", "k: 2, modality: 2", 
                                                                                                                 "k: 3, modality: 1", "k: 3, modality: 2"), NULL)))
                               )))
  
  
  res <- formatOutputBasicMode(res0, dictionary)
  
  expect_equal(res$algo$dictionary, dictionary)
  expect_equal(res$variable$data$z_class$completed, c("virginica", "virginica", "setosa", "versicolor"))
  expect_equal(colnames(res$variable$data$z_class$stat), c("k: setosa", "k: versicolor", "k: virginica"))
  expect_equal(res$variable$data$categ1$completed, c("b", "a", "b"))
  expect_equal(rownames(res$variable$param$z_class$stat), c("k: setosa", "k: versicolor", "k: virginica"))
  expect_equal(rownames(res$variable$param$z_class$log), c("k: setosa", "k: versicolor", "k: virginica"))
  expect_equal(rownames(res$variable$param$categ1$stat), c("k: setosa, modality: a", "k: setosa, modality: b", 
                                                           "k: versicolor, modality: a", "k: versicolor, modality: b", 
                                                           "k: virginica, modality: a", "k: virginica, modality: b"))
  expect_equal(rownames(res$variable$param$categ1$log), c("k: setosa, modality: a", "k: setosa, modality: b", 
                                                          "k: versicolor, modality: a", "k: versicolor, modality: b", 
                                                          "k: virginica, modality: a", "k: virginica, modality: b"))
  
})


Sys.unsetenv("MC_DETERMINISTIC")
