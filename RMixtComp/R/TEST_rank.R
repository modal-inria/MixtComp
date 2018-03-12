testRank <- function() {
  var <- list()
  var$z_class <- zParam()
  
  var$Rank1 <- rankParam("Rank1") # validated

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

rankParam <- function(name) {
  Rank <- list()
  Rank$param <- list()
  Rank$param[[1]] <- list()
  Rank$param[[2]] <- list()
  
  Rank$name <- name
  Rank$type <- "Rank"
  Rank$generator <- rankGenerator
  Rank$param[[1]]$mu <- c(1, 2, 3, 4)
  Rank$param[[1]]$pi <- 0.8
  Rank$param[[2]]$mu <- c(4, 3, 2, 1)
  Rank$param[[2]]$pi <- 0.8
  
  return(Rank)
}