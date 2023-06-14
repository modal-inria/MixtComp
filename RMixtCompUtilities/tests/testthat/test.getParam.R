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
context("getParam")


test_that("getParamNumerical formats well", {
  param <- list(stat = matrix(1:9, nrow = 3, dimnames = list(c("k: a", "k: b", "k: c"), NULL)))
  paramFormatted <- getParamNumerical(param, 3, "alpha")

  expect_equal(dim(paramFormatted), c(3, 1))
  expect_equivalent(paramFormatted[, 1], 1:3)
  expect_equal(colnames(paramFormatted), "alpha")
  expect_equal(rownames(paramFormatted), c("k: a", "k: b", "k: c"))


  param <- list(stat = matrix(1:12, nrow = 4, dimnames = list(c(rep("k: 1", 2), rep("k: 2", 2)), NULL)))
  paramFormatted <- getParamNumerical(param, 2, c("alpha", "beta"))

  expect_equal(dim(paramFormatted), c(2, 2))
  expect_equivalent(paramFormatted[, 1], c(1, 3))
  expect_equivalent(paramFormatted[, 2], c(2, 4))
  expect_equal(colnames(paramFormatted), c("alpha", "beta"))
  expect_equal(rownames(paramFormatted), c("k: 1", "k: 2"))
})


test_that("getParamMultinomial works well", {
  param <- list(
    stat = matrix(
      1:8, nrow = 8, ncol = 3,
      dimnames = list(
        c("k: a, modality: aa", "k: a, modality: 22", "k: a, modality: ouais", "k: a, modality: 0", rep("k: 1", 4)), NULL
      )
    ),
    paramStr = "nModality: 4"
  )
  paramFormatted <- getParamMultinomial(param, 2)

  expect_equal(dim(paramFormatted), c(2, 4))
  expect_equivalent(paramFormatted[1, ], 1:4)
  expect_equivalent(paramFormatted[2, ], 5:8)
  expect_equal(colnames(paramFormatted), c("modality aa", "modality 22", "modality ouais", "modality 0"))
  expect_equal(rownames(paramFormatted), c("k: a", "k: 1"))
})


test_that("getParamRank_ISR works well", {
  param <- list(
    mu = list(stat = list(
      "k: 0" = list(rank = matrix(4:1, nrow = 1), proba = 0.8),
      "k: 1" = list(rank = matrix(1:4, nrow = 1), proba = 0.9),
      "k: 2" = list(rank = matrix(c(3, 4, 1, 2), nrow = 1), proba = 1)
    )),
    pi = list(stat = matrix(1:9, nrow = 3, ncol = 3, dimnames = list(c("k: 1", "k: 2", "k: 3"), NULL))),
    paramStr = "nModality: 4"
  )
  paramFormatted <- getParamRank_ISR(param, 3)

  expect_equal(length(paramFormatted), 2)
  expect_equal(dim(paramFormatted$pi), c(3, 1))
  expect_equal(dim(paramFormatted$mu), c(3, 4))
  expect_equivalent(paramFormatted$pi, 1:3)
  expect_equivalent(paramFormatted$mu[1, ], 4:1)
  expect_equivalent(paramFormatted$mu[2, ], 1:4)
  expect_equivalent(paramFormatted$mu[3, ], c(3, 4, 1, 2))
  expect_equal(colnames(paramFormatted$pi), "pi")
  expect_equal(rownames(paramFormatted$pi), c("k: 1", "k: 2", "k: 3"))
  expect_equal(rownames(paramFormatted$mu), c("k: 1", "k: 2", "k: 3"))
})

test_that("getnSub and getnCoeff works", {
  paramStr <- "nSub  : 2, nCoeff   : 3 "
  expect_equal(getnSub(paramStr), 2)
  expect_equal(getnCoeff(paramStr), 3)

  paramStr <- "nSub    :   12, nCoeff:23"
  expect_equal(getnSub(paramStr), 12)
  expect_equal(getnCoeff(paramStr), 23)
})

test_that("getParamFunc_CS works well", {
  param <- list(
    # alpha size: nbClass * nSub * 2
    alpha = list(stat = matrix(12:1, nrow = 12, ncol = 3, dimnames = list(rep(c("k: 1", "k: 2"), each = 6), NULL))),
    # beta size: nbClass * nSub * nCoeff
    beta = list(stat = matrix(1:12, nrow = 12, ncol = 3, dimnames = list(rep(c("k: 1", "k: 2"), each = 6), NULL))),
    # sd size: nbClass * nSub
    sd = list(stat = matrix(1:6, nrow = 6, ncol = 3, dimnames = list(rep(c("k: 1", "k: 2"), each = 3), NULL))),
    paramStr = "nSub: 3, nCoeff: 2"
  )
  paramFormatted <- getParamFunc_CS(param, 2)

  expect_equal(length(paramFormatted), 3)
  expect_equal(dim(paramFormatted$alpha), c(2, 6))
  expect_equal(dim(paramFormatted$beta), c(2, 6))
  expect_equal(dim(paramFormatted$sd), c(2, 3))
  expect_equivalent(paramFormatted$alpha[1, ], 12:7)
  expect_equivalent(paramFormatted$alpha[2, ], 6:1)
  expect_equivalent(paramFormatted$beta[1, ], 1:6)
  expect_equivalent(paramFormatted$beta[2, ], 7:12)
  expect_equivalent(paramFormatted$sd[1, ], 1:3)
  expect_equivalent(paramFormatted$sd[2, ], 4:6)
  expect_equal(
    colnames(paramFormatted$alpha),
    c("s:1, alpha0", "s:1, alpha1", "s:2, alpha0", "s:2, alpha1", "s:3, alpha0", "s:3, alpha1")
  )
  expect_equal(colnames(paramFormatted$beta), c("s:1, c:1", "s:1, c:2", "s:2, c:1", "s:2, c:2", "s:3, c:1", "s:3, c:2"))
  expect_equal(colnames(paramFormatted$sd), c("s:1", "s:2", "s:3"))
  expect_equal(rownames(paramFormatted$alpha), c("k: 1", "k: 2"))
  expect_equal(rownames(paramFormatted$beta), c("k: 1", "k: 2"))
  expect_equal(rownames(paramFormatted$sd), c("k: 1", "k: 2"))
})

failedResLearn <- list(
  "warnLog" = "something"
)

test_that("getCompletedData works with failed event", {
  expect_warning(comp <- getCompletedData(failedResLearn))
  expect_equal(comp, c())
})

test_that("getPartition works with failed event", {
  expect_warning(partition <- getPartition(failedResLearn))
  expect_equal(partition, c())
})

test_that("getType works with failed event", {
  expect_warning(type <- getType(failedResLearn))
  expect_equal(type, c())
})

test_that("getModel works with failed event", {
  expect_warning(model <- getModel(failedResLearn))
  expect_equal(model, c())
})

test_that("getVarNames works with failed event", {
  expect_warning(varNames <- getVarNames(failedResLearn))
  expect_equal(varNames, c())
})

test_that("getEmpiricTik works with failed event", {
  expect_warning(tik <- getEmpiricTik(failedResLearn))
  expect_equal(tik, c())
})

test_that("getTik works with failed event", {
  expect_warning(tik <- getTik(failedResLearn))
  expect_equal(tik, c())
})

test_that("getMixtureDensity works with failed event", {
  expect_warning(dens <- getMixtureDensity(failedResLearn))
  expect_equal(dens, c())
})

test_that("getBIC works with failed event", {
  expect_warning(crit <- getBIC(failedResLearn))
  expect_equal(crit, NaN)
})

test_that("getICL works with failed event", {
  expect_warning(crit <- getICL(failedResLearn))
  expect_equal(crit, NaN)
})
