context("Concatenate parameter for hierarchical MixtComp")

test_that("concatenate zclass", {
  paramParent <- list(pi = list(stat = matrix(c(0.25, 0.5, 0.1, 0.15), nrow = 4, ncol = 3), paramStr = ""))
  paramChild <- list(pi = list(stat = matrix(c(0.4, 0.6), nrow = 2, ncol = 3), paramStr = ""))
  classParentOfChild <- 2

  out <- concatenateParamZclass(paramParent, paramChild, classParentOfChild)

  expect_equal(nrow(out$pi$stat), 5)
  expect_equal(ncol(out$pi$stat), 3)
  expect_equal(out$pi$stat[,1], c(0.25, 0.1, 0.15, 0.2, 0.3))
  expect_equal(out$pi$paramStr, "")
})


test_that("Concatenate Gaussian/Weibull/NegBin", {
  paramParent <- list(NumericalParam = list(stat = matrix(1:4, nrow = 4, ncol = 3), paramStr = ""))
  paramChild <- list(NumericalParam = list(stat = matrix(4:1, nrow = 4, ncol = 3), paramStr = ""))
  classParentOfChild <- 1

  out <- concatenateParamNumeric2Param(paramParent, paramChild, classParentOfChild)

  expect_equal(nrow(out$NumericalParam$stat), 6)
  expect_equal(ncol(out$NumericalParam$stat), 3)
  expect_equal(out$NumericalParam$stat[,1], c(3, 4, 4, 3, 2, 1))
  expect_equal(out$NumericalParam$paramStr, "")
})

test_that("Concatenate Poisson", {
  paramParent <- list(NumericalParam = list(stat = matrix(1:4, nrow = 4, ncol = 3), paramStr = ""))
  paramChild <- list(NumericalParam = list(stat = matrix(2:1, nrow = 2, ncol = 3), paramStr = ""))
  classParentOfChild <- 2

  out <- concatenateParamPoisson(paramParent, paramChild, classParentOfChild)

  expect_equal(nrow(out$NumericalParam$stat), 5)
  expect_equal(ncol(out$NumericalParam$stat), 3)
  expect_equal(out$NumericalParam$stat[,1], c(1, 3, 4, 2, 1))
  expect_equal(out$NumericalParam$paramStr, "")
})

test_that("Concatenate Categorical", {
  paramParent <- list(NumericalParam = list(stat = matrix(1:12, nrow = 12, ncol = 3), paramStr = "nModality: 3"))
  paramChild <- list(NumericalParam = list(stat = matrix(6:1, nrow = 6, ncol = 3), paramStr = "nModality: 3"))
  classParentOfChild <- 2

  out <- concatenateParamCategorical(paramParent, paramChild, classParentOfChild)

  expect_equal(nrow(out$NumericalParam$stat), 15)
  expect_equal(ncol(out$NumericalParam$stat), 3)
  expect_equal(out$NumericalParam$stat[,1], c(1:3, 7:12, 6:1))
  expect_equal(out$NumericalParam$paramStr, "nModality: 3")
})


test_that("Concatenate Functional", {
  paramParent <- list(alpha = list(stat = matrix(1:8, nrow = 8, ncol = 3,
                                                 dimnames = list(c("k: 0, s: 0, alpha0", "k: 0, s: 0, alpha1", "k: 0, s: 1, alpha0", "k: 0, s: 1, alpha1",
                                                                   "k: 1, s: 0, alpha0", "k: 1, s: 0, alpha1", "k: 1, s: 1, alpha0", "k: 1, s: 1, alpha1"), NULL)),
                                   paramStr = "nSub: 2, nCoeff: 2"),
                      beta = list(stat = matrix(1:8, nrow = 8, ncol = 3,
                                                dimnames = list(c("k: 0, s: 0, c: 0", "k: 0, s: 0, c: 1", "k: 0, s: 1, c: 0", "k: 0, s: 1, c: 1",
                                                                  "k: 1, s: 0, c: 0", "k: 1, s: 0, c: 1", "k: 1, s: 1, c: 0", "k: 1, s: 1, c: 1"), NULL)),
                                  paramStr = "nSub: 2, nCoeff: 2"),
                      sd = list(stat = matrix(1:4, nrow = 4, ncol = 3,
                                              dimnames = list(c("k: 0, s: 0", "k: 0, s: 1", "k: 1, s: 0", "k: 1, s: 1"), NULL)),
                                paramStr = "nSub: 2, nCoeff: 2"))
  paramChild <- list(alpha = list(stat = matrix(8:1, nrow = 8, ncol = 3,
                                                dimnames = list(c("k: 0, s: 0, alpha0", "k: 0, s: 0, alpha1", "k: 0, s: 1, alpha0", "k: 0, s: 1, alpha1",
                                                                  "k: 1, s: 0, alpha0", "k: 1, s: 0, alpha1", "k: 1, s: 1, alpha0", "k: 1, s: 1, alpha1"), NULL)),
                                  paramStr = "nSub: 2, nCoeff: 2"),
                     beta = list(stat = matrix(8:1, nrow = 8, ncol = 3,
                                               dimnames = list(c("k: 0, s: 0, c: 0", "k: 0, s: 0, c: 1", "k: 0, s: 1, c: 0", "k: 0, s: 1, c: 1",
                                                                 "k: 1, s: 0, c: 0", "k: 1, s: 0, c: 1", "k: 1, s: 1, c: 0", "k: 1, s: 1, c: 1"), NULL)),
                                 paramStr = "nSub: 2, nCoeff: 2"),
                     sd = list(stat = matrix(4:1, nrow = 4, ncol = 3,
                                             dimnames = list(c("k: 0, s: 0", "k: 0, s: 1", "k: 1, s: 0", "k: 1, s: 1"), NULL)),
                               paramStr = "nSub: 2, nCoeff: 2"))

  classParentOfChild <- 2

  out <- concatenateParamFunctional(paramParent, paramChild, classParentOfChild)

  expect_equal(nrow(out$alpha$stat), 12)
  expect_equal(ncol(out$alpha$stat), 3)
  expect_equal(nrow(out$beta$stat), 12)
  expect_equal(ncol(out$beta$stat), 3)
  expect_equal(nrow(out$sd$stat), 6)
  expect_equal(ncol(out$sd$stat), 3)
  expect_equivalent(out$alpha$stat[,1], c(1:4, 8:1))
  expect_equivalent(out$beta$stat[,1], c(1:4, 8:1))
  expect_equivalent(out$sd$stat[,1], c(1:2, 4:1))
  expect_equal(out$alpha$paramStr, "nSub: 2, nCoeff: 2")
  expect_equal(out$beta$paramStr, "nSub: 2, nCoeff: 2")
  expect_equal(out$sd$paramStr, "nSub: 2, nCoeff: 2")
})

test_that("Concatenate Rank", {
  paramParent <- list(mu = list(stat = list("k: 1, mu" = list(c(3, 2, 1, 0), 0.8), "k: 2, mu" = list(c(0, 1, 2, 3), 0.6)), paramStr = "nModality: 4"))
  paramChild <- list(mu = list(stat = list("k: 1, mu" = list(c(2, 1, 3, 0), 0.9), "k: 2, mu" = list(c(2, 3, 1, 0), 0.7)), paramStr = "nModality: 4"))
  classParentOfChild <- 2

  out <- concatenateParamRank(paramParent, paramChild, classParentOfChild)

  expect_equal(length(out$mu$stat), 3)
  expect_equal(out$mu$stat[[1]][[1]], 3:0)
  expect_equal(out$mu$stat[[2]][[1]], c(2, 1, 3, 0))
  expect_equal(out$mu$stat[[3]][[1]], c(2, 3, 1, 0))
  expect_equal(out$mu$stat[[1]][[2]], 0.8)
  expect_equal(out$mu$stat[[2]][[2]], 0.9)
  expect_equal(out$mu$stat[[3]][[2]], 0.7)
  expect_equal(out$mu$paramStr, "nModality: 4")
})

