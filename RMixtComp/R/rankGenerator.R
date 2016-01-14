rankTestGenerator <- function() {
  rankTestParam <- list(mu = c(1, 4, 2, 3),
                        pi = 0.8)
  rankTestMissing <- FALSE
  rankTestX <- rankFullGenerator(rankTestParam)
  
  rankTestStr <- rankHideData(rankTestMissing,
                              rankTestX)
  
  return(rankTestStr)
}

rankGenerator <- function(present,
                          param) {

  x <- rankFullGenerator(param)
  
  xStr <- rankHideData(present,
                       x)
  
  return(xStr)
}

rankFullGenerator <- function(param) {
  nbPos <- length(param$mu)
  muRanking <-  switchRepresentation(param$mu) # mu in ranking representation instead of ordering representation
  y <- sample(nbPos)
  x <- list(y[1])
  
  for (p in 2:nbPos) { # current element in the presentation order, or current size of the x vector + 1
    currY <- y[p]
    yPlaced <- FALSE # has currY been placed correctly ?
    
    for (i in 1:(p - 1)) {
      comparison <- muRanking[currY] < muRanking[x[[i]]]
      
      if (param$pi < runif(1)) { # is the comparison incorrect ?
        comparison <- !comparison
      }
      
      if (comparison) { # currY must be placed before the current p element (and become the p-th element)
        x <- append(x,
                    currY,
                    i - 1)
        yPlaced = TRUE
        break # no need to test further position for j element
      }
    }
    
    if (!yPlaced) { # if element j has not been placed yet, it goes at the end of x
      x <- append(x,
                  currY,
                  p - 1)
    }
  }
  
  return(x)
}

switchRepresentation <- function(inRank) {
  nbMod <- length(inRank)
  outRank <- vector(mode = "integer", length = nbMod)
  
  for (p in 1:nbMod) {
    outRank[inRank[p]] = p;
  }
  
  return(outRank)
}

rankHideData <- function(present,
                         x) {
  nbPos <- length(x)
  
  xStr <- vector(mode = 'character', length = nbPos)
  
  for (p in 1:nbPos) {
      xStr[p] <- x[p]
  }
  
  if(!present) {
    isMissing <- sample(3, size = 1) # 1: missing, 2: missingFiniteValues, 3: both
    
    if(isMissing %in% c(2, 3)) {
      firstIndex <- sample(1:(nbPos - 1), size = 1)
      lastIndex <- firstIndex + sample(nbPos - firstIndex, size = 1)
      
      missingStr <- paste(x[firstIndex:lastIndex],
                          sep = " ",
                          collapse = " ")
      missingStr <- paste("{",
                          missingStr,
                          "}",
                          sep = "")
      
      for (p in 1:nbPos) {
        if (p %in% firstIndex:lastIndex) {
          xStr[p] <- missingStr
        }
      }
    }
    
    if(isMissing %in% c(1, 3)) {
      indexMissing <- sample(nbPos, size = 1)
      xStr[indexMissing] <- "?"
    }
  }
  
  xStr <- paste(xStr,
                sep = "",
                collapse = ", ")
  
  return(xStr)
}