categoricalParam <- function(name) {
  categorical <- list()
  categorical$name <- name
  categorical$type <- "Categorical_pjk"
  categorical$param <- list()
  categorical$param[[1]] <- c(0.1, 0.2, 0.2, 0.5)
  categorical$param[[2]] <- c(0.5, 0.3, 0.1, 0.1)
  
  return(categorical)
}


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
        modalities <- sample(nbModalities, nbModalitiesToDraw)

        x <- paste0("{",paste0(modalities, collapse = ","),"}")
      }

    }
  }
  
  
  return(x)
}