gaussianGenerator <- function(present, param) {
  x <- gaussianFullGenerator(param)
  
  xStr <- gaussianHideData(present, x, param)
  
  return(xStr)
}


gaussianFullGenerator <- function(param)
{
  rnorm(1, param$mean, param$sd)
}


gaussianHideData <- function(present, x, param)
{
  if(!present)
  {
    missType <- sample(4, size = 1)
    
    bounds <- round(sort(rnorm(2, param$mean, param$sd)), 5)
    
    
    x <- switch(missType,
                "1" = "?",
                "2" = paste0("[",bounds[1],":",bounds[2],"]"),
                "3" = paste0("[","-inf",":",bounds[2],"]"),
                "4" = paste0("[",bounds[1],":","+inf","]"))
    
  }
  
  return(as.character(x))
}