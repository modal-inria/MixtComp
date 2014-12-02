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

testLearnPredict <- function()
{
  cat("Launching testGenDataLearn\n")
  myDataLearn <- testGenDataLearn()
  cat("Launching testGenDataPredict\n")
  myDataPredict <- testGenDataPredict(myDataLearn[[1]]@results@proportions,
                                      myDataLearn[[2]]$param)
  
  return(myDataPredict)
}

testGenDataLearn <- function(nbClass = 2,
                             nbBurnInIter = 20,
                             nbSampleLearn = 500,
                             nbSamplePredict = 50,
                             confidenceLevel = 0.95,
                             regen = TRUE)
{
  missingCategorical <- c(0.8, # present
                          0.1, # missing
                          0.1) # missing finite value
  missingGaussian <- c(0.5, # present
                       0.1, # missing
                       0.1, # missing interval
                       0.1, # missing left unbounded
                       0.1) # missing right unbounded
#   missingCategorical <- c(1., # present
#                           0., # missing
#                           0.) # missing finite value
#   missingGaussian <- c(1., # present
#                        0., # missing
#                        0., # missing interval
#                        0., # missing left unbounded
#                        0.) # missing right unbounded
  if (regen == TRUE)
  {
    dataParamGenerator(nbSampleLearn, # nbSamples
                       nbSamplePredict, # nbSamplePredict
                       2, # nbVariablesCat
                       3, # minModality
                       6, # nbModalities
                       2, # nbVariablesGauss
                       50., # maxMean
                       10., # maxVar
                       nbClass, # nbClasses
                       missingCategorical, # missingCategorical
                       missingGaussian) # missingGaussian
  }
  
  lm <- getData(c("dataGen/learn/gaussianData.csv",
                  "dataGen/learn/gaussianDescriptor.csv"),
                c("dataGen/learn/categoricalData.csv",
                  "dataGen/learn/categoricalDescriptor.csv"))
  
  # creation of parameters container
  mcCluster <- getMixtCompCluster(2, # nbTrialInInit
                                  nbBurnInIter, # nbBurnInIter
                                  100, # nbIter
                                  20, # nbGibbsBurnInIter
                                  100) # nbGibbsIter
  
  # launch of the MixtComp algorithm
  dataParam <- mixtCompCluster(lm,
                               mcCluster,
                               nbClass,
                               confidenceLevel)
  return(list(mcCluster,
              dataParam))
}


testGenDataPredict <- function(prop,
                               param,
                               nbClass = 2,
                               nbBurnInIter = 20,
                               confidenceLevel = 0.95)
{
  lm <- getData(c("dataGen/predict/gaussianData.csv",
                  "dataGen/predict/gaussianDescriptor.csv"),
                c("dataGen/predict/categoricalData.csv",
                  "dataGen/predict/categoricalDescriptor.csv"))
  
  # creation of parameters container
  mcCluster <- getMixtCompCluster(2, # nbTrialInInit
                                  nbBurnInIter, # nbBurnInIter
                                  100, # nbIter
                                  20, # nbGibbsBurnInIter
                                  100) # nbGibbsIter
  
  # launch of the MixtComp algorithm
  dataParam <- mixtCompPredict(lm,
                               prop,
                               param,
                               mcCluster,
                               nbClass,
                               confidenceLevel)
  return(list(mcCluster,
              dataParam))
}

testRMC <- function (){
  lm <- getData(c("data/gaussianData.csv", "data/gaussianDescriptor.csv"),
                c("data/categoricalData.csv", "data/categoricalDescriptor.csv"))
#  lm <- getData(c("data/gaussianData.csv", "data/gaussianDescriptor.csv"))
#  lm <- getData(c("data/categoricalData.csv", "data/categoricalDescriptor.csv"))

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