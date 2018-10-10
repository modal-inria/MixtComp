# Simple test case for the new IO sytem
# Sampling law is mixture model with 0.5 0.5 proportions
# First class conditional law is N(5., 1.), rnorm(10, 5., 1.)
# Second class conditional law is N(20., 2.), rnorm(10, 20., 2.)
simpleNormalTest <- function() {
  
  Sys.setenv(MC_DETERMINISTIC = 42)
  
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
  
  desc <- list(
    var1 = list(
      type = "Gaussian",
      paramStr = ""
    )
  )
  
  res <- rmc(algo, data, desc, list())
  
  Sys.unsetenv("MC_DETERMINISTIC")
  
  return(res)
}