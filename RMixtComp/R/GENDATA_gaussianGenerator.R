gaussianParam <- function(name) {
  gaussian <- list()
  gaussian$name <- name
  gaussian$type <- "Gaussian_sjk"
  gaussian$param <- list()
  gaussian$param[[1]] <- list(mean = 0, sd = 1)
  gaussian$param[[2]] <- list(mean = 2, sd = 1)
  
  return(gaussian)
}


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
  
  
  return(x)
}