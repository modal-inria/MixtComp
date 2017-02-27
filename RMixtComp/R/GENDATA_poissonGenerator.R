poissonParam <- function(name) {
  poisson <- list()
  poisson$name <- name
  poisson$type <- "Poisson_k"
  poisson$param <- list()
  poisson$param[[1]] <- 3
  poisson$param[[2]] <- 8
  
  return(poisson)
}


poissonGenerator <- function(present, param) {
  x <- poissonFullGenerator(param)

  xStr <- poissonHideData(present, x)

  return(xStr)
}


poissonFullGenerator <- function(param)
{
  rpois(1, lambda = param)
}


poissonHideData <- function(present, x)
{
  if(!present)
    x <- "?"
  
  return(x)
}