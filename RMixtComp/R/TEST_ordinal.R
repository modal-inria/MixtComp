testOrdinal <- function() {
  var <- list()
  var$z_class <- zParam()
  
  var$Ordinal1 <- ordinalParamRandom("Ordinal1") # validated
  var$Ordinal2 <- ordinalParamRandom("Ordinal2")
  var$Ordinal3 <- ordinalParamRandom("Ordinal3")
  
  dat <- dataGenerator(1000, 0.9, var)
  
  resGetData <- getData(list(dat$data, dat$descriptor))
  
  # define the algorithm's parameters
  mcStrategy <- list(
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 50,
    nbGibbsIter = 50,
    ratioInitialization = 0.2)
  
  # run RMixtCompt for clustering
  resLearn <- mixtCompCluster(
    resGetData$lm,
    mcStrategy,
    nbClass = 2,
    confidenceLevel = 0.95)
  
  confMatSampled <- table(dat$z, resLearn$variable$data$z_class$completed)
  print(confMatSampled)
}

ordinalParam1 <- function(name) {
  Ordinal <- list()
  Ordinal$name <- name
  Ordinal$type <- "Ordinal"
  Ordinal$generator <- ordinalGenerator
  
  Ordinal$param <- list()
  Ordinal$param[[1]] <- list(nbMod = 4, mu = 1, pi = 0.8)
  Ordinal$param[[2]] <- list(nbMod = 4, mu = 4, pi = 0.8)
  
  return(Ordinal)
}

ordinalParam2 <- function(name) {
  Ordinal <- list()
  Ordinal$name <- name
  Ordinal$type <- "Ordinal"
  Ordinal$generator <- ordinalGenerator
  
  Ordinal$param <- list()
  Ordinal$param[[1]] <- list(nbMod = 4, mu = 5, pi = 0.8)
  Ordinal$param[[2]] <- list(nbMod = 4, mu = 2, pi = 0.8)
  
  return(Ordinal)
}

ordinalParamRandom <- function(name) {
  nMod <- 4
  
  Ordinal <- list()
  Ordinal$name <- name
  Ordinal$type <- "Ordinal"
  Ordinal$generator <- ordinalGenerator
  
  Ordinal$param <- list()
  Ordinal$param[[1]] <- list(nbMod = nMod, mu = sample(1:nMod, 1), pi = runif(1))
  Ordinal$param[[2]] <- list(nbMod = nMod, mu = sample(1:nMod, 1), pi = runif(1))
  
  return(Ordinal)
}