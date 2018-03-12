testFunctional <- function() {
  var <- list()
  var$z_class <- zParam()
  
  var$Functional1 <- functionalParam2sub("Functional1")
  var$Functional1 <- functionalInterPolyParam("Functional1") # validated
  
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

functionalInterPolyParam <- function(name) {
  Functional <- list(
    name = name,
    type = "Functional",
    generator = functionalInterPolyGenerator,
    paramStr = "nSub: 2, nCoeff: 2",
    param = list())
  
  Functional$param[[1]] <- list(
    x = c(0., 10., 20.),
    y = c(0., 10., 0.),
    sd = 0.1,
    tMin = 0.,
    tMax = 20.,
    nTime = 100)
  
  Functional$param[[2]] <- list(
    x = c(0., 10., 20.),
    y = c(10., 0., 10.),
    sd = 0.1,
    tMin = 0.,
    tMax = 20.,
    nTime = 100)
  
  return(Functional)
}

functionalParam1sub <- function(name) {
  Functional <- list()
  Functional$param <- list()
  Functional$param[[1]] <- list()
  Functional$param[[2]] <- list()
  
  Functional$name <- name
  Functional$type <- "Functional"
  Functional$generator <- functionalGenerator
  Functional$paramStr <- "nSub: 1, nCoeff: 2"
  
  coeffAlpha1 <- getLinEq(c(0.,  100.), c(25., 0.)) # first sub
  
  coeffBeta11 <- getLinEq(c(0., 10.), c(50., 10.)) # first class, first sub
  
  coeffBeta21 <- getLinEq(c(0., 0.), c(50.,  0.)) # second class, first sub
  
  alpha <- matrix(c(coeffAlpha1[1], coeffAlpha1[2]),
                  1, 2, byrow = TRUE)
  
  beta1 <- matrix(c(coeffBeta11[1], coeffBeta11[2]),
                  1, 2, byrow = TRUE)
  
  beta2 <- matrix(c(coeffBeta21[1], coeffBeta21[2]),
                  1, 2, byrow = TRUE)
  
  sigma <- 0.01
  
  # print("alpha")
  # print(alpha)
  # print("beta1")
  # print(beta1)
  # print("beta2")
  # print(beta2)
  
  Functional$param[[1]]$alpha <- alpha
  Functional$param[[1]]$beta <- beta1
  Functional$param[[1]]$sigma <- c(sigma, sigma)
  Functional$param[[1]]$nTime <- 20
  Functional$param[[1]]$tMin <- 0.
  Functional$param[[1]]$tMax <- 50.
  
  Functional$param[[2]]$alpha <- alpha
  Functional$param[[2]]$beta <- beta2
  Functional$param[[2]]$sigma <- c(sigma, sigma)
  Functional$param[[2]]$nTime <- 20
  Functional$param[[2]]$tMin <- 0.
  Functional$param[[2]]$tMax <- 50.
  
  return(Functional)
}

functionalParam2sub <- function(name) {
  Functional <- list()
  Functional$param <- list()
  Functional$param[[1]] <- list()
  Functional$param[[2]] <- list()
  
  Functional$name <- name
  Functional$type <- "Functional"
  Functional$generator <- functionalGenerator
  Functional$paramStr <- "nSub: 2, nCoeff: 2"
  
  coeffAlpha1 <- getLinEq(c(0.,  100.), c(25., 0.)) # first sub
  coeffAlpha2 <- getLinEq(c(0., -100.), c(25., 0.)) # second sub
  
  coeffBeta11 <- getLinEq(c(0.,   0.), c(25., 10.)) # first class, first sub
  coeffBeta12 <- getLinEq(c(25., 10.), c(50.,  0.)) # first class, second sub
  
  coeffBeta21 <- getLinEq(c(0., 10.), c(25.,  0.)) # second class, first sub
  coeffBeta22 <- getLinEq(c(25., 0.), c(50., 10.)) # second class, second sub
  
  alpha <- matrix(c(coeffAlpha1[1], coeffAlpha1[2],
                    coeffAlpha2[1], coeffAlpha2[2]),
                  2, 2, byrow = TRUE)
  
  beta1 <- matrix(c(coeffBeta11[1], coeffBeta11[2],
                    coeffBeta12[1], coeffBeta12[2]),
                  2, 2, byrow = TRUE)
  
  beta2 <- matrix(c(coeffBeta21[1], coeffBeta21[2],
                    coeffBeta22[1], coeffBeta22[2]),
                  2, 2, byrow = TRUE)
  
  sigma <- 0.01
  
  # print("alpha")
  # print(alpha)
  # print("beta1")
  # print(beta1)
  # print("beta2")
  # print(beta2)
  
  Functional$param[[1]]$alpha <- alpha
  Functional$param[[1]]$beta <- beta1
  Functional$param[[1]]$sigma <- c(sigma, sigma)
  Functional$param[[1]]$nTime <- 20
  Functional$param[[1]]$tMin <- 0.
  Functional$param[[1]]$tMax <- 50.
  
  Functional$param[[2]]$alpha <- alpha
  Functional$param[[2]]$beta <- beta2
  Functional$param[[2]]$sigma <- c(sigma, sigma)
  Functional$param[[2]]$nTime <- 20
  Functional$param[[2]]$tMin <- 0.
  Functional$param[[2]]$tMax <- 50.
  
  return(Functional)
}

getLinEq <- function(pointA, pointB) {
  a <- matrix(c(1., pointA[1],
                1., pointB[1]),
              2, 2, byrow = TRUE)
  b <- c(pointA[2], pointB[2])
  x <- solve(a, b)
  
  return(x)
}