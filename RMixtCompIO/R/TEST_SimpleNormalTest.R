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
 

# Simple test case for the new IO sytem
# Sampling law is mixture model with 0.5 0.5 proportions
# First class conditional law is N(5., 1.), rnorm(10, 5., 1.)
# Second class conditional law is N(20., 2.), rnorm(10, 20., 2.)
# @author Vincent Kubicki
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
    nStableCriterion = 5,
    ratioStableCriterion = 0.9,
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
  partition <- resLearn$variable$data$z_class$completed
  paste0("rand: ", rand.index(partition, zLearn)) # expected 0.9 rand
  print("contengency: ")
  print(table(zLearn, partition))
  
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
    nStableCriterion = 5,
    ratioStableCriterion = 0.9,
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
  partition <- resPredict$variable$data$z_class$completed
  paste0("rand: ", rand.index(partition, zPredict)) # expected 0.9 rand
  print("contengency: ")
  print(table(zPredict, partition))
  
  Sys.unsetenv("MC_DETERMINISTIC")
}