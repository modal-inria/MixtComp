dataGeneratorNewTest <- function() {
  proportionMissing <- 0.
  nbInd <- 100
  
  var <- list()
  
  var$z_class <- zParam()
  var$Rank1 <- rankParam()
  # var$Ordinal1 <- ordinalParam()
  
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
  z_class$allPresent <- FALSE
  z_class$allMissing <- TRUE
  
  return(z_class)
}

rankParam <- function() {
  Rank <- list()
  Rank$param <- list()
  Rank$param[[1]] <- list()
  Rank$param[[2]] <- list()
  
  Rank$name <- "Rank1"
  Rank$type <- "Rank"
  Rank$param[[1]]$mu <- c(1, 2, 3, 4)
  Rank$param[[1]]$pi <- 0.8
  Rank$param[[2]]$mu <- c(4, 3, 2, 1)
  Rank$param[[2]]$pi <- 0.8
  
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
  Ordinal$param[[2]]$pi <- 0.8
  
  return(Ordinal)
}