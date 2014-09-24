testGenData <- function()
{
  missingCategorical <- c(0.8, # present
                          0.1, # missing
                          0.1) # missing finite value
  missingGaussian <- c(0.5, # present
                       0.1, # missing
                       0.1, # missing interval
                       0.1, # missing left unbounded
                       0.1) # missing right unbounded
  dataGenerator(c(0.5, 0.5), # proportions
                500, # nbSamples
                6, # nbVariablesCat
                1, # minModality
                3, # nbModalities
                2, # nbVariablesGauss
                50., # maxMean
                10., # maxVar
                2, # nbClasses
                missingCategorical, # missingCategorical
                missingGaussian) # missingGaussian
  
#  lm <- getData(c("categoricalData.csv", "categoricalDescriptor.csv"))
#  lm <- getData(c("dataGen/gaussianData.csv", "dataGen/gaussianDescriptor.csv"))
  lm <- getData(c("dataGen/gaussianData.csv"   , "dataGen/gaussianDescriptor.csv"   ),
                c("dataGen/categoricalData.csv", "dataGen/categoricalDescriptor.csv"))
  
  mcCluster <- getMixtCompCluster(2, # nbTrialInInit
                                  20, # nbBurnInIter
                                  100, # nbIter
                                  20, # nbGibbsBurnInIter
                                  100) # nbGibbsIter
  data <- list()
  param <- list()
  dataParam <- mixtCompCluster(lm,
                               mcCluster,
                               2)
  return(list(mcCluster,
              dataParam))
}

exportData <- function(dataOut)
{
  write.table(dataOut[[1]]@results@partition,
              file = "out/classOut.csv",
              row.names=FALSE,
              col.names=FALSE)
  
  write.table(data.frame(categorical1 = dataOut[[2]]$data$categorical1$completed),
              file = "out/categorical1data.csv",
              row.names = FALSE,
              col.names = FALSE)
  write.table(data.frame(categorical1 = dataOut[[2]]$param$categorical1),
              file = "out/categorical1param.csv",
              sep = ";",
              row.names = FALSE,
              col.names = TRUE)
  
  write.table(data.frame(gaussian1 = dataOut[[2]]$data$gaussian1$completed),
              file = "out/gaussian1data.csv",
              row.names = FALSE,
              col.names = FALSE)
  write.table(data.frame(categorical1 = dataOut[[2]]$param$gaussian1),
              file = "out/gaussian1param.csv",
              sep = ";",
              row.names = FALSE,
              col.names = TRUE)
}

exportMap <- function(nbIterations)
{
  classIn <- read.table("dataGen/classIn.csv",
                        sep = ";")
  nbInd <- length(classIn$V1)
  
  for (i in -1:(nbIterations-1))
  {
    nbMisClass <- 0
    
    fileNamez_i <- paste("out/log/composer-",
                         i,
                         "-z_i.csv",
                         sep = "")
    z_i <- read.table(fileNamez_i,
                      sep = ";")
    
    fileNameVar1 <- paste("out/log/gaussian1-",
                          i,
                          "-data.csv",
                          sep = "")
    var1 <- read.table(fileNameVar1,
                       sep = ";")
    
    fileNameVar2 <- paste("out/log/gaussian2-",
                          i,
                          "-data.csv",
                          sep = "")
    var2 <- read.table(fileNameVar2,
                       sep = ";")
    
    param1 <- read.table(paste("out/log/gaussian1-",
                               i,
                               "-param.csv",
                               sep = ""),
                         sep = ";")
    param2 <- read.table(paste("out/log/gaussian2-",
                               i,
                               "-param.csv",
                               sep = ""),
                         sep = ";")
    
    for (ind in 1:nbInd)
    {
      if ((z_i$V1[ind] + 1) != classIn$V1[ind])
      {
        nbMisClass = nbMisClass + 1
      }
    }
    
    colPool <- c("red", "blue", "green")
    png(paste("out/graph/", i, ".png", sep = ""),
        width = 1000,
        height = 1000)
    plot(var1$V1,
         var2$V1,
#         ylim = c(-150., 150.),
#         xlim = c(-150., 150.),
         col = colPool[z_i$V1 + 1])
    text(-20,
         0,
         paste("Misclassified / Total: ", nbMisClass, " / ", nbInd, "\n",
               "Misclassification rate: ", nbMisClass / nbInd, "\n",
               "Var 1:\n",
               "k=1 - exp: ", param1$V1[1], "\n",
               "k=1 - sd: ", param1$V1[2], "\n",
               "k=2 - exp: ", param1$V1[3], "\n",
               "k=2 - sd: ", param1$V1[4], "\n",
               "Var 2:\n",
               "k=1 - exp: ", param2$V1[1], "\n",
               "k=1 - sd: ", param2$V1[2], "\n",
               "k=2 - exp: ", param2$V1[3], "\n",
               "k=2 - sd: ", param2$V1[4], "\n",
               sep = ""),
         adj = 0.)
    
    dev.off()
  }
}

exportGraph2 <- function(nbIterations)
{
  classIn <- read.table("dataGen/classIn.csv",
                        sep = ";")
  nbInd <- length(classIn$V1)
  
  misClass <- vector(mode = "integer", length = nbIterations)
  
  for (i in -1:(nbIterations-1))
  {
    fileNamez_i <- paste("out/log/composer-",
                         i,
                         "-z_i.csv",
                         sep = "")
    z_i <- read.table(fileNamez_i,
                      sep = ";")
    
    nbMisClass <- 0
    for (ind in 1:nbInd)
    {
      if ((z_i$V1[ind] + 1) != classIn$V1[ind])
      {
        nbMisClass = nbMisClass + 1
      }
    }
    misClass[i + 1] <- nbMisClass
  }
  plot(misClass,
       ylim = c(0., 50.),
       type = "l")
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