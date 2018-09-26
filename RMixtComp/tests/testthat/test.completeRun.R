context("Run MixtComp")

Sys.setenv(MC_DETERMINISTIC = 42)

test_that("Hard coded simple test", {
  set.seed(42)
  
  algo <- list(
    nClass = 2,
    nInd = 20,
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 100,
    nbGibbsIter = 100,
    nInitPerClass = 3,
    nSemTry = 20,
    confidenceLevel = 0.95,
    mode = "learn"
  )
  
  data <- list(
    var1 = c(
      "3.432200",
      "19.14747",
      "5.258037",
      "22.37596",
      "2.834802",
      "18.60959",
      "4.640250",
      "18.59525",
      "5.957942",
      "19.94644",
      "5.560189",
      "17.98966",
      "6.708977",
      "18.10192",
      "5.331169",
      "20.35260",
      "4.003947",
      "21.81531",
      "6.217908",
      "19.38892"
    )
  )
  
  z <- c(1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2)
  
  desc <- list(var1 = list(type = "Gaussian",
                           paramStr = ""))
  
  resLearn <- rmc(algo, data, desc, list())
  
  expect_equal(resLearn$mixture$warnLog, NULL)
  expect_gte(rand.index(getZ_class(resLearn), z), 0.9)
  
  confMatSampled <- table(z, getZ_class(resLearn))
  print(confMatSampled)
})

Sys.unsetenv("MC_DETERMINISTIC")