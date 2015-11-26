dataGeneratorNewTest <- function() {
  proportionMissing <- 0.2
  
  var <- list()
  var$z_class <- list() # z_class must be the first variable in the list
  var$Rank1 <- list()
  
  var$z_class$name <- "z_class"
  var$z_class$type <- "LatentClass"
  var$z_class$param <- c(0.3, 0.7) # parameters for z_class are the mixture proportions
  
  var$Rank1$name <- "Rank1"
  var$Rank1$type <- "Rank"
  var$Rank1$param <- c(0.3, 0.7) # parameters for z_class are the mixture proportions
  
  res <- dataGeneratorNew("dataGenNew/learn", # prefix
                          500, # nbSample
                          proportionMissing,
                          var) # param
  
  write.table(res$data,
              file = "dataGenNew/learn/data.csv",
              append = FALSE,
              quote = FALSE,
              sep = ";",
              eol = "\n",
              na = "NA",
              dec = ".",
              row.names = FALSE,
              col.names = FALSE,
              qmethod = c("escape",
                          "double"),
              fileEncoding = "")
  
  write.table(res$desc,
              file = "dataGenNew/learn/desc.csv",
              append = FALSE,
              quote = FALSE,
              sep = ";",
              eol = "\n",
              na = "NA",
              dec = ".",
              row.names = FALSE,
              col.names = FALSE,
              qmethod = c("escape",
                          "double"),
              fileEncoding = "")
}

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
  for (i in 1:nbInd)
  {
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
    
    if (presentVar[1] == 1) { # z is missing
      dataStr[i, 1] = "?"
    }
    else{ # z is observed
      dataStr[i, 1] = paste(z[i])
    }
    
    for (j in 2:nbVar) { # export values for other types
      if (var[[j]]$type == "Rank") {
        dataStr[i, j] <- rankGenerator()
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
  
  return(list(data = dataMat,
              descriptor = descMat))
}

rankGenerator <- function() {
  return("generator to be implemented")
}