
# gaussian ----------------------------------------------------------------

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


# poisson -----------------------------------------------------------------

poissonGenerator <- function(present, param) {
  x <- poissonFullGenerator(param)
  
  xStr <- poissonHideData(present, x, param)
  
  return(xStr)
}


poissonFullGenerator <- function(param)
{
  rpois(1, lambda = param)
}


poissonHideData <- function(present, x, param)
{
  if(!present)
  {
    missType <- sample(3, size = 1)
    
    bounds <- round(sort(rpois(2, param)), 5)
    if(bounds[1] == bounds[2])
      bounds[2] = bounds[2] + 1
    
    
    x <- switch(missType,
                "1" = "?",
                "2" = paste0("[",bounds[1],":",bounds[2],"]"),
                "3" = paste0("[",bounds[1],":","+inf","]"))
    
  }
  
  return(as.character(x))
}



# negative binomial -------------------------------------------------------

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


# weibull -----------------------------------------------------------------

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
    missType <- sample(4, size = 1)
    
    bounds <- round(sort(rweibull(2, param$shape, param$scale)), 5)
    
    x <- switch(
      missType,
      "1" = "?",
      "2" = paste0("[",bounds[1],":","+inf","]"),
      "3" = paste0("[","-inf",":",bounds[2],"]"),
      "4" = paste0("[",bounds[1],":",bounds[2],"]"))
  }
  
  return(as.character(x))
}



# multinomial -------------------------------------------------------------

categoricalGenerator <- function(present, param) {
  x <- categoricalFullGenerator(param)
  
  xStr <- categoricalHideData(present, x, param)
  
  return(xStr)
}


categoricalFullGenerator <- function(param)
{
  which(rmultinom(1, 1, param) == 1)
}


categoricalHideData <- function(present, x, param)
{
  if(!present)
  {
    missType <- sample(2, size = 1) # 1: missing, 2: missingInterval
    
    if(missType == 1)
    {
      x = "?"
    }else{
      nbModalities <- length(param)
      
      if(nbModalities <= 2)
      {
        x = "?"
      }else{
        nbModalitiesToDraw <- max(sample(2:(nbModalities-1), 1), 2)
        modalities <- sort(sample(nbModalities, nbModalitiesToDraw))
        
        x <- paste0("{",paste0(modalities, collapse = ","),"}")
      }
      
    }
  }
  
  
  return(as.character(x))
}
