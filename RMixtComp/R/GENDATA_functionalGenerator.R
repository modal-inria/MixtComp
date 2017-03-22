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