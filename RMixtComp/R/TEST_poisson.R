testPoisson <- function() {
  var <- list()
  var$z_class <- zParam()
  
  var$Poisson0 <- poissonParamRandom("Poisson0")
  var$Poisson1 <- poissonParamRandom("Poisson1")
  var$Poisson2 <- poissonParamRandom("Poisson2")
  var$Poisson3 <- poissonParamRandom("Poisson3")
  var$Poisson4 <- poissonParamRandom("Poisson4")
  
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

poissonParam <- function(name) {
  poisson <- list()
  poisson$name <- name
  poisson$type <- "Poisson_k"
  poisson$generator <- poissonGenerator
  poisson$param <- list()
  poisson$param[[1]] <- 1
  poisson$param[[2]] <- 10
  
  return(poisson)
}

poissonParamRandom <- function(name) {
  valMin <- 0.1
  valMax <- 15.0
  
  poisson <- list()
  poisson$name <- name
  poisson$type <- "Poisson_k"
  poisson$generator <- poissonGenerator
  poisson$param <- list()
  poisson$param[[1]] <- runif(n = 1, min = valMin, max = valMax)
  poisson$param[[2]] <- runif(n = 1, min = valMin, max = valMax)
  
  return(poisson)
}