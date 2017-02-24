dataGenerator <- function(prefix,
                          categoricalParams,
                          gaussianParams,
                          poissonParams,
                          proportions,
                          nbSamples,
                          nbVariableCat,
                          nbModalities,
                          nbVariableGauss,
                          nbVariablePoisson,
                          nbClasses,
                          missingZ,
                          missingCategorical,
                          missingGaussian,
                          missingPoisson)
{
  zDis <- rmultinom(nbSamples,
                    1,
                    proportions)
  z <- vector("integer",
              nbSamples)
  for (i in 1:nbSamples)
  {
    z[i] <- match(1, zDis[, i])
  }
  
  listMissing <- list()
  nbMissingVal <- 0
  
  zGenerator(prefix,
             z,
             nbClasses,
             missingZ)
  
  if (nbVariableCat > 0)
  {
    retList <- categoricalGenerator(prefix,
                                    nbSamples,
                                    nbVariableCat,
                                    nbModalities,
                                    z,
                                    categoricalParams,
                                    missingCategorical)
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
  
  if (nbVariablePoisson > 0)
  {
    retList <- poissonGenerator(prefix,
                                nbSamples,
                                nbVariablePoisson,
                                z,
                                poissonParams,
                                missingPoisson)
    listMissing <- union(listMissing, retList[["listMissingInd"]])
    nbMissingVal <- nbMissingVal + retList[["nbMissingVal"]]
  }
  
  nbMissing <- length(listMissing)
  nbTotalVal <- (nbSamples * (nbVariableCat + nbVariableGauss + nbVariablePoisson))
  
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

dataParamGenerator <- function(nbSamplesLearn,
                               nbSamplesPredict,
                               nbVariableCat,
                               nbModalities,
                               nbVariableGauss,
                               maxMean,
                               maxSD,
                               nbVariablePoisson,
                               maxLambda,
                               nbClasses,
                               missingZ,
                               missingCategorical,
                               missingGaussian,
                               missingPoisson)
{
  proportions <- runif(nbClasses)
  proportions <- proportions / sum(proportions)
  write.table(proportions,
              file = "dataGen/param/proportions.csv",
              sep = ";",
              row.names=FALSE,
              col.names=FALSE)
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
                        maxSD   * runif(1))
#         proba <- c(2. * maxMean * (runif(1) - 0.5), # null variance version
#                    0)
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
  
  if (nbVariablePoisson > 0)
  {
    poissonParams <- matrix(nrow = nbClasses,
                            ncol = nbVariablePoisson)
    for (j in 1:nbVariablePoisson)
    {
      for (k in 1:nbClasses)
      {
        proba <- maxLambda * runif(1)
        poissonParams[k, j] <- proba
      }
    }
    write.table(poissonParams,
                file = "dataGen/param/poissonParams.csv",
                sep = ";",
                row.names=FALSE,
                col.names=FALSE)
  }
  
  dataGenerator("dataGen/learn",
                categoricalParams,
                gaussianParams,
                poissonParams,
                proportions,
                nbSamplesLearn,
                nbVariableCat,
                nbModalities,
                nbVariableGauss,
                nbVariablePoisson,
                nbClasses,
                missingZ,
                missingCategorical,
                missingGaussian,
                missingPoisson)
  dataGenerator("dataGen/predict",
                categoricalParams,
                gaussianParams,
                poissonParams,
                proportions,
                nbSamplesPredict,
                nbVariableCat,
                nbModalities,
                nbVariableGauss,
                nbVariablePoisson,
                nbClasses,
                missingZ,
                missingCategorical,
                missingGaussian,
                missingPoisson)
}