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
context("Slope heuristic")

test_that("computeCSlopeHeuristic works", {
  indD0 <- 1
  logLikeObs <- (1:10)*2
  nFreeParam <- 1:10
  
  C <- computeCSlopeHeuristic(logLikeObs, nFreeParam, indD0)
  
  expect_equal(C, 2)

  
  indD0 <- 5
  C <- computeCSlopeHeuristic(logLikeObs, nFreeParam, indD0)
  
  expect_equal(C, 2)
  
  
  indD0 <- 5
  logLikeObs <- c(1:4, (5:10) * 2)
  nFreeParam <- 1:10
  
  C <- computeCSlopeHeuristic(logLikeObs, nFreeParam, indD0)
  
  expect_equal(C, 2)
  
})



test_that("computeCSlopeHeuristic manages NA, NaN and Inf", {
  
  indD0 <- 1
  logLikeObs <- (1:10)*2
  nFreeParam <- 1:10
  
  logLikeObs[10] = NA
  C <- computeCSlopeHeuristic(logLikeObs, nFreeParam, indD0)
  expect_equal(C, 2)
  

  logLikeObs[10] = Inf
  C <- computeCSlopeHeuristic(logLikeObs, nFreeParam, indD0)
  expect_equal(C, 2)
  

  logLikeObs[10] = NaN
  C <- computeCSlopeHeuristic(logLikeObs, nFreeParam, indD0)
  expect_equal(C, 2)
})
