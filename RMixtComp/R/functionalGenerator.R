functionalParam <- function(name) {
  alphaSlope <- 0.5
  alpha0 <- 25.
  
  Functional <- list()
  Functional$param <- list()
  Functional$param[[1]] <- list()
  Functional$param[[2]] <- list()
  
  Functional$name <- name
  Functional$type <- "Functional"
  Functional$paramStr <- "nSub: 2, nCoeff: 2"
  
  Functional$param[[1]]$alpha <- matrix(c( alpha0 * alphaSlope, -alphaSlope,
                                          -alpha0 * alphaSlope,  alphaSlope), 2, 2, byrow = TRUE)
  Functional$param[[1]]$beta <- matrix(c(0., -0.5,
                                         0., 0.5),
                                       2, 2, byrow = TRUE)
  Functional$param[[1]]$sigma <- c(1., 1.)
  Functional$param[[1]]$nTime <- 20
  Functional$param[[1]]$tMin <- 0.
  Functional$param[[1]]$tMax <- 50.
  
  Functional$param[[2]]$alpha <- matrix(c(-alpha0 * alphaSlope,  alphaSlope,
                                           alpha0 * alphaSlope, -alphaSlope), 2, 2, byrow = TRUE)
  Functional$param[[2]]$beta <- matrix(c(0., -0.9,
                                         0., 0.9),
                                       2, 2, byrow = TRUE)
  Functional$param[[2]]$sigma <- c(1., 1.)
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