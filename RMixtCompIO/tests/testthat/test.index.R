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
context("Change index number")

test_that("correctIndexCompletedStatNumerical works", {
  stat <- matrix(1:10, nrow = 10, ncol = 4, dimnames = list(NULL, c("index", "median", "q 2.5%", "q 97.5%")))
  expectedOut <- stat
  expectedOut[, 1] = expectedOut[, 1] + 1
  
  out <- correctIndexCompletedStatNumerical(stat)
  
  expect_equal(out, expectedOut)
})


test_that("correctIndexCompletedStatMultinomRank works", {
  stat <- list("5" = matrix(1:10, nrow = 5, ncol = 2, dimnames = list(NULL, c("categ", "proba"))), 
               "10" = matrix(1:10, nrow = 5, ncol = 2, dimnames = list(NULL, c("categ", "proba"))))
  expectedOut <- list("6" = matrix(1:10, nrow = 5, ncol = 2, dimnames = list(NULL, c("categ", "proba"))), 
                      "11" = matrix(1:10, nrow = 5, ncol = 2, dimnames = list(NULL, c("categ", "proba"))))
  
  out <- correctIndexCompletedStatMultinomRank(stat)
  
  expect_equal(out, expectedOut)
})

test_that("correctIndexCompletedStat works", {
  stat <- list("5" = matrix(1:10, nrow = 5, ncol = 2, dimnames = list(NULL, c("categ", "proba"))), 
               "10" = matrix(1:10, nrow = 5, ncol = 2, dimnames = list(NULL, c("categ", "proba"))))
  expectedOut <- list("6" = matrix(1:10, nrow = 5, ncol = 2, dimnames = list(NULL, c("categ", "proba"))), 
                      "11" = matrix(1:10, nrow = 5, ncol = 2, dimnames = list(NULL, c("categ", "proba"))))
  
  out <- correctIndexCompletedStat(stat, "Multinomial")
  expect_equal(out, expectedOut)
  
  out <- correctIndexCompletedStat(stat, "Rank_ISR")
  expect_equal(out, expectedOut)
  
  
  stat <- matrix(1:10, nrow = 10, ncol = 4, dimnames = list(NULL, c("index", "median", "q 2.5%", "q 97.5%")))
  expectedOut <- stat
  expectedOut[, 1] = expectedOut[, 1] + 1
  
  out <- correctIndexCompletedStat(stat, "NegativeBinomial")
  expect_equal(out, expectedOut)
  
  out <- correctIndexCompletedStat(stat, "Weibull")
  expect_equal(out, expectedOut)
  
  out <- correctIndexCompletedStat(stat, "Poisson")
  expect_equal(out, expectedOut)
  
  out <- correctIndexCompletedStat(stat, "Gaussian")
  expect_equal(out, expectedOut)
  
  
  
  out <- correctIndexCompletedStat(stat, "LatentClass")
  expect_equal(out, stat)
  
  out <- correctIndexCompletedStat(stat, "Func_CS")
  expect_equal(out, stat)
  
  out <- correctIndexCompletedStat(stat, "Func_SharedAlpha_CS")
  expect_equal(out, stat)
  
  out <- correctIndexCompletedStat(stat, "CorReg")
  expect_equal(out, stat)
})

