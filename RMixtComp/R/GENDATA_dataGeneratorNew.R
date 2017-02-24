dataGeneratorNew <- function(prefix,
                             nbInd,
                             proportionPresent,
                             var) {
  nbVar <- length(var) # number of variables including the latent class
  zDis <- rmultinom(nbInd,
                    1,
                    var$z_class$param)
  nIndClass <- numeric(length(var$z_class$param))
  
  z <- vector("integer",
              nbInd)
  for (i in 1:nbInd) {
    z[i] <- match(1, zDis[, i])
    nIndClass[z[i]] <- nIndClass[z[i]] + 1
  }
  
  cat("Number of observation per class: ", nIndClass, "\n")
  
  headerStr <- matrix(data = "",
                      nrow = 1,
                      ncol = nbVar)
  
  for (j in 1:nbVar) {
    headerStr[1, j] <- var[[j]]$name
  }
  
  dataStr <- matrix(data = "",
                    nrow = nbInd,
                    ncol = nbVar)
  
  nbPresentVar <- round(nbVar * proportionPresent)
  
  for (i in 1:nbInd) {
    presentVar <- sample(1:nbVar, nbPresentVar)
    
    if (!(1 %in% presentVar) || var$z_class$allMissing) {
      dataStr[i, 1] = "?"
    }
    else if ((1 %in% presentVar) || var$z_class$allPresent) {
      dataStr[i, 1] = paste(z[i])
    }

    for (j in 2:nbVar) { # export values for other types
      if (var[[j]]$type == "Rank") {
        dataStr[i, j] <- rankGenerator(j %in% presentVar,
                                       var[[j]]$param[[z[i]]])
      }
      if (var[[j]]$type == "Ordinal") {
        dataStr[i, j] <- ordinalGenerator(j %in% presentVar,
                                          var[[j]]$param[[z[i]]])
      }
      if (var[[j]]$type == "Functional") {
        dataStr[i, j] <- functionalGenerator(var[[j]]$param[[z[i]]])
      }
    }
  }
  
  dataMat <- rbind(headerStr, dataStr)
  
  descStr <- matrix(data = "",
                    nrow = 2,
                    ncol = nbVar)
  
  for (j in 1:nbVar) {
    descStr[1, j] <- var[[j]]$type
    if (exists("paramStr", where = var[[j]])) {
      descStr[2, j] <- var[[j]]$paramStr
    }
  }
  
  descMat <- rbind(headerStr, descStr)
  
  return(list(z = z,
              data = dataMat,
              descriptor = descMat))
}