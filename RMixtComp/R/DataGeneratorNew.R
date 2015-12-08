dataGeneratorNewTest <- function() {
  proportionMissing <- 1.0
  nbInd <- 100
  
  var <- list()
  
  var$z_class <- zParam()
  var$Rank1 <- rankParam()
#  var$Ordinal1 <- ordinalParam()
  
  res <- dataGeneratorNew("dataGenNew/learn", # prefix
                          nbInd, # nbInd
                          proportionMissing,
                          var) # param
  
  write.table(res$z,
              file = paste("dataGenNew/learn/classIn.csv",
                           sep = "/"),
              row.names=FALSE,
              col.names=FALSE)
  
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

zParam <- function() {
  z_class <- list() # z_class must be the first variable in the list
  
  z_class$name <- "z_class"
  z_class$type <- "LatentClass"
  z_class$param <- c(0.3, 0.7) # parameters for z_class are the mixture proportions
  
  return(z_class)
}

rankParam <- function() {
  Rank <- list()
  Rank$param <- list()
  Rank$param[[1]] <- list()
  Rank$param[[2]] <- list()
  
  Rank$name <- "Rank1"
  Rank$type <- "Rank"
  Rank$param[[1]]$mu <- c(3, 4, 2, 1)
  Rank$param[[1]]$pi <- 0.8
  Rank$param[[2]]$mu <- c(1, 3, 2, 4)
  Rank$param[[2]]$pi <- 0.7
  
  return(Rank)
}

ordinalParam <- function() {
  Ordinal <- list()
  Ordinal$param <- list()
  Ordinal$param[[1]] <- list()
  Ordinal$param[[2]] <- list()
  
  Ordinal$name <- "Ordinal1"
  Ordinal$type <- "Ordinal"
  Ordinal$param[[1]]$nbMod <- 4
  Ordinal$param[[1]]$mu <- 1
  Ordinal$param[[1]]$pi <- 0.8
  Ordinal$param[[2]]$nbMod <- 4
  Ordinal$param[[2]]$mu <- 4
  Ordinal$param[[2]]$pi <- 0.7
  
  return(Ordinal)
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
        dataStr[i, j] <- rankGenerator(FALSE,
                                       var[[j]]$param[[z[i]]])
      }
      if (var[[j]]$type == "Ordinal") {
        dataStr[i, j] <- OrdinalGenerator(FALSE,
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

OrdinalGenerator <- function(missing,
                             param) {
  nbMod <- param$nbMod
  x <- vector(mode = "integer", length = nbMod)
  seg <- c(1, nbMod) # initial segment is the input
  
  for (n in 1:(nbMod - 1)) {
    y <- sample(seg[1]:seg[2], 1) # uniform sampling of y
    
    firstIndSeg <- vector(mode = "integer", length = 3)
    lengthSeg   <- vector(mode = "integer", length = 3)
    distSeg     <- vector(mode = "integer", length = 3)
    
    firstIndSeg[1] <- seg[1]
    lengthSeg[1] <- y - seg[1]
    
    firstIndSeg[2] <- y
    lengthSeg[2] <- 1
    
    firstIndSeg[3] <- y + 1
    lengthSeg[3] <- seg[2] - y
    
    for (i in 1:3) {
      if (lengthSeg[i] > 0) {
        distSeg[i] <- min(abs(param$mu - firstIndSeg[i]),
                          abs(param$mu - (firstIndSeg[i] + lengthSeg[i] - 1)))
      }
      else {
        distSeg[i] <- nbMod # this distance enforce that the segment will not be considered the closest if its length is 0
      }
    }
    
    z <- sample(x = 2,
                size = 1,
                prob = c(1. - param$pi,
                         param$pi))
    
    e <- -1 # selected segment
    if (z == 1) { # innacurate comparison, sample using length as proportions
      proba <- distSeg / sum(distSeg)
      e <- sample(x = 3,
                  size = 1,
                  prob = proba)
    }
    else { # find which non null segment is the closest
      e <- which(distSeg == min(distSeg))[1]
    }
    
    seg[1] <- firstIndSeg[e]
    seg[2] <- firstIndSeg[e] + lengthSeg[e] - 1
  }
  
  return(paste0(seg[1]))
}