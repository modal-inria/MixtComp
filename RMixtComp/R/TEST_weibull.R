testWeibull <- function() {
  var <- list()
  var$z_class <- zParam()
  
  var$Gaussian1 <- weibullParam("Gaussian1")
  
  dat <- dataGenerator(1000, 0.9, var)
  
  resGetData <- getData(list(dat$data, dat$descriptor))
  
  # define the algorithm's parameters
  mcStrategy <- list(
    nbBurnInIter = 100,
    nbIter = 100,
    nbGibbsBurnInIter = 50,
    nbGibbsIter = 50,
    parameterEdgeAuthorized = TRUE)
  
  # run RMixtCompt for clustering
  resLearn <- mixtCompCluster(
    resGetData$lm,
    mcStrategy,
    nbClass = 2,
    confidenceLevel = 0.95)
  
  confMatSampled <- table(dat$z, resLearn$variable$data$z_class$completed)
  print(confMatSampled)
}

weibullParam <- function(name) {
  weibull <- list()
  weibull$name <- name
  weibull$type <- "Weibull"
  weibull$generator <- weibullGenerator
  weibull$param <- list()
  weibull$param[[1]] <- list(shape = 2.0, scale = 0.5)
  weibull$param[[2]] <- list(shape = 4.0, scale = 3.0)
  
  return(weibull)
}