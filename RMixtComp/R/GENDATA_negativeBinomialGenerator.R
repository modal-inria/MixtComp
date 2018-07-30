negativeBinomialGenerator <- function(present, param) {
  x <- negativeBinomialFullGenerator(param)
  
  xStr <- negativeBinomialHideData(present, x)
  
  return(xStr)
}


negativeBinomialFullGenerator <- function(param)
{
  rnbinom(1, size = param$n, prob = param$p)
}


negativeBinomialHideData <- function(present, x)
{
  if(!present)
    x <- "?"
  
  return(as.character(x))
}