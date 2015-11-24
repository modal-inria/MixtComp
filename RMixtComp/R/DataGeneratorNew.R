dataGeneratorNewTest <- function() {
  proportionMissing <- 0.2
  
  var <- list()
  var$z_class <- list() # z_class must be the first variable in the list
  
  var$z_class$name <- "z_class"
  var$z_class$type <- "LatentClass"
  var$z_class$param <- c(0.3, 0.7) # parameters for z_class are the mixture proportions
  
  matStr <- dataGeneratorNew("dataGenNew/learn", # prefix
                   500, # nbSample
                   proportionMissing,
                   var) # param
  
  write.table(matStr,
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
                      nrow = 2,
                      ncol = nbVar)
  
  for (j in 1:nbVar) {
    headerStr[1, j] <- var[[j]]$name
    headerStr[2, j] <- var[[j]]$type
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
  }
  
  matStr <- rbind(headerStr, dataStr)
  
  return(matStr)
  
  #   listMissing <- list()
  #   nbMissingVal <- 0
  #   
  #   zGenerator(prefix,
  #              z,
  #              nbClasses,
  #              missingZ)
  #   
  #   if (nbVariableCat > 0)
  #   {
  #     retList <- categoricalGenerator(prefix,
  #                                     nbSamples,
  #                                     nbVariableCat,
  #                                     nbModalities,
  #                                     z,
  #                                     categoricalParams,
  #                                     missingCategorical)
  #     listMissing <- union(listMissing, retList[["listMissingInd"]])
  #     nbMissingVal <- nbMissingVal + retList[["nbMissingVal"]]
  #   }
  #   
  #   if (nbVariableGauss > 0)
  #   {
  #     retList <- gaussianGenerator(prefix,
  #                                  nbSamples,
  #                                  nbVariableGauss,
  #                                  z,
  #                                  gaussianParams,
  #                                  missingGaussian)
  #     listMissing <- union(listMissing, retList[["listMissingInd"]])
  #     nbMissingVal <- nbMissingVal + retList[["nbMissingVal"]]
  #   }
  #   
  #   if (nbVariablePoisson > 0)
  #   {
  #     retList <- poissonGenerator(prefix,
  #                                 nbSamples,
  #                                 nbVariablePoisson,
  #                                 z,
  #                                 poissonParams,
  #                                 missingPoisson)
  #     listMissing <- union(listMissing, retList[["listMissingInd"]])
  #     nbMissingVal <- nbMissingVal + retList[["nbMissingVal"]]
  #   }
  #   
  #   nbMissing <- length(listMissing)
  #   nbTotalVal <- (nbSamples * (nbVariableCat + nbVariableGauss + nbVariablePoisson))
  #   
  #   fileConn <- file(paste(prefix,
  #                          "dataStat.txt",
  #                          sep = "/"))
  #   cat("Missing individuals / Total individuals: ", nbMissing, " / ", nbSamples, "\n",
  #       "Ratio missing individuals: ", nbMissing / nbSamples, "\n",
  #       "Missing values / Total values: ", nbMissingVal, " / ", nbTotalVal, "\n",
  #       "Ratio missing values: ", nbMissingVal / nbTotalVal, "\n",
  #       file = fileConn,
  #       sep = "")
  #   close(fileConn)
  #   
  #   write.table(z,
  #               file = paste(prefix,
  #                            "classIn.csv",
  #                            sep = "/"),
  #               row.names=FALSE,
  #               col.names=FALSE)
}
