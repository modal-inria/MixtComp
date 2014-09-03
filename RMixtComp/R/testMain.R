testGenData <- function()
{
  dataGenerator6()
#   lm <- getData(c("categoricalData.csv", "categoricalDescriptor.csv"))
  lm <- getData(c("gaussianData.csv", "gaussianDescriptor.csv"))
#   lm <- getData(c("gaussianData.csv", "gaussianDescriptor.csv"),
#                 c("categoricalData.csv", "categoricalDescriptor.csv"))
  
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

exportMap <- function(nbIterations)
{
  for (i in -1:(nbIterations-1))
  {
    fileNamez_i <- paste("log/composer-",
                         i,
                         "-z_i.csv",
                         sep = "")
    z_i <- read.table(fileNamez_i,
                      sep = ";")
    
    fileNameVar1 <- paste("log/gaussian1-",
                          i,
                          "-data.csv",
                          sep = "")
    var1 <- read.table(fileNameVar1,
                       sep = ";")
    
    fileNameVar2 <- paste("log/gaussian2-",
                          i,
                          "-data.csv",
                          sep = "")
    var2 <- read.table(fileNameVar2,
                       sep = ";")
    
    param1 <- read.table(paste("log/gaussian1-",
                               i,
                               "-param.csv",
                               sep = ""),
                         sep = ";")
    param2 <- read.table(paste("log/gaussian2-",
                               i,
                               "-param.csv",
                               sep = ""),
                         sep = ";")
    
    colPool <- c("red", "blue", "green")
    png(paste("graph/", i, ".png", sep = ""),
        width = 1000,
        height = 1000)
    plot(var1$V1,
         var2$V1,
#         ylim = c(-150., 150.),
#         xlim = c(-150., 150.),
         col = colPool[z_i$V1 + 1])
    
    text(-20,
         0,
         paste("Var 1:\n",
               "k=1 - exp: ", param1$V1[1], "\n",
               "k=1 - sd: ", param1$V1[2], "\n",
               "k=2 - exp: ", param1$V1[3], "\n",
               "k=2 - sd: ", param1$V1[4], "\n",
               "Var 2:\n",
               "k=1 - exp: ", param2$V1[1], "\n",
               "k=1 - sd: ", param2$V1[2], "\n",
               "k=2 - exp: ", param2$V1[3], "\n",
               "k=2 - sd: ", param2$V1[4], "\n"),
         adj = 0.)
    
    dev.off()
  }
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