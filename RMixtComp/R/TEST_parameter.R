# This file contains functions to create the parameter object in the different examples and test of the package

zParam <- function() {
  z_class <- list() # z_class must be the first variable in the list
  
  z_class$name <- "z_class"
  z_class$type <- "LatentClass"
  z_class$param <- c(0.3, 0.7) # parameters for z_class are the mixture proportions
  #  z_class$param <- c(1.) # parameters for z_class are the mixture proportions
  z_class$allPresent <- FALSE
  z_class$allMissing <- TRUE
  z_class$fixedInitialization <- FALSE
  
  return(z_class)
}

zParamFixedInit <- function() {
  z_class <- list() # z_class must be the first variable in the list
  
  z_class$name <- "z_class"
  z_class$type <- "LatentClass"
  z_class$param <- c(0.3, 0.7) # parameters for z_class are the mixture proportions
  #  z_class$param <- c(1.) # parameters for z_class are the mixture proportions
  z_class$allPresent <- TRUE
  z_class$allMissing <- FALSE
  z_class$fixedInitialization <- TRUE
  z_class$paramStr <- "fixedInitialization"
  
  return(z_class)
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