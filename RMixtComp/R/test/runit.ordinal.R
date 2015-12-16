test.allSeg <- function() {
  nbPos <- 5
  allTrue <- TRUE
  
  for (i in 1:nbPos) {
    for (j in i:nbPos) {
      for (y in i:j) {
        for (z in 1:2) {
          for (m in 1:nbPos) {
            param <- list(mu = m)
            
            currSeg <- allSeg(y,
                              c(i, j),
                              param,
                              nbPos)
            
            e <- eSample(z,
                         currSeg)
            
            seg <- c(currSeg$firstIndSeg[e],
                     currSeg$firstIndSeg[e] + currSeg$lengthSeg[e] - 1)
            
            allTrue <- allTrue && (seg[1] <= seg[2])
          }
        }
      }
    }
  }
  
  checkEquals(allTrue, TRUE)
}

test.ordinalGeneratorSeg <- function() {
  nbTest <- 100
  nbMod <- 5
  
  allTrue <- TRUE
  
  for (i in 1:nbTest) {
    param <- list(mu = sample(nbMod, 1),
                  pi = runif(1),
                  nbMod = nbMod)
    
    seg <- ordinalGeneratorSeg(FALSE,
                               param)
    
    allTrue <- allTrue && (seg[1] == seg[2])
  }
  
  checkEquals(allTrue, TRUE)
}