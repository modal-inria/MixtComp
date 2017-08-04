#' Generate a functional observation using an interpolating polynomial instead of the generating model.
#' This allows for a little more flexibility and convenience in specifying data than functionalGenerator
#' which is currently bugged anyway.
#'
#' @param param A structure describing the points by which the interpolating polynomial is supposed to go through.
#' @return The generated observation as a string.
#' @examples
#' param <- list(
#'   x = c(0., 10., 20.),
#'   y = c(0., 10., 0.),
#'   sd = 0.1,
#'   tMin = 0.,
#'   tMax = 20.,
#'   nTime = 100)
#' functionalInterPolyGenerator(param)

functionalInterPolyGenerator <- function(present, param) {
  timeObs <- vector("character", param$nTime)
  
  nCoeff <- length(param$x)
  v <- vandermonde(param$x, nCoeff)
  a <- solve(v, param$y)
  x <- vector(mode = "numeric", length = param$nTime)
  
  for (i in 1:param$nTime) {
    x[i] <- runif(1, param$tMin, param$tMax)
  }
  x <- sort(x)
  
  for (i in 1:param$nTime) {
    t <- evalFunc(a, x[i]) + rnorm(1, mean = 0, sd = param$sd)
    timeObs[i] <- paste(x, t, sep = ":")
  }
  
  xStr <- paste(timeObs, collapse = ",")
  
  return(xStr)
}

vandermonde <- function(vec, nCoeff) {
  v <- matrix(nrow = nCoeff, ncol = nCoeff)
  for (i in 1:nCoeff) {
    for (j in 1:nCoeff) {
      v[i, j] <- vec[i] ** (j - 1)
    }
  }
  
  return(v)
}

evalFunc <- function(a, x) {
  nObs <- length(x)
  nCoeff <- length(a)
  y <- vector(mode = "numeric", length = length(x))
  
  for (i in 1:nObs) {
    y[i] <- 0.
    for (k in 1:nCoeff) {
      y[i] <- y[i] + a[k] * (x[i] ** (k - 1))
    }
  }
  
  return(y)
}