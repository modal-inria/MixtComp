rankGenerator <- function(missing,
                          param) {
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
  
  xStr <- paste(x,
                sep = "",
                collapse = ", ")
  
  return(xStr)
}

switchRepresentation <- function(inRank) {
  nbMod <- length(inRank)
  outRank <- vector(mode = "integer", length = nbMod)
  
  for (p in 1:nbMod) {
    outRank[inRank[p]] = p;
  }
  
  return(outRank)
}