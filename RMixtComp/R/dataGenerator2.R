dataGenerator2 <- function()
{
  nbSamples <- 500
  
  nbVariableCat <- 0
  nbVariableGauss <- 2
  minModality <- 1
  nbModalities <- 3
  nbClasses <- 2
  
  proportions <- c(0.5, 0.5)
  
  zDis <- rmultinom(nbSamples,
                    1,
                    proportions)
  z <- rep(0, nbSamples)
  for (i in 1:nbSamples)
  {
    z[i] <- match(1, zDis[, i])
  }
  
  listMissing <- list()
  nbMissingVal <- 0
  
  if (nbVariableCat > 0)
  {
    missingCategorical <- c(0.8, # present
                            0.1, # missing
                            0.1) # missing finite value
    categoricalParams <- matrix(data = c(0.3,0.3,0.3,0.3,0.3,0.3,
                                         0.6,0.6,0.6,0.6,0.6,0.6,
                                         0.1,0.1,0.1,0.1,0.1,0.1,
                                         
                                         0.6,0.6,0.6,0.6,0.6,0.6,
                                         0.1,0.1,0.1,0.1,0.1,0.1,
                                         0.3,0.3,0.3,0.3,0.3,0.3),
                                nrow = nbModalities * nbClasses,
                                ncol = nbVariableCat,
                                byrow = TRUE)
    retList <- categoricalGenerator(nbSamples,
                                    nbVariableCat,
                                    nbModalities,
                                    z,
                                    categoricalParams,
                                    missingCategorical,
                                    minModality)
    listMissing <- union(listMissing, retList[["listMissingInd"]])
    nbMissingVal <- nbMissingVal + retList[["nbMissingVal"]]
  }
  if (nbVariableGauss > 0)
  {
    missingGaussian <- c(0.5, # present
                         0.5, # missing
                         0., # missing interval
                         0., # missing left unbounded
                         0.) # missing right unbounded
    gaussianParams <- matrix(data = c(-50.,-50.,
                                      5.,5.,
                                      
                                      50.,50.,
                                      5.,5.),
                             nrow = 2 * nbClasses,
                             ncol = nbVariableGauss,
                             byrow = TRUE)
    retList <- gaussianGenerator(nbSamples,
                                 nbVariableGauss,
                                 z,
                                 gaussianParams,
                                 missingGaussian)
    listMissing <- union(listMissing, retList[["listMissingInd"]])
    nbMissingVal <- nbMissingVal + retList[["nbMissingVal"]]
  }

  nbMissing <- length(listMissing)
  nbTotalVal <- (nbSamples * (nbVariableCat + nbVariableGauss))

  fileConn <- file("dataGen/dataStat.txt")
  cat("Missing individuals / Total individuals: ", nbMissing, " / ", nbSamples, "\n",
      "Ratio missing individuals: ", nbMissing / nbSamples, "\n",
      "Missing values / Total values: ", nbMissingVal, " / ", nbTotalVal, "\n",
      "Ratio missing values: ", nbMissingVal / nbTotalVal, "\n",
      file = fileConn,
      sep = "")
  close(fileConn)

  write.table(z,
            file = "dataGen/classIn.csv",
            row.names=FALSE,
            col.names=FALSE)
}