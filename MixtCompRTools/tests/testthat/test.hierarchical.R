Sys.setenv(MC_DETERMINISTIC = 42)

context("Launch hierarchical MixtComp")


test_that("simple case", {

  set.seed(42)
  data <- matrix(nrow = 200, ncol = 2)
  data[,1] = c(rnorm(50, 0, 0.5), rnorm(50, -4, 0.8), rnorm(30, 2, 0.5), rnorm(70, -1, 0.4))
  data[,2] = c(rnorm(100, -2, 1), rnorm(100, 3, 0.4))
  colnames(data) = c("gauss1", "gauss2")
  truePartition <- rep(1:4, c(50, 50, 30, 70))

  descriptor <- matrix("Gaussian_sjk", nrow = 1, ncol = 2, dimnames = list(NULL, colnames(data)))

  resGetData <- getData(list(data, descriptor))

  mcStrategy <- list(nbBurnInIter = 100,
                    nbIter = 100,
                    nbGibbsBurnInIter = 50,
                    nbGibbsIter = 50,
                    nInitPerClass = 10,
                    nSemTry = 20)

  res <- hierarchicalMixtCompLearn(resGetData$lm, mcStrategy, nbClass = 4, confidenceLevel = 0.95, criterion = "BIC", minClassSize = 5)

  partition = res$res[[4]]$variable$data$z_class$completed

  expect_gt(rand.index(partition, truePartition), 0.9)
})

Sys.unsetenv("MC_DETERMINISTIC")
