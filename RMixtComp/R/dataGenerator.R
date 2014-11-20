dataGenerator <- function(prefix,
                          categoricalParams,
                          gaussianParams,
                          proportions,
                          nbSamples,
                          nbVariableCat,
                          minModality,
                          nbModalities,
                          nbVariableGauss,
                          nbClasses,
                          missingCategorical,
                          missingGaussian)
{
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
    retList <- categoricalGenerator(prefix,
                                    nbSamples,
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
    retList <- gaussianGenerator(prefix,
                                 nbSamples,
                                 nbVariableGauss,
                                 z,
                                 gaussianParams,
                                 missingGaussian)
    listMissing <- union(listMissing, retList[["listMissingInd"]])
    nbMissingVal <- nbMissingVal + retList[["nbMissingVal"]]
  }
  
  nbMissing <- length(listMissing)
  nbTotalVal <- (nbSamples * (nbVariableCat + nbVariableGauss))
  
  fileConn <- file(paste(prefix,
                         "dataStat.txt",
                         sep = "/"))
  cat("Missing individuals / Total individuals: ", nbMissing, " / ", nbSamples, "\n",
      "Ratio missing individuals: ", nbMissing / nbSamples, "\n",
      "Missing values / Total values: ", nbMissingVal, " / ", nbTotalVal, "\n",
      "Ratio missing values: ", nbMissingVal / nbTotalVal, "\n",
      file = fileConn,
      sep = "")
  close(fileConn)
  
  write.table(z,
              file = paste(prefix,
                           "classIn.csv",
                           sep = "/"),
              row.names=FALSE,
              col.names=FALSE)
}

dataParamGenerator <- function(proportions,
                               nbSamplesLearn,
                               nbSamplesPredict,
                               nbVariableCat,
                               minModality,
                               nbModalities,
                               nbVariableGauss,
                               maxMean,
                               maxVar,
                               nbClasses,
                               missingCategorical,
                               missingGaussian)
{
  cat("dataParamGenerator\n")
  if (nbVariableCat > 0)
  {
    categoricalParams <- matrix(nrow = nbClasses * nbModalities,
                                ncol = nbVariableCat)
    for (j in 1:nbVariableCat)
    {
      for (n in 1:nbClasses)
      {
        proba <- runif(nbModalities) # proba of each modality for the current class
        proba <- proba / sum(proba)
        categoricalParams[((n - 1) * nbModalities + 1) :
                          ( n      * nbModalities    ) , j] <- proba
      }
    }
    write.table(categoricalParams,
                file = "dataGen/param/categoricalParams.csv",
                sep = ";",
                row.names=FALSE,
                col.names=FALSE)
  }
  
  if (nbVariableGauss > 0)
  {
    gaussianParams <- matrix(nrow = 2 * nbClasses,
                             ncol = nbVariableGauss)
    for (j in 1:nbVariableGauss)
    {
      for (n in 1:nbClasses)
      {
        proba <- c(2. * maxMean * (runif(1) - 0.5),
                        maxVar  * runif(1))
        gaussianParams[((n - 1) * 2 + 1) :
                       ( n      * 2    ) , j] <- proba
      }
    }
    write.table(gaussianParams,
                file = "dataGen/param/gaussianParams.csv",
                sep = ";",
                row.names=FALSE,
                col.names=FALSE)
  }
  cat("dataGenerator\n")
  dataGenerator("dataGen/learn",
                categoricalParams,
                gaussianParams,
                proportions,
                nbSamplesLearn,
                nbVariableCat,
                minModality,
                nbModalities,
                nbVariableGauss,
                nbClasses,
                missingCategorical,
                missingGaussian)
  dataGenerator("dataGen/predict",
                categoricalParams,
                gaussianParams,
                proportions,
                nbSamplesPredict,
                nbVariableCat,
                minModality,
                nbModalities,
                nbVariableGauss,
                nbClasses,
                missingCategorical,
                missingGaussian)
}