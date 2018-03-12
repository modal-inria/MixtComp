testCategorical <- function() {
  var <- list()
  var$z_class <- zParam()
  
  var$Categorical0 <- categoricalParamRandom("Categorical0")
  var$Categorical1 <- categoricalParamRandom("Categorical1")
  var$Categorical2 <- categoricalParamRandom("Categorical2")
  var$Categorical3 <- categoricalParamRandom("Categorical3")
  var$Categorical4 <- categoricalParamRandom("Categorical4")
  var$Categorical5 <- categoricalParamRandom("Categorical5")
  var$Categorical6 <- categoricalParamRandom("Categorical6")
  var$Categorical7 <- categoricalParamRandom("Categorical7")
  var$Categorical8 <- categoricalParamRandom("Categorical8")
  var$Categorical9 <- categoricalParamRandom("Categorical9")
  var$Categorical10 <- categoricalParamRandom("Categorical10")
  var$Categorical11 <- categoricalParamRandom("Categorical11")
  var$Categorical12 <- categoricalParamRandom("Categorical12")
  var$Categorical13 <- categoricalParamRandom("Categorical13")
  var$Categorical14 <- categoricalParamRandom("Categorical14")
  var$Categorical15 <- categoricalParamRandom("Categorical15")
  var$Categorical16 <- categoricalParamRandom("Categorical16")
  var$Categorical17 <- categoricalParamRandom("Categorical17")
  var$Categorical18 <- categoricalParamRandom("Categorical18")
  var$Categorical19 <- categoricalParamRandom("Categorical19")
  
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
  
  # print(resLearn$mixture$warnLog)
  
  confMatSampled <- table(dat$z, resLearn$variable$data$z_class$completed)
  print(confMatSampled)
}

categoricalParam1 <- function(name) {
  categorical <- list()
  categorical$name <- name
  categorical$type <- "Categorical_pjk"
  categorical$generator <- categoricalGenerator 
  categorical$param <- list()
  categorical$param[[1]] <- c(0.1, 0.2, 0.2, 0.5)
  categorical$param[[2]] <- c(0.5, 0.3, 0.1, 0.1)
  
  return(categorical)
}

categoricalParam2 <- function(name) {
  categorical <- list()
  categorical$name <- name
  categorical$type <- "Categorical_pjk"
  categorical$generator <- categoricalGenerator 
  categorical$param <- list()
  categorical$param[[1]] <- c(0.6, 0.1, 0.1, 0.2)
  categorical$param[[2]] <- c(0.1, 0.05, 0.8, 0.05)
  
  return(categorical)
}

categoricalParamRandom <- function(name) {
  nbModalities <- 4
  param1 <- runif(4)
  param1 <- param1 / sum(param1)
  param2 <- runif(4)
  param2 <- param2 / sum(param2)
  
  categorical <- list()
  categorical$name <- name
  categorical$type <- "Categorical_pjk"
  categorical$generator <- categoricalGenerator 
  categorical$param <- list()
  categorical$param[[1]] <- param1
  categorical$param[[2]] <- param2
  
  return(categorical)
}