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
 

# gaussian ----------------------------------------------------------------

# @author Vincent Kubicki
gaussianParam <- function(name) {
  gaussian <- list()
  gaussian$name <- name
  gaussian$type <- "Gaussian"
  gaussian$paramStr <- ""
  gaussian$generator <- gaussianGenerator
  gaussian$param <- list()
  gaussian$param[[1]] <- list(mean = -5, sd = 1)
  gaussian$param[[2]] <- list(mean = 5, sd = 1)
  
  return(gaussian)
}


# poisson -----------------------------------------------------------------

# @author Vincent Kubicki
poissonParam <- function(name) {
  poisson <- list()
  poisson$name <- name
  poisson$type <- "Poisson"
  poisson$paramStr <- ""
  poisson$generator <- poissonGenerator
  poisson$param <- list()
  poisson$param[[1]] <- 1
  poisson$param[[2]] <- 10
  
  return(poisson)
}

# @author Vincent Kubicki
poissonParamRandom <- function(name) {
  valMin <- 0.1
  valMax <- 15.0
  
  poisson <- list()
  poisson$name <- name
  poisson$type <- "Poisson"
  poisson$paramStr <- ""
  poisson$generator <- poissonGenerator
  poisson$param <- list()
  poisson$param[[1]] <- runif(n = 1, min = valMin, max = valMax)
  poisson$param[[2]] <- runif(n = 1, min = valMin, max = valMax)
  
  return(poisson)
}


# categorical -------------------------------------------------------------

# @author Vincent Kubicki
categoricalParam1 <- function(name) {
  categorical <- list()
  categorical$name <- name
  categorical$type <- "Multinomial"
  categorical$paramStr <- ""
  categorical$generator <- categoricalGenerator 
  categorical$param <- list()
  categorical$param[[1]] <- c(0.1, 0.2, 0.2, 0.5)
  categorical$param[[2]] <- c(0.5, 0.3, 0.1, 0.1)
  
  return(categorical)
}

# @author Vincent Kubicki
categoricalParam2 <- function(name) {
  categorical <- list()
  categorical$name <- name
  categorical$type <- "Multinomial"
  categorical$paramStr <- ""
  categorical$generator <- categoricalGenerator 
  categorical$param <- list()
  categorical$param[[1]] <- c(0.6, 0.1, 0.1, 0.2)
  categorical$param[[2]] <- c(0.1, 0.05, 0.8, 0.05)
  
  return(categorical)
}

# @author Vincent Kubicki
categoricalParamRandom <- function(name) {
  nbModalities <- 4
  param1 <- runif(4)
  param1 <- param1 / sum(param1)
  param2 <- runif(4)
  param2 <- param2 / sum(param2)
  
  categorical <- list()
  categorical$name <- name
  categorical$type <- "Multinomial"
  categorical$paramStr <- ""
  categorical$generator <- categoricalGenerator 
  categorical$param <- list()
  categorical$param[[1]] <- param1
  categorical$param[[2]] <- param2
  
  return(categorical)
}


# Weibull -----------------------------------------------------------------

# @author Vincent Kubicki
weibullParam <- function(name) {
  weibull <- list()
  weibull$name <- name
  weibull$type <- "Weibull"
  weibull$paramStr <- ""
  weibull$generator <- weibullGenerator
  weibull$param <- list()
  weibull$param[[1]] <- list(shape = 2.0, scale = 1.0)
  weibull$param[[2]] <- list(shape = 4.0, scale = 3.0)
  
  return(weibull)
}

# Negative Binomial -------------------------------------------------------

# @author Quentin Grimonprez
nBinomParam <- function(name) {
  nBinom <- list()
  nBinom$name <- name
  nBinom$type <- "NegativeBinomial"
  nBinom$paramStr <- ""
  nBinom$generator <- negativeBinomialGenerator
  nBinom$param <- list()
  nBinom$param[[1]] <- list(n = 3.0, p = 0.7)
  nBinom$param[[2]] <- list(n = 20.0, p = 0.8)
  
  return(nBinom)
}

# @author Quentin Grimonprez
nBinomParamRandom <- function(name) {
  nBinom <- list()
  nBinom$name <- name
  nBinom$type <- "NegativeBinomial"
  nBinom$paramStr <- ""
  nBinom$generator <- negativeBinomialGenerator
  nBinom$param <- list()
  nBinom$param[[1]] <- list(n = sum(rbinom(20, 1, 0.5)), p = runif(1, 0.3, 0.8))
  nBinom$param[[2]] <- list(n = sum(rbinom(20, 1, 0.5)), p = runif(1, 0.3, 0.8))
  
  return(nBinom)
}


# rank --------------------------------------------------------------------

# @author Vincent Kubicki
rankParam <- function(name) {
  Rank <- list()
  Rank$param <- list()
  Rank$param[[1]] <- list()
  Rank$param[[2]] <- list()
  
  Rank$name <- name
  Rank$type <- "Rank_ISR"
  Rank$paramStr <- ""
  Rank$generator <- rankGenerator
  Rank$param[[1]]$mu <- c(1, 2, 3, 4)
  Rank$param[[1]]$pi <- 0.8
  Rank$param[[2]]$mu <- c(4, 3, 2, 1)
  Rank$param[[2]]$pi <- 0.8
  
  return(Rank)
}
