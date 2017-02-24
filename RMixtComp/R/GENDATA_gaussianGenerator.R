completeGaussianData <- function(nbSamples,
                                 nbVariables,
                                 z,
                                 params)
{
  vals <- matrix(nrow = nbSamples,
                 ncol = nbVariables)
  for (i in 1:nbSamples)
  {
    for (j in 1:nbVariables)
    {
      vals[i, j] <- rnorm(1,
                          mean = params[2*z[i]-1, j],
                          sd   = params[2*z[i]  , j])
    }
  }
  colnames(vals) <- paste("gaussian",
                          1:nbVariables,
                          sep = "")
  return(vals)
}

missingGaussianData <- function(data,
                                z,
                                params,
                                missingParams)
{
  listMissingInd <- list()
  nbMissingVal <- 0
  for (i in 1:nrow(data))
  {
    nbVar <- ncol(data)
    nbSampleVar <- sample(nbVar - 1, 1) # number of modalities to be drawn
    sampledVar <- sort(sample(nbVar, nbSampleVar)) # modalities drawn
    isMissing <- F
    for (j in sampledVar)
    {
      missType <- match(1,
                        rmultinom(1,
                                  1,
                                  missingParams))
      if (missType == 2) # missing
      {
        data[i, j] <- "?"
        isMissing <- T
        nbMissingVal <- nbMissingVal + 1
      }
      else if (missType == 3) # missing interval
      {
        bounds <- sort(rnorm(2,
                             mean = params[2*z[i]-1, j],
                             sd   = params[2*z[i]  , j]))
        data[i, j] <- paste("[",
                            paste(bounds,
                                  collapse = ":"),
                            "]",
                            sep ="") # formatting for the data file
        isMissing <- T
        nbMissingVal <- nbMissingVal + 1
      }
      else if (missType == 4) # missing left unbounded
      {
        bounds <- list("-inf",
                       rnorm(1,
                             mean = params[2*z[i]-1, j],
                             sd   = params[2*z[i]  , j]))
        data[i, j] <- paste("[",
                            paste(bounds,
                                  collapse = ":"),
                            "]",
                            sep ="") # formatting for the data file
        isMissing <- T
        nbMissingVal <- nbMissingVal + 1
      }
      else if (missType == 5) # missing right unbounded
      {
        bounds <- list(rnorm(1,
                             mean = params[2*z[i]-1, j],
                             sd   = params[2*z[i]  , j]),
                       "+inf")
        data[i, j] <- paste("[",
                            paste(bounds,
                                  collapse = ":"),
                            "]",
                            sep ="") # formatting for the data file
        isMissing <- T
        nbMissingVal <- nbMissingVal + 1
      }
      if (isMissing == T)
      {
        listMissingInd <- append(listMissingInd, i)
      }
    }
  }
  return(list(data = data,
              listMissingInd = listMissingInd,
              nbMissingVal = nbMissingVal))
}

writeGaussianData <- function(fileName,
                              data)
{
  write.table(data,
              file = fileName,
              quote = FALSE,
              sep = ";",
              row.names = FALSE,
              dec = ".")
}

writeGaussianDataDescriptor <- function(fileName,
                                        nbVariables)
{
  data <- matrix(nrow = 2, ncol = nbVariables)
  data[1,] <- paste("gaussian",
                    1:nbVariables,
                    sep = "")
  data[2,] <- rep("Gaussian_sjk", nbVariables)
  
  write.table(data,
              file = fileName,
              quote = FALSE,
              sep = ";",
              row.names = FALSE,
              col.names = FALSE,
              dec = ".")
}

gaussianGenerator <- function(prefix,
                              nbSamples,
                              nbVariables,
                              z,
                              params,
                              missingParams)
{
  data <- completeGaussianData(nbSamples,
                               nbVariables,
                               z,
                               params)
  writeGaussianData(paste(prefix,
                          "gaussianData.complete.csv",
                          sep = "/"),
                    data)
  
  retList <- missingGaussianData(data,
                                 z,
                                 params,
                                 missingParams)
  
  writeGaussianData(paste(prefix,
                          "gaussianData.csv",
                          sep = "/"),
                    retList[["data"]])
  
  writeGaussianDataDescriptor(paste(prefix,
                                    "gaussianDescriptor.csv",
                                    sep = "/"),
                              nbVariables)
  
  return(list(listMissingInd = retList[["listMissingInd"]],
              nbMissingVal = retList[["nbMissingVal"]]))
}