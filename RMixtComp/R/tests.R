testRMixtComp <- function (){
  data(composite1)
  data(composite2)
  
  lm <- list()
  
  lm <- addIngredient(lm, composite1$var1, "Gamma_ajk_bjk")
  lm <- addIngredient(lm, composite1$var2, "Gaussian_sjk")
 
  mcStrategy <- mixtCompSemStrategy(2, 20, 100)
  mcCluster <- getMixtCompCluster(mcStrategy)
  
  mixtCompCluster(lm, mcCluster, 3)
  return(mcCluster)
}

testParseCsv <- function (){
   parseCsv("data/missValues.csv")
}