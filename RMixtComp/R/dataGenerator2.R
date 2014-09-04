dataGenerator2 <- function()
{
  nbSamples <- 500
  
  nbVariablesCat <- 6
  nbVariablesGauss <- 2
  minModality <- 1
  nbModalities <- 3
  nbClasses <- 2
  
#    missingCategorical <- c(0.7, # present
#                            0.15, # missing
#                            0.15) # missing finite value
#    missingGaussian <- c(0.6, # present
#                         0.1, # missing
#                         0.1, # missing interval
#                         0.1, # missing left unbounded
#                         0.1) # missing right unbounded

  missingCategorical <- c(0.8, # present
                          0.1, # missing
                          0.1) # missing finite value
  missingGaussian <- c(0.8, # present
                       0.2, # missing
                       0., # missing interval
                       0., # missing left unbounded
                       0.) # missing right unbounded

  categoricalParams <- matrix(data = c(0.3,0.3,0.3,0.3,0.3,0.3,
                                       0.6,0.6,0.6,0.6,0.6,0.6,
                                       0.1,0.1,0.1,0.1,0.1,0.1,
                                       
                                       0.6,0.6,0.6,0.6,0.6,0.6,
                                       0.1,0.1,0.1,0.1,0.1,0.1,
                                       0.3,0.3,0.3,0.3,0.3,0.3),
                              nrow = nbModalities * nbClasses,
                              ncol = nbVariablesCat,
                              byrow = TRUE)
  
  gaussianParams <- matrix(data = c(-50.,-50.,
                                    5.,5.,
                                    
                                    50.,50.,
                                    5.,5.),
                           nrow = 2 * nbClasses,
                           ncol = nbVariablesGauss,
                           byrow = TRUE)
  
  proportions <- c(0.5, 0.5)
  
  zDis <- rmultinom(nbSamples,
                    1,
                    proportions)
  z <- rep(0, nbSamples)
  for (i in 1:nbSamples)
  {
    z[i] <- match(1, zDis[, i])
  }
  
  retCat <- categoricalGenerator(nbSamples,
                                 nbVariablesCat,
                                 nbModalities,
                                 z,
                                 categoricalParams,
                                 missingCategorical,
                                 minModality)
  retGauss <- gaussianGenerator(nbSamples,
                                nbVariablesGauss,
                                z,
                                gaussianParams,
                                missingGaussian)
  listMissing <- union(retCat[["listMissingInd"]], retGauss[["listMissingInd"]])
  nbMissing <- length(listMissing)
  nbMissingVal <- retGauss[["nbMissingVal"]] + retCat[["nbMissingVal"]]
  nbTotalVal <- (nbSamples * (nbVariablesCat + nbVariablesGauss))

  fileConn <- file("dataGen/dataStat.txt")
  cat("Missing individuals / Total individuals: ", nbMissing, " / ", nbSamples, "\n",
      "Ratio missing individuals: ", nbMissing / nbSamples, "\n",
      "Missing values / Total values: ", nbMissingVal, " / ", nbTotalVal, "\n",
      "Ratio missing values: ", nbMissing / nbTotalVal, "\n",
      file = fileConn,
      sep = "")
  close(fileConn)

  write.table(z,
            file = "dataGen/classIn.csv",
            row.names=FALSE,
            col.names=FALSE)
}