testComplete <- function()
{
  nbBurnInIter <- 20
  nbSample <- 500
  myData <- testGenData(nbBurnInIter, nbSample)
  confidenceInterval(myData, nbSample)
  completedData(myData)
  exportParam(myData)
  exportMisClass(nbBurnInIter)
  export2DPoints(nbBurnInIter)
}

testLearnPredict <- function(regen = TRUE)
{
  cat("Launching testGenDataLearn\n")
  res <- testGenDataLearn(regen = regen)
  cat("Launching testGenDataPredict\n")
  if (nchar(res$mixture$warnLog) == 0)
  {
    myDataPredict <- testGenDataPredict(res$variable$param)
    return(myDataPredict)
  }
}

testGenDataLearn <- function(nbClass = 2,
                             nbBurnInIter = 100,
                             nbSampleLearn = 50,
                             nbSamplePredict = 50,
                             confidenceLevel = 0.95,
                             regen = TRUE)
{  
  missingZ <- c(0.3, # present
                0.4, # missing
                0.3) # missing finite value
  
  missingCategorical <- c(0.8, # present
                          0.1, # missing
                          0.1) # missing finite value
  
  missingGaussian <- c(0.6, # present
                       0.1, # missing
                       0.1, # missing interval
                       0.1, # missing left unbounded
                       0.1) # missing right unbounded
  
  missingPoisson <- c(0.8, # present
                      0.2) # missing
  
  #   missingZ <- c(0., # present
  #                 1., # missing
  #                 0.) # missing finite value
  #   
  #   missingCategorical <- c(1., # present
  #                           0., # missing
  #                           0.) # missing finite value
  # 
  #   missingGaussian <- c(1., # present
  #                        0., # missing
  #                        0., # missing interval
  #                        0., # missing left unbounded
  #                        0.) # missing right unbounded
  #   
  #   missingPoisson <- c(1., # present
  #                       0.) # missing
  
  if (regen == TRUE)
  {
    dataParamGenerator(nbSampleLearn, # nbSamples
                       nbSamplePredict, # nbSamplePredict
                       3, # nbVariablesCat
                       6, # nbModalities
                       3, # nbVariablesGauss
                       0.5, # maxMean
                       0.1, # maxSD
                       3, # nbVariablePois
                       20., # maxLambda
                       nbClass, # nbClasses
                       missingZ,
                       missingCategorical, # missingCategorical
                       missingGaussian,
                       missingPoisson) # missingGaussian
  }
  
#    resGetData <- getData(c("dataGen/learn/zData.csv",
#                            "dataGen/learn/zDescriptor.csv"),
#                          c("dataGen/learn/gaussianData.csv",
#                           "dataGen/learn/gaussianDescriptor.csv"),
#                          c("dataGen/learn/categoricalData.csv",
#                            "dataGen/learn/categoricalDescriptor.csv"),
#                          c("dataGen/learn/poissonData.csv",
#                            "dataGen/learn/poissonDescriptor.csv"))
   
   resGetData <- getData(c("dataGen/learn/gaussianData.csv",
                           "dataGen/learn/gaussianDescriptor.csv"),
                         c("dataGen/learn/categoricalData.csv",
                           "dataGen/learn/categoricalDescriptor.csv"),
                         c("dataGen/learn/poissonData.csv",
                           "dataGen/learn/poissonDescriptor.csv"))

#   lm <- getData(c("dataGen/learn/gaussianData.csv",
#                   "dataGen/learn/gaussianDescriptor.csv"))

#   lm <- getData(c("dataGen/learn/poissonData.csv",
#                   "dataGen/learn/poissonDescriptor.csv"))

#   lm <- getData(c("dataGen/learn/categoricalData.csv",
#                   "dataGen/learn/categoricalDescriptor.csv"))
  
  # creation of strategy list
  mcStrategy <- list(nbBurnInIter = nbBurnInIter,
                     nbIter = 100,
                     nbGibbsBurnInIter = 100,
                     nbGibbsIter = 100)
  
  # launch of the MixtComp algorithm
  res <- mixtCompCluster(resGetData$lm,
                         mcStrategy,
                         nbClass,
                         confidenceLevel)
  if (nchar(res$mixture$warnLog) > 0)
  {
    warning(res$mixture$warnLog)
  }
  
  confMat <- confusionMatrix(nbClass,
                             "dataGen/learn/classIn.csv",
                             res$variable)
  print(confMat)
  cat("lnObservedLikelihood: "     , res$mixture$lnObservedLikelihood     , "\n",
      "lnSemiCompletedLikelihood: ", res$mixture$lnSemiCompletedLikelihood, "\n",
      "lnCompletedLikelihood: "    , res$mixture$lnCompletedLikelihood    , "\n",
      sep = "")
  
  return(res)
}

testGenDataPredict <- function(param,
                               nbClass = 2,
                               nbBurnInIter = 20,
                               confidenceLevel = 0.95)
{
  #   resGetData <- getData(c("dataGen/predict/zData.csv",
  #                           "dataGen/predict/zDescriptor.csv"),
  #                         c("dataGen/predict/gaussianData.csv",
  #                           "dataGen/predict/gaussianDescriptor.csv"),
  #                         c("dataGen/predict/categoricalData.csv",
  #                           "dataGen/predict/categoricalDescriptor.csv"),
  #                         c("dataGen/predict/poissonData.csv",
  #                           "dataGen/predict/poissonDescriptor.csv"))

  resGetData <- getData(c("dataGen/predict/gaussianData.csv",
                          "dataGen/predict/gaussianDescriptor.csv"),
                        c("dataGen/predict/categoricalData.csv",
                          "dataGen/predict/categoricalDescriptor.csv"),
                        c("dataGen/predict/poissonData.csv",
                          "dataGen/predict/poissonDescriptor.csv"))
  
  #   lm <- getData(c("dataGen/predict/gaussianData.csv",
  #                   "dataGen/predict/gaussianDescriptor.csv"))
  
  #   lm <- getData(c("dataGen/predict/poissonData.csv",
  #                   "dataGen/predict/poissonDescriptor.csv"))
  
  #   lm <- getData(c("dataGen/learn/categoricalData.csv",
  #                   "dataGen/learn/categoricalDescriptor.csv"))
  
  # creation of strategy list
  mcStrategy <- list(nbBurnInIter = nbBurnInIter,
                     nbIter = 100,
                     nbGibbsBurnInIter = 100,
                     nbGibbsIter = 100)
  
  # launch of the MixtComp algorithm
  res <- mixtCompPredict(resGetData$lm,
                         param,
                         mcStrategy,
                         nbClass,
                         confidenceLevel)
  
  if (nchar(res$mixture$warnLog) > 0)
  {
    warning(res$mixture$warnLog)
  }
  
  confMat <- confusionMatrix(nbClass,
                             "dataGen/predict/classIn.csv",
                             res$variable)
  print(confMat)
  cat("lnObservedLikelihood: "     , res$mixture$lnObservedLikelihood     , "\n",
      "lnSemiCompletedLikelihood: ", res$mixture$lnSemiCompletedLikelihood, "\n",
      "lnCompletedLikelihood: "    , res$mixture$lnCompletedLikelihood    , "\n",
      sep = "")
  
  return(res)
}

confusionMatrix <- function(nbClass,
                            classInFile,
                            dataParam)
{
  classIn <- read.csv2(classInFile,
                       header = FALSE,
                       dec = ".")
  matConf <- matrix(0,
                    nbClass,
                    nbClass)
  for (i in 1:nrow(classIn))
  {
    matConf[classIn[i, 1],
            dataParam$data$z_class$completed[i]] = matConf[classIn[i, 1],
                                                           dataParam$data$z_class$completed[i]] + 1
  }
  
  return(matConf)
}

testGeyser <- function()
{
  lm <- getData(c("data/faithful.csv", "data/faithfulDescriptor.csv"))
  mcCluster <- getMixtCompCluster(2, # nbTrialInInit
                                  20, # nbBurnInIter
                                  100, # nbIter
                                  20, # nbGibbsBurnInIter
                                  100) # nbGibbsIter
  data <- list()
  param <- list()
  dataParam <- mixtCompCluster(lm,
                               mcCluster,
                               3)
  return(list(mcCluster,
              dataParam))
}

testAERES <- function()
{
  nbClass <- 2
  confidenceLevel <- 0.95
  
  resGetData <- getData(c("dataTest/AERES/data.csv",
                          "dataTest/AERES/desc.csv"))
  
  mcStrategy <- list(nbBurnInIter = 100,
                     nbIter = 100,
                     nbGibbsBurnInIter = 100,
                     nbGibbsIter = 100)
  
  # launch of the MixtComp algorithm
  res <- mixtCompCluster(resGetData$lm,
                         mcStrategy,
                         nbClass,
                         confidenceLevel)
}