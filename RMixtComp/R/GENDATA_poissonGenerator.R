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
  
  return(as.character(x))
}