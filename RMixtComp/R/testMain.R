testGenData <- function()
{
  dataGenerator()
#   lm <- getData(c("categoricalData.csv", "categoricalDescriptor.csv"))
#   lm <- getData(c("gaussianData.csv", "gaussianDescriptor.csv"))
  lm <- getData(c("gaussianData.csv", "gaussianDescriptor.csv"),
                c("categoricalData.csv", "categoricalDescriptor.csv"))
  
  mcCluster <- getMixtCompCluster(2, 20, 100)
  mixtCompCluster(lm, mcCluster, 2)
  return(mcCluster)
}

testRMC <- function (){
#  lm <- getData(c("data/gaussianData.csv", "data/gaussianDescriptor.csv"),
#                c("data/categoricalData.csv", "data/categoricalDescriptor.csv"))
  lm <- getData(c("data/gaussianData.csv", "data/gaussianDescriptor.csv"))
  mcCluster <- getMixtCompCluster(2, 20, 100)
  mixtCompCluster(lm, mcCluster, 2)
  return(mcCluster)
}