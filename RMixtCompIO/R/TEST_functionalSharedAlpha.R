# @author Vincent Kubicki
functionalSharedAlphaInterPolyParam <- function(name) {
  functionalSharedAlpha <- list(
    name = name,
    type = "Func_SharedAlpha_CS",
    generator = functionalInterPolyGenerator,
    paramStr = "nSub: 2, nCoeff: 2",
    param = list())
  
  functionalSharedAlpha$param[[1]] <- list(
    x = c(0., 10., 20.),
    y = c(0., 10., 0.),
    sd = 0.1,
    tMin = 0.,
    tMax = 20.,
    nTime = 100)
  
  functionalSharedAlpha$param[[2]] <- list(
    x = c(0., 10., 20.),
    y = c(10., 0., 10.),
    sd = 0.1,
    tMin = 0.,
    tMax = 20.,
    nTime = 100)
  
  return(functionalSharedAlpha)
}

# @author Vincent Kubicki
functionalSharedAlphaParam1sub <- function(name) {
  functionalSharedAlpha <- list()
  functionalSharedAlpha$param <- list()
  functionalSharedAlpha$param[[1]] <- list()
  functionalSharedAlpha$param[[2]] <- list()
  
  functionalSharedAlpha$name <- name
  functionalSharedAlpha$type <- "Func_SharedAlpha_CS"
  functionalSharedAlpha$generator <- functionalGenerator
  functionalSharedAlpha$paramStr <- "nSub: 1, nCoeff: 2"
  
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

  functionalSharedAlpha$param[[1]]$alpha <- alpha
  functionalSharedAlpha$param[[1]]$beta <- beta1
  functionalSharedAlpha$param[[1]]$sigma <- c(sigma, sigma)
  functionalSharedAlpha$param[[1]]$nTime <- 20
  functionalSharedAlpha$param[[1]]$tMin <- 0.
  functionalSharedAlpha$param[[1]]$tMax <- 50.
  
  functionalSharedAlpha$param[[2]]$alpha <- alpha
  functionalSharedAlpha$param[[2]]$beta <- beta2
  functionalSharedAlpha$param[[2]]$sigma <- c(sigma, sigma)
  functionalSharedAlpha$param[[2]]$nTime <- 20
  functionalSharedAlpha$param[[2]]$tMin <- 0.
  functionalSharedAlpha$param[[2]]$tMax <- 50.
  
  return(functionalSharedAlpha)
}

# @author Vincent Kubicki
functionalSharedAlphaParam2sub <- function(name) {
  functionalSharedAlpha <- list()
  functionalSharedAlpha$param <- list()
  functionalSharedAlpha$param[[1]] <- list()
  functionalSharedAlpha$param[[2]] <- list()
  
  functionalSharedAlpha$name <- name
  functionalSharedAlpha$type <- "Func_SharedAlpha_CS"
  functionalSharedAlpha$generator <- functionalGenerator
  functionalSharedAlpha$paramStr <- "nSub: 2, nCoeff: 2"
  
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

  functionalSharedAlpha$param[[1]]$alpha <- alpha
  functionalSharedAlpha$param[[1]]$beta <- beta1
  functionalSharedAlpha$param[[1]]$sigma <- c(sigma, sigma)
  functionalSharedAlpha$param[[1]]$nTime <- 20
  functionalSharedAlpha$param[[1]]$tMin <- 0.
  functionalSharedAlpha$param[[1]]$tMax <- 50.
  
  functionalSharedAlpha$param[[2]]$alpha <- alpha
  functionalSharedAlpha$param[[2]]$beta <- beta2
  functionalSharedAlpha$param[[2]]$sigma <- c(sigma, sigma)
  functionalSharedAlpha$param[[2]]$nTime <- 20
  functionalSharedAlpha$param[[2]]$tMin <- 0.
  functionalSharedAlpha$param[[2]]$tMax <- 50.
  
  return(functionalSharedAlpha)
}
