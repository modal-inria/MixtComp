ordinalGenerator <- function(missing,
                             param) {
  nbMod <- param$nbMod
  seg <- c(1, nbMod) # initial segment is the input
  
  for (n in 1:(nbMod - 1)) {
    y <- ySample(seg)
    
    z <- zSample(param)
    
    seg <- segSample(y,
                     z,
                     seg,
                     param,
                     nbMod)
  }
  
  cat("last seg: ", seg, "\n", sep = "")
  
  return(paste0(seg[1]))
}

ySample <- function(seg) {
  return(sample(seg[1]:seg[2], 1)) # uniform sampling of y
}

zSample <- function(param) {
  return(sample(x = 2,
                size = 1,
                prob = c(1. - param$pi,
                         param$pi)))
}

segSample <- function(y,
                      z,
                      seg,
                      param,
                      nbMod) {

  currSeg <- allSeg(y,
                    seg,
                    param,
                    nbMod)
  
  e <- eSample(z,
               currSeg)
  
  seg[1] <- currSeg$firstIndSeg[e]
  seg[2] <- currSeg$firstIndSeg[e] + currSeg$lengthSeg[e] - 1
  
  return(seg)
}

allSeg <- function(y,
                   seg,
                   param,
                   nbMod) {
  firstIndSeg <- vector(mode = "integer", length = 3)
  lengthSeg   <- vector(mode = "integer", length = 3)
  distSeg     <- vector(mode = "integer", length = 3)
  
  firstIndSeg[1] <- seg[1]
  lengthSeg[1] <- y - seg[1]
  
  firstIndSeg[2] <- y
  lengthSeg[2] <- 1
  
  firstIndSeg[3] <- y + 1
  lengthSeg[3] <- seg[2] - y
  
  for (i in 1:3) {
    if (0 < lengthSeg[i]) { # this test enforces that the segment will not be considered the closest if its length is 0
      distSeg[i] <- min(abs(param$mu - firstIndSeg[i]),
                        abs(param$mu - (firstIndSeg[i] + lengthSeg[i] - 1)))
    }
    else {
      distSeg[i] <- nbMod 
    }
  }
  
  return(list(firstIndSeg = firstIndSeg,
              lengthSeg = lengthSeg,
              distSeg = distSeg))
}

eSample <- function(z,
                    currSeg) {
  e <- -1
  if (z == 1) { # innacurate comparison, sample using length as proportions
    proba <- currSeg$lengthSeg / sum(currSeg$lengthSeg)
    e <- sample(x = 3,
                size = 1,
                prob = proba)
  }
  else { # find which non null segment is the closest
    e <- which(currSeg$distSeg == min(currSeg$distSeg))[1]
  }
  
  return(e)
}