# MixtComp version 4.0  - july 2019
# Copyright (C) Inria - Université de Lille - CNRS

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# 
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>
 

# @author Vincent Kubicki
ordinalTestGenerator <- function() {
  ordinalTestParam <- list(nbMod = 5,
                           mu = 3,
                           pi = 0.8)
  ordinalTestPresent <- FALSE
  ordinalTestX <- ordinalFullGenerator(ordinalTestParam)
  
  ordinalTestStr <- ordinalHideData(ordinalTestPresent,
                                    ordinalTestParam,
                                    ordinalTestX)
  
  return(ordinalTestStr)
}

# @author Vincent Kubicki
ordinalGenerator <- function(present,
                             param) {
  x <- ordinalFullGenerator(param)
  
  xStr <- ordinalHideData(present,
                          param,
                          x)
  
  return(xStr)
}

# @author Vincent Kubicki
ordinalFullGenerator <- function(param) {
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
  
  return(seg[1])
}

# @author Vincent Kubicki
ySample <- function(seg) { # uniform sampling of y
  segVal <- seg[1]:seg[2]
  y <- segVal[sample(length(segVal), 1)]
  return(y) 
}

# @author Vincent Kubicki
zSample <- function(param) {
  return(sample(x = 2,
                size = 1,
                prob = c(1. - param$pi,
                         param$pi)))
}

# @author Vincent Kubicki
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

# @author Vincent Kubicki
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

# @author Vincent Kubicki
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

# @author Vincent Kubicki
ordinalHideData <- function(present,
                            param,
                            x) {
  if(present) {
    return(paste0(x))
  }
  else {
    isMissing <- sample(2, size = 1) # 1: missing, 2: missingInterval

    if (isMissing == 1) {
      return("?")
    }
    else {
      firstIndex <- sample(1:(param$nbMod - 1), size = 1)
      lastIndex <- firstIndex + sample(param$nbMod - firstIndex, size = 1)
      
      missingStr <- paste(firstIndex,
                          ":",
                          lastIndex,
                          sep = " ",
                          collapse = " ")
      missingStr <- paste("[",
                          missingStr,
                          "]",
                          sep = "")
      return(missingStr)
    }
  }
}