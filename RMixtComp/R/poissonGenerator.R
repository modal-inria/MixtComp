completePoissonData <- function(nbSamples,
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
      vals[i, j] <- rpois(1,
                          lambda = params[z[i], j])
    }
  }
  colnames(vals) <- paste("poisson",
                          1:nbVariables,
                          sep = "")
  return(vals)
}

missingPoissonData <- function(data,
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

writePoissonData <- function(fileName,
                              data)
{
  write.table(data,
              file = fileName,
              quote = FALSE,
              sep = ";",
              row.names = FALSE,
              dec = ".")
}

writePoissonDataDescriptor <- function(fileName,
                                        nbVariables)
{
  data <- matrix(nrow = 2, ncol = nbVariables)
  data[1,] <- rep("Poisson_k", nbVariables)
  data[2,] <- paste("poisson",
                    1:nbVariables,
                    sep = "")
  
  write.table(data,
              file = fileName,
              quote = FALSE,
              sep = ";",
              row.names = FALSE,
              col.names = FALSE,
              dec = ".")
}

poissonGenerator <- function(prefix,
                             nbSamples,
                             nbVariables,
                             z,
                             params,
                             missingParams)
{
  data <- completePoissonData(nbSamples,
                              nbVariables,
                              z,
                              params)
  writePoissonData(paste(prefix,
                         "poissonData.complete.csv",
                         sep = "/"),
                   data)
  
  retList <- missingPoissonData(data,
                                z,
                                params,
                                missingParams)
  
  writePoissonData(paste(prefix,
                          "poissonData.csv",
                          sep = "/"),
                    retList[["data"]])
  
  writePoissonDataDescriptor(paste(prefix,
                                    "poissonDescriptor.csv",
                                    sep = "/"),
                              nbVariables)
  
  return(list(listMissingInd = retList[["listMissingInd"]],
              nbMissingVal = retList[["nbMissingVal"]]))
}