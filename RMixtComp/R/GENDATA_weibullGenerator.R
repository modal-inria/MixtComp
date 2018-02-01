weibullGenerator <- function(present, param) {
  x <- weibullFullGenerator(param)
  xStr <- weibullHideData(present, x, param)
  return(xStr)
}

weibullFullGenerator <- function(param) {
  rweibull(1, param$shape, param$scale)
}

weibullHideData <- function(present, x, param) {
  if(!present) {
    missType <- sample(2, size = 1)
    
    bounds <- round(sort(rweibull(2, param$shape, param$scale)), 5)
    
    x <- switch(
      missType,
      "1" = "?",
      "2" = paste0("[",bounds[1],":","+inf","]"))
  }
  
  return(as.character(x))
}