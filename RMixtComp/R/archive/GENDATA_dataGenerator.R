#' Generate a test case.
#' 
#' The var parameter must contain a list of sublists. There must be one sublist for each variable, and each must contains the following fields:
#' - name
#' - param, one parameter set per class
#' - generator, a function that takes parameters
#' - type
#' - paramStr
#' 
#' The z_class variable is mandatory, so as to sample the latent class. It must be present as the first variable in the var parameter.
#' 
#' @param nbInd number of individuals
#' @param proportionPresent proportion of present data, the rest will be sampled using the various types
#' @param var list one element per variable
dataGenerator <- function(
  nbInd,
  proportionPresent,
  var)
{
  nbVar <- length(var) # number of variables including the latent class
  nIndClass <- numeric(length(var$z_class$param))
  
  # create the partition
  zDis <- rmultinom(nbInd, 1, var$z_class$param) # result as a complete disjunctive table
  z <- vector("integer", nbInd)
  
  for (i in 1:nbInd) {
    z[i] <- match(1, zDis[, i])
    nIndClass[z[i]] <- nIndClass[z[i]] + 1
  }
  
  cat("Number of observation per class: ", nIndClass, "\n")
  
  headerStr <- matrix(data = "", nrow = 1, ncol = nbVar) # header is shared in data and desc
  for (j in 1:nbVar) {
    headerStr[1, j] <- var[[j]]$name
  }

  dataStr <- matrix(data = "", nrow = nbInd,  ncol = nbVar) # complete data table, every value described as a character string
  
  for (i in 1:nbInd) {
    presentVar <- which(rbinom(nbVar, 1, proportionPresent) == 1) # indices of missing variables for current observation
    if(length(presentVar) == 0) { # no observed variable for the individual
      presentVar = sample(nbVar, 1) # one variable is forced to be observed
    }
    
    if (!(1 %in% presentVar) || var$z_class$allMissing) { # %in% is used to test if 1 is present in presentVar. The first variable must be z_class.
      dataStr[i, 1] = "?"
    }
    else if ((1 %in% presentVar) || var$z_class$allPresent) {
      dataStr[i, 1] = paste(z[i])
    }
    
    for (j in 2:nbVar) { # export values for all other, non z_class, variables
      dataStr[i, j] <- var[[j]]$generator(j %in% presentVar, var[[j]]$param[[z[i]]])
    }
  }
  
  colnames(dataStr) = headerStr 

  descStr <- matrix(
    data = "",
    nrow = 2,
    ncol = nbVar)
  
  for (j in 1:nbVar) {
    descStr[1, j] <- var[[j]]$type
    if (exists("paramStr", where = var[[j]])) {
      descStr[2, j] <- var[[j]]$paramStr
    }
  }
  
  colnames(descStr) = headerStr 

  return(list(
    z = z,
    data = dataStr,
    descriptor = descStr))
}