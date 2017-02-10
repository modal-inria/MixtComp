dataGeneratorNewLearn <- function(nbInd,
                                  proportionPresent) {
  var <- list()
  var$z_class <- zParam()
  # var$z_class <- zParamFixedInit()
  # var$Rank1 <- rankParam("Rank1")
  var$Ordinal1 <- ordinalParam("Ordinal1")
  # var$Functional1 <- functionalParam1sub("Functional1")
  # var$Functional1 <- functionalParam2sub("Functional1")
  
  res <- dataGeneratorNew("dataGenNew/learn", # prefix
                          nbInd, # nbInd
                          proportionPresent,
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

dataGeneratorNewPredict <- function(nbInd,
                                    proportionPresent) {
  var <- list()
  var$z_class <- zParam()
  # var$z_class <- zParamFixedInit()
  # var$Rank1 <- rankParam("Rank1")
  # var$Ordinal1 <- ordinalParam("Ordinal1")
  # var$Functional1 <- functionalParam1sub("Functional1")
  var$Functional1 <- functionalParam2sub("Functional1")
  
  res <- dataGeneratorNew("dataGenNew/predict", # prefix
                          nbInd, # nbInd
                          proportionPresent,
                          var) # param
  
  write.table(res$z,
              file = paste("dataGenNew/predict/classIn.csv",
                           sep = "/"),
              row.names=FALSE,
              col.names=FALSE)
  
  write.table(res$data,
              file = "dataGenNew/predict/data.csv",
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
              file = "dataGenNew/predict/desc.csv",
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
  #  z_class$param <- c(1.) # parameters for z_class are the mixture proportions
  z_class$allPresent <- FALSE
  z_class$allMissing <- TRUE
  z_class$fixedInitialization <- FALSE
  
  return(z_class)
}

zParamFixedInit <- function() {
  z_class <- list() # z_class must be the first variable in the list
  
  z_class$name <- "z_class"
  z_class$type <- "LatentClass"
  z_class$param <- c(0.3, 0.7) # parameters for z_class are the mixture proportions
#  z_class$param <- c(1.) # parameters for z_class are the mixture proportions
  z_class$allPresent <- TRUE
  z_class$allMissing <- FALSE
  z_class$fixedInitialization <- TRUE
  z_class$paramStr <- "fixedInitialization"
  
  return(z_class)
}
