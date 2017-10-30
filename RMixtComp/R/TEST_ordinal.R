testOrdinal <- function() {
  var <- list()
  var$z_class <- zParam()
  
  # var$Functional1 <- functionalParam2sub("Functional1")
  # var$Functional1 <- functionalInterPolyParam("Functional1") # validated
  
  var$Ordinal1 <- ordinalParamRandom("Ordinal1") # validated
  var$Ordinal2 <- ordinalParamRandom("Ordinal2")
  var$Ordinal3 <- ordinalParamRandom("Ordinal3")

  # var$Rank1 <- rankParam("Rank1") # validated
  
  # var$Categorical0 <- categoricalParamRandom("Categorical0")
  # var$Categorical1 <- categoricalParamRandom("Categorical1")
  # var$Categorical2 <- categoricalParamRandom("Categorical2")
  # var$Categorical3 <- categoricalParamRandom("Categorical3")
  # var$Categorical4 <- categoricalParamRandom("Categorical4")
  # var$Categorical5 <- categoricalParamRandom("Categorical5")
  # var$Categorical6 <- categoricalParamRandom("Categorical6")
  # var$Categorical7 <- categoricalParamRandom("Categorical7")
  # var$Categorical8 <- categoricalParamRandom("Categorical8")
  # var$Categorical9 <- categoricalParamRandom("Categorical9")
  # var$Categorical10 <- categoricalParamRandom("Categorical10")
  # var$Categorical11 <- categoricalParamRandom("Categorical11")
  # var$Categorical12 <- categoricalParamRandom("Categorical12")
  # var$Categorical13 <- categoricalParamRandom("Categorical13")
  # var$Categorical14 <- categoricalParamRandom("Categorical14")
  # var$Categorical15 <- categoricalParamRandom("Categorical15")
  # var$Categorical16 <- categoricalParamRandom("Categorical16")
  # var$Categorical17 <- categoricalParamRandom("Categorical17")
  # var$Categorical18 <- categoricalParamRandom("Categorical18")
  # var$Categorical19 <- categoricalParamRandom("Categorical19")
  
  # var$Gaussian1 <- gaussianParam("Gaussian1") # validated
  
  # var$Poisson0 <- poissonParamRandom("Poisson0")
  # var$Poisson1 <- poissonParamRandom("Poisson1")
  # var$Poisson2 <- poissonParamRandom("Poisson2")
  # var$Poisson3 <- poissonParamRandom("Poisson3")
  # var$Poisson4 <- poissonParamRandom("Poisson4")
  
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