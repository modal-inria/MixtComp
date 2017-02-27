plotFunctional <- function(i) {
  df <- read.csv(file = "dataGenNew/learn/data.csv",
                 header = TRUE,
                 sep = ";",
                 as.is = TRUE)
  lsSplit <- strsplit(df$Functional1[[i]], ",")
  
  mySplit <- function(x) {
    return(strsplit(x, ":")[[1]])
  }
  
  dataSplit <- lapply(lsSplit[[1]], mySplit)
  
  nPoints <- length(dataSplit)
  x <- vector("numeric", nPoints)
  y <- vector("numeric", nPoints)
  
  for (i in 1:nPoints) {
    x[i] <- as.numeric(dataSplit[[i]][1])
    y[i] <- as.numeric(dataSplit[[i]][2])
  }
  
  plot(x, y)
}

getLinEq <- function(pointA, pointB) {
  a <- matrix(c(1., pointA[1],
                1., pointB[1]),
              2, 2, byrow = TRUE)
  b <- c(pointA[2], pointB[2])
  x <- solve(a, b)
  
  return(x)
}

functionalParam1sub <- function(name) {
  Functional <- list()
  Functional$param <- list()
  Functional$param[[1]] <- list()
  Functional$param[[2]] <- list()
  
  Functional$name <- name
  Functional$type <- "Functional"
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

functionalGenerator <- function(param) {
  nSub <- nrow(param$alpha)
  nCoeff <- ncol(param$beta)

  xStr <- ""
  t <- vector("numeric", param$nTime)
  x <- vector("numeric", param$nTime)
  
  for (i in 1:param$nTime) {
    t[i] <- runif(1, param$tMin, param$tMax)
    logKappa <- vector("numeric", nSub)
    
    for (s in 1:nSub) {
      logKappa[s] <- param$alpha[s, 1] + param$alpha[s, 2] * t[i]
    }
    
    kappa <- logToMulti(logKappa)
    w <- match(1, rmultinom(1, 1, kappa))
    xExp <- 0.
    
    for (c in 1:nCoeff) {
      xExp <- xExp + param$beta[w, c] * t[i] ** (c - 1)
    }
    
    x[i] <- rnorm(1, xExp, param$sigma[w])
    xStr <- paste(xStr, t[i], ":", x[i], sep = "")
    
    if (i < param$nTime) {
      xStr <- paste(xStr, ",", sep="")
    }
  }
  
  return(xStr)
}

logToMulti <- function(logIn) {
  max <- max(logIn);
  multiOut <- logIn - max
  multiOut <- exp(multiOut)
  sum <- sum(multiOut)
  multiOut <- multiOut / sum;
  
  return(multiOut)
}