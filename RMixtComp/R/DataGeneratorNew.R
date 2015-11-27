dataGeneratorNewTest <- function() {
  proportionMissing <- 0.2
  
  var <- list()
  var$z_class <- list() # z_class must be the first variable in the list
  var$Rank1 <- list()
  var$Rank1$param <- list()
  var$Rank1$param[[1]] <- list()
  var$Rank1$param[[2]] <- list()
  
  var$z_class$name <- "z_class"
  var$z_class$type <- "LatentClass"
  var$z_class$param <- c(0.3, 0.7) # parameters for z_class are the mixture proportions
  
  var$Rank1$name <- "Rank1"
  var$Rank1$type <- "Rank"
  var$Rank1$param[[1]]$mu <- c(3, 4, 2, 1)
  var$Rank1$param[[1]]$pi <- 0.8
  var$Rank1$param[[2]]$mu <- c(1, 3, 2, 4)
  var$Rank1$param[[2]]$pi <- 0.7
  
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
    
    if (presentVar[1] == 1) { # z is missing
      dataStr[i, 1] = "?"
    }
    else { # z is observed
      dataStr[i, 1] = paste(z[i])
    }
    
    for (j in 2:nbVar) { # export values for other types
      if (var[[j]]$type == "Rank") {
        dataStr[i, j] <- rankGenerator(var[[j]]$param[[z[i]]])
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

rankGenerator <- function(missing,
                          param) {
  nbPos <- length(param$mu)
  muRanking <-  switchRepresentation(param$mu) # mu in ranking representation instead of ordering representation
  y <- sample(nbPos)
  x <- list(y[1])
  
  for (p in 2:nbPos) { # current element in the presentation order, or current size of the x vector + 1
    currY <- y[p]
    yPlaced <- FALSE # has currY been placed correctly ?
    
    for (i in 1:(p - 1)) {
      comparison <- muRanking[currY] < muRanking[x[[i]]]
      
      if (param$pi < runif(1)) { # is the comparison incorrect ?
        comparison <- !comparison
      }
      
      if (comparison) { # currY must be placed before the current p element (and become the p-th element)
        x <- append(x,
                    currY,
                    i - 1)
        yPlaced = TRUE
        break # no need to test further position for j element
      }
    }
    
    if (!yPlaced) { # if element j has not been placed yet, it goes at the end of x
      x <- append(x,
                  currY,
                  p - 1)
    }
  }
  
  xStr <- paste(x,
                sep = "",
                collapse = ", ")
  
  return(xStr)
}

switchRepresentation <- function(inRank) {
  nbMod <- length(inRank)
  outRank <- vector(mode = "integer", length = nbMod)
  
  for (p in 1:nbMod) {
    outRank[inRank[p]] = p;
  }
  
  return(outRank)
}