# Simple test case for the new IO sytem
# Sampling law is mixture model with 0.5 0.5 proportions
# First class conditional law is N(5., 1.), rnorm(10, 5., 1.)
# Second class conditional law is N(20., 2.), rnorm(10, 20., 2.)
simpleNormalTest <- function() {
  
  Sys.setenv(MC_DETERMINISTIC = 42)
  
  algoLearn <- list(
    nClass = 2,
    nInd = 20,
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 100,
    nbGibbsIter = 100,
    nInitPerClass = 3,
    nSemTry = 20,
    confidenceLevel = 0.95,
    nStableCriterium = 5,
    ratioStableCriterium = 0.9,
    mode = "learn"
  )
  
  dataLearn <- list(
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
  
  descLearn <- list(
    var1 = list(
      type = "Gaussian",
      paramStr = ""
    )
  )
  
  zLearn <- c(1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2)
  
  resLearn <- rmc(algoLearn, dataLearn, descLearn, list())
  
  print("learn")
  paste0("rand: ", rand.index(getPartition(resLearn), zLearn)) # expected 0.9 rand
  print("contengency: ")
  print(table(zLearn, getPartition(resLearn)))
  
  algoPredict <- list(
    nClass = 2,
    nInd = 6,
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 100,
    nbGibbsIter = 100,
    nInitPerClass = 3,
    nSemTry = 20,
    confidenceLevel = 0.95,
    nStableCriterium = 5,
    ratioStableCriterium = 0.9,
    mode = "predict"
  )
  
  dataPredict <- list(var1 = c(
    "4.838457",
    "19.90595",
    "4.577347",
    "21.19830",
    "5.048325",
    "20.46875"
  ))
  
  descPredict <- list(var1 = list(type = "Gaussian",
                                  paramStr = ""))
  
  zPredict <- c(1, 2, 1, 2, 1, 2)
  
  resPredict <- rmc(algoPredict, dataPredict, descPredict, resLearn)
  
  print("predict")
  paste0("rand: ", rand.index(getPartition(resPredict), zPredict)) # expected 0.9 rand
  print("contengency: ")
  print(table(zPredict, getPartition(resPredict)))
  
  Sys.unsetenv("MC_DETERMINISTIC")
}