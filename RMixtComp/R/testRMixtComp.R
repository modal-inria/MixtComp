testRMixtComp <- function (){
  mm1 <- new("MixtureModel", matrix(c(1.,2.,3.,4.,5.,6.),2), "Gaussian", "num")
  mm2 <- new("MixtureModel", matrix(c(1.,2.,3.,4.,5.,6.),2), "Gaussian", "num")
  
  listMixtureModel <- list(mm1,mm2)
  
  mainMixtComp(listMixtureModel)
}