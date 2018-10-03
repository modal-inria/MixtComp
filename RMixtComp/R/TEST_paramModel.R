
# gaussian ----------------------------------------------------------------

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

weibullParam <- function(name) {
  weibull <- list()
  weibull$name <- name
  weibull$type <- "Weibull"
  weibull$paramStr <- ""
  weibull$generator <- weibullGenerator
  weibull$param <- list()
  weibull$param[[1]] <- list(shape = 2.0, scale = 0.5)
  weibull$param[[2]] <- list(shape = 4.0, scale = 3.0)
  
  return(weibull)
}

# Negative Binomial -------------------------------------------------------

nBinomParam <- function(name) {
  nBinom <- list()
  nBinom$name <- name
  nBinom$type <- "NegativeBinomial"
  gaussian$paramStr <- ""
  nBinom$generator <- negativeBinomialGenerator
  nBinom$param <- list()
  nBinom$param[[1]] <- list(n = 2.0, p = 0.5)
  nBinom$param[[2]] <- list(n = 10.0, p = 0.8)
  
  return(nBinom)
}

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
