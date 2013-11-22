testRMixtComp <- function (){
  mm1 <- new("MixtureModel", matrix(c(1.,2.,3.,4.,5.,6.),2), "Gaussian")
  mm2 <- new("MixtureModel", matrix(c(1.,2.,3.,4.,5.,6.),2), "Gaussian")
  
  listMixtureModel <- list(mm1,mm2)
  
  mainMixtComp(listMixtureModel)
}