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
