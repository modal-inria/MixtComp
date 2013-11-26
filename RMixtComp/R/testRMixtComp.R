testRMixtComp <- function (){
  data(composite1)
  data(composite2)
  
  lm <- list()
  
  lm <- addIngredient(lm, composite1$var1, "Gamma_ajk_bjk")
  lm <- addIngredient(lm, composite1$var2, "Gaussian_sjk")
 
  mcStrategy <- new("Strategy","randomInit", 2, 20, 100)
  
  mixtCompCluster(lm, mcStrategy, 3)
}