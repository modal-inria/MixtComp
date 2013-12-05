testRMixtComp <- function (){
  lm <- getData("data/composite1.csv", "data/descriptor1.csv")
  
  mcCluster <- getMixtCompCluster(2, 20, 100)
  mixtCompCluster(lm, mcCluster, 3)
  return(mcCluster)
}

testGetData <- function(){
  data <- getData("data/composite1.csv", "data/descriptor1.csv")
}

testParseCsv <- function (){
   parseCsv("data/missValues.csv")
}