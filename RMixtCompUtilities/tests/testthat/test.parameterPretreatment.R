# MixtComp version 4 - july 2019
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


test_that("formatModel does not change well formated data", {
  desc <- list(
    var1 = list(type = "Gaussian", paramStr = ""),
    var2 = list(type = "CorReg", paramStr = "ouais"),
    var3 = list(type = "Multinomial", paramStr = "CorReg")
  )

  outDesc <- formatModel(desc)
  expect_equal(outDesc, desc)
})


test_that("formatModel adds paramStr when missing", {
  desc <- list(
    var1 = list(type = "Gaussian"),
    var2 = list(type = "CorReg", paramStr = "ouais"),
    var3 = list(type = "Multinomial")
  )

  outDesc <- formatModel(desc)
  expect_equal(outDesc$var2, desc$var2)
  expect_equal(outDesc$var1, list(type = "Gaussian", paramStr = ""))
  expect_equal(outDesc$var3, list(type = "Multinomial", paramStr = ""))
})


test_that("formatModel puts type in a list format", {
  desc <- list(
    var1 = "Gaussian",
    var2 = list(type = "CorReg", paramStr = "ouais"),
    var3 = "Multinomial"
  )

  outDesc <- formatModel(desc)
  expect_equal(outDesc$var1, list(type = "Gaussian", paramStr = ""))
  expect_equal(outDesc$var2, desc$var2)
  expect_equal(outDesc$var3, list(type = "Multinomial", paramStr = ""))
})

test_that("formatModel works with named vector", {
  desc <- c("var1" = "Gaussian", "var2" = "CorReg")
  expectedOut <- list(
    var1 = list(type = "Gaussian", paramStr = ""),
    var2 = list(type = "CorReg", paramStr = "")
  )

  outDesc <- formatModel(desc)
  expect_equal(outDesc, expectedOut)
})

test_that("formatModel works with matrix", {
  desc <- matrix(c("Gaussian", "", "CorReg", "ouais"), nrow = 2, dimnames = list(NULL, c("var1", "var2")))
  expectedOut <- list(
    var1 = list(type = "Gaussian", paramStr = ""),
    var2 = list(type = "CorReg", paramStr = "ouais")
  )

  outDesc <- formatModel(desc)
  expect_equal(outDesc, expectedOut)
})

test_that("formatModel works with data.frame", {
  desc <- data.frame(
    var1 = c("Gaussian", ""),
    var2 = c("CorReg", "ouais")
  )
  expectedOut <- list(
    var1 = list(type = "Gaussian", paramStr = ""),
    var2 = list(type = "CorReg", paramStr = "ouais")
  )

  outDesc <- formatModel(desc)
  expect_equal(outDesc, expectedOut)
})

test_that("formatData converts data.frame into a list format", {
  dat <- data.frame(x1 = 1:10, x2 = 10:1)
  dataOut <- formatData(dat)

  expect_equal(names(dataOut), colnames(dat))
  expect_equivalent(sapply(dataOut, length), rep(nrow(dat), ncol(dat)))
  expect_true(all(sapply(dataOut, is.character)))
})

test_that("formatData converts matrix into a list format", {
  dat <- matrix(c(1:10, 10:1), ncol = 2, dimnames = list(NULL, c("x1", "x2")))
  dataOut <- formatData(dat)

  expect_equal(names(dataOut), colnames(dat))
  expect_equivalent(sapply(dataOut, length), rep(nrow(dat), ncol(dat)))
  expect_true(all(sapply(dataOut, is.character)))
})

test_that("formatData keeps list in list format", {
  dat <- list(x1 = 1:10, x2 = 10:1)
  dataOut <- formatData(dat)

  expect_true(is.list(dataOut))
  expect_equal(names(dataOut), names(dat))
  expect_equal(class(dataOut$x1), "character")
  expect_equal(class(dataOut$x2), "character")
})

test_that("completeAlgo adds missing elements", {
  algo <- list()
  outAlgo <- completeAlgo(algo)
  expectedAlgo <- createAlgo()

  expect_setequal(names(outAlgo), names(expectedAlgo))
  expect_equal(outAlgo[c(order(names(outAlgo)))], expectedAlgo[c(order(names(expectedAlgo)))])


  algo <- list(nbIter = 100)
  outAlgo <- completeAlgo(algo)
  expectedAlgo <- createAlgo(nbIter = 100)

  expect_setequal(names(outAlgo), names(expectedAlgo))
  expect_equal(outAlgo[c(order(names(outAlgo)))], expectedAlgo[c(order(names(expectedAlgo)))])
})

test_that("completeAlgo keeps unrequired fields", {
  algo <- list(nbIter = 100, mode = "learn")
  outAlgo <- completeAlgo(algo)
  expectedAlgo <- c(createAlgo(nbIter = 100), list(mode = "learn"))

  expect_setequal(names(outAlgo), names(expectedAlgo))
  expect_equal(outAlgo[c(order(names(outAlgo)))], expectedAlgo[c(order(names(expectedAlgo)))])
})

Sys.unsetenv("MC_DETERMINISTIC")
