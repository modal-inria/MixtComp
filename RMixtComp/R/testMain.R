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

testGenData <- function(nbBurnInIter = 20,
                        nbSample = 500,
                        confidenceLevel = 0.95,
                        regen = TRUE)
{
  nbClass <- 2
  missingCategorical <- c(0.8, # present
                          0.1, # missing
                          0.1) # missing finite value
  missingGaussian <- c(0.5, # present
                       0.1, # missing
                       0.1, # missing interval
                       0.1, # missing left unbounded
                       0.1) # missing right unbounded
  if (regen == TRUE)
  {
    dataGenerator(c(0.5, 0.5), # proportions
                  nbSample, # nbSamples
                  0, # nbVariablesCat
                  3, # minModality
                  6, # nbModalities
                  2, # nbVariablesGauss
                  50., # maxMean
                  10., # maxVar
                  nbClass, # nbClasses
                  missingCategorical, # missingCategorical
                  missingGaussian) # missingGaussian    
  }
  
  # read and parse the data
#  lm <- getData(c("categoricalData.csv", "categoricalDescriptor.csv"))
  lm <- getData(c("dataGen/gaussianData.csv", "dataGen/gaussianDescriptor.csv"))
#  lm <- getData(c("dataGen/gaussianData.csv"   , "dataGen/gaussianDescriptor.csv"   ),
#                c("dataGen/categoricalData.csv", "dataGen/categoricalDescriptor.csv"))
  
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