dataGeneratorNew <- function(prefix,
                             nbInd,
                             proportionMissing,
                             var) {
  nbVar <- length(var) # number of variables including the latent class
  zDis <- rmultinom(nbInd,
                    1,
                    var$z_class$param)
  z <- vector("integer",
              nbInd)
  for (i in 1:nbInd) {
    z[i] <- match(1, zDis[, i])
  }
  
  headerStr <- matrix(data = "",
                      nrow = 1,
                      ncol = nbVar)
  
  for (j in 1:nbVar) {
    headerStr[1, j] <- var[[j]]$name
  }
  
  dataStr <- matrix(data = "",
                    nrow = nbInd,
                    ncol = nbVar)
  
  for (i in 1:nbInd) {
    presentVar <- sample(2,
                         size = nbVar,
                         replace = TRUE,
                         prob = c(proportionMissing,
                                  1. - proportionMissing)) # list of variables containing observed values for the current individual
    
    if (presentVar[1] == 1 || var$z_class$allMissing) {
      dataStr[i, 1] = "?"
    }
    else if (presentVar[1] == 2 || var$z_class$allPresent) {
      dataStr[i, 1] = paste(z[i])
    }

    for (j in 2:nbVar) { # export values for other types
      if (var[[j]]$type == "Rank") {
        dataStr[i, j] <- rankGenerator(FALSE,
                                       var[[j]]$param[[z[i]]])
      }
      if (var[[j]]$type == "Ordinal") {
        dataStr[i, j] <- ordinalGenerator(FALSE,
                                          var[[j]]$param[[z[i]]])
      }
    }
  }
  
  dataMat <- rbind(headerStr, dataStr)
  
  descStr <- matrix(data = "",
                    nrow = 1,
                    ncol = nbVar)
  
  for (j in 1:nbVar) {
    descStr[1, j] <- var[[j]]$type
  }
  
  descMat <- rbind(headerStr, descStr)
  
  return(list(z = z,
              data = dataMat,
              descriptor = descMat))
}