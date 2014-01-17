testRMC <- function (){
  lm <- getData(c("data/composite1.csv", "data/descriptor1.csv"),
                c("data/composite2.csv", "data/descriptor2.csv"))
  
  mcCluster <- getMixtCompCluster(2, 20, 100)
  mixtCompCluster(lm, mcCluster, 3)
  return(mcCluster)
}

testRMCZ <- function (){
  lm <- getDataZ(c("data/composite1.csv.gz", "data/descriptor1.csv.gz"),
                c("data/composite2.csv.gz", "data/descriptor2.csv.gz"))
  
  mcCluster <- getMixtCompCluster(2, 20, 100)
  mixtCompCluster(lm, mcCluster, 3)
  return(mcCluster)
}

testRMixtCompDev <- function ()
{
  lm <- getData(c("data/composite1.csv", "data/descriptor1.csv"),
                c("data/composite2.csv", "data/descriptor2.csv"))
  
  mcCluster <- getMixtCompCluster(2, 20, 100)
#   mixtCompCluster(lm, mcCluster, 3)
#   return(mcCluster)
}