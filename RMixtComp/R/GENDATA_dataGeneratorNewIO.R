# Generate a test case.
# 
# The var parameter must contain a list of sublists. There must be one sublist for each variable, and each must contains the following fields:
# - name
# - param, one parameter set per class
# - generator, a function that takes parameters
# - type
# - paramStr
# 
# The z_class variable is mandatory, so as to sample the latent class. It must be present as the first variable in the var parameter.
# 
# @param nbInd number of individuals
# @param proportionPresent proportion of present data, the rest will be sampled using the various types
# @param var list one element per variable
# @author Vincent Kubicki
dataGeneratorNewIO <- function(
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

  dataStr <- vector("list", nbVar)
  for (j in 1:nbVar) {
    dataStr[[j]] <- vector("character", nbVar)
    names(dataStr)[j] <- var[[j]]$name
  }
  
  for (i in 1:nbInd) {
    presentVar <- which(rbinom(nbVar, 1, proportionPresent) == 1) # indices of missing variables for current observation
    if(length(presentVar) == 0) { # no observed variable for the individual
      presentVar = sample(nbVar, 1) # one variable is forced to be observed
    }
    
    if (!(1 %in% presentVar) || var$z_class$allMissing) { # %in% is used to test if 1 is present in presentVar. The first variable must be z_class.
      dataStr[[1]][i] = "?"
    }
    else if ((1 %in% presentVar) || var$z_class$allPresent) {
      dataStr[[1]][i] = paste(z[i])
    }
    
    for (j in 2:nbVar) { # export values for all other, non z_class, variables
      dataStr[[j]][i] <- var[[j]]$generator(j %in% presentVar, var[[j]]$param[[z[i]]])
    }
  }
  
  descStr <- list()
  for (j in 1:nbVar) {
    descStr[[var[[j]]$name]] <- list(type = var[[j]]$type, paramStr = var[[j]]$paramStr)
  }

  return(list(
    z = z,
    data = dataStr,
    descriptor = descStr))
}