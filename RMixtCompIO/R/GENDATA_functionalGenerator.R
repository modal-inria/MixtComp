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

  x <- vector("numeric", param$nTime)
  
  t <- sort(runif(param$nTime, param$tMin, param$tMax))

  for (i in 1:param$nTime) {
    logKappa <- param$alpha[, 1] + param$alpha[, 2] * t[i]
    
    kappa <- logToMulti(logKappa)
    indSub <- which(rmultinom(1, 1, kappa) == 1)
    
    xExp <- drop(param$beta[indSub, ] %*% t[i]^((1:nCoeff)-1))
    
    x[i] <- rnorm(1, xExp, param$sigma[indSub])
  }
  
  xStr <- paste(t, x, sep = ":", collapse = ",")
  
  return(xStr)
}

# @author Vincent Kubicki
logToMulti <- function(logIn) {
  max <- max(logIn)
  multiOut <- logIn - max
  multiOut <- exp(multiOut)
  sum <- sum(multiOut)
  multiOut <- multiOut / sum;
  
  return(multiOut)
}