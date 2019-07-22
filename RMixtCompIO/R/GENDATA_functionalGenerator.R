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
functionalGenerator <- function(present, param) {
  nSub <- nrow(param$alpha)
  nCoeff <- ncol(param$beta)

  xStr <- ""
  t <- vector("numeric", param$nTime)
  x <- vector("numeric", param$nTime)
  
  for (i in 1:param$nTime) {
    t[i] <- runif(1, param$tMin, param$tMax)
  }
  t <- sort(t)

  for (i in 1:param$nTime) {
    logKappa <- vector("numeric", nSub)
    
    for (s in 1:nSub) {
      logKappa[s] <- param$alpha[s, 1] + param$alpha[s, 2] * t[i]
    }
    
    kappa <- logToMulti(logKappa)
    w <- match(1, rmultinom(1, 1, kappa))
    xExp <- 0.
    
    for (c in 1:nCoeff) {
      xExp <- xExp + param$beta[w, c] * t[i] ** (c - 1)
    }
    
    x[i] <- rnorm(1, xExp, param$sigma[w])
    xStr <- paste(xStr, t[i], ":", x[i], sep = "")
    
    if (i < param$nTime) {
      xStr <- paste(xStr, ",", sep="")
    }
  }
  
  return(xStr)
}

# @author Vincent Kubicki
logToMulti <- function(logIn) {
  max <- max(logIn);
  multiOut <- logIn - max
  multiOut <- exp(multiOut)
  sum <- sum(multiOut)
  multiOut <- multiOut / sum;
  
  return(multiOut)
}