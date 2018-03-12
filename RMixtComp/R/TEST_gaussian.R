testGaussian <- function() {
  var <- list()
  var$z_class <- zParam()
  
  var$Gaussian1 <- gaussianParam("Gaussian1") # validated
  
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

gaussianParam <- function(name) {
  gaussian <- list()
  gaussian$name <- name
  gaussian$type <- "Gaussian_sjk"
  gaussian$generator <- gaussianGenerator
  gaussian$param <- list()
  gaussian$param[[1]] <- list(mean = -5, sd = 1)
  gaussian$param[[2]] <- list(mean = 5, sd = 1)
  
  return(gaussian)
}