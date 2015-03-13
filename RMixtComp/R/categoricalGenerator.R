completeCategoricalData <- function(nbSamples,
                                    nbVariables,
                                    nbModalities,
                                    z,
                                    params)
{
  vals <- matrix(nrow = nbSamples,
                 ncol = nbVariables)
  for (i in 1:nbSamples)
  {
    for (j in 1:nbVariables)
    {
      sampleVals <- rmultinom(1,
                              1,
                              params[(nbModalities * (z[i] - 1) + 1) :
                                       (nbModalities *  z[i]         ) , j])
      vals[i, j] <- match(1, sampleVals)
    }
  }
  colnames(vals) <- paste("categorical",
                          1:nbVariables,
                          sep = "")
  return(vals)
}

missingCategoricalData <- function(data,
                                   nbModalities,
                                   z,
                                   params,
                                   missingParams)
{
  listMissingInd <- list()
  nbMissingVal <- 0
  for (i in 1:nrow(data))
  {
    nbVar <- ncol(data)
    nbSampleVar <- sample(nbVar - 1, 1) # number of variables to be missing
    sampledVar <- sort(sample(nbVar, nbSampleVar)) # missing variables
    isMissing <- F
    for (j in sampledVar)
    {
      missType <- match(1,
                        rmultinom(1,
                                  1,
                                  missingParams))
      if (missType == 2) # completely missing
      {
        data[i, j] <- "?"
        isMissing <- T
        nbMissingVal <- nbMissingVal + 1
      }
      else if (missType == 3) # missing finite value
      {
        nbSampleMod <- sample(nbModalities, 1) # number of modalities to be drawn
        modalities <- sort(sample(1 : nbModalities, nbSampleMod)) # modalities drawn
        data[i, j] <- paste("{",
                            paste(modalities,
                                  collapse = ","),
                            "}",
                            sep ="") # formatting for the data file
        isMissing <- T
        nbMissingVal <- nbMissingVal + 1
      }
    }
    if (isMissing == T)
    {
      listMissingInd <- append(listMissingInd, i)
    }
  }
  return(list(data = data,
              listMissingInd = listMissingInd,
              nbMissingVal = nbMissingVal))
}

writeCategoricalData <- function(fileName,
                                 data)
{
  write.table(data,
              file = fileName,
              quote = FALSE,
              sep = ";",
              row.names = FALSE,
              dec = ".")
}

writeCategoricalDataDescriptor <- function(fileName,
                                           nbVariables)
{
  data <- matrix(nrow = 2, ncol = nbVariables)
  data[1,] <- rep("Categorical_pjk", nbVariables)
  data[2,] <- paste("categorical",
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

categoricalGenerator <- function(prefix,
                                 nbSamples,
                                 nbVariables,
                                 nbModalities,
                                 z,
                                 params,
                                 missingParams)
{
  data <- completeCategoricalData(nbSamples,
                                  nbVariables,
                                  nbModalities,
                                  z,
                                  params)
  
  writeCategoricalData(paste(prefix,
                             "categoricalData.complete.csv",
                             sep = "/"),
                       data)
  
  retList <- missingCategoricalData(data,
                                    nbModalities,
                                    z,
                                    params,
                                    missingParams)
  
  writeCategoricalData(paste(prefix,
                             "categoricalData.csv",
                             sep = "/"),
                       retList[["data"]])

  writeCategoricalDataDescriptor(paste(prefix,
                                       "categoricalDescriptor.csv",
                                       sep = "/"),
                                 nbVariables)
  
  return(list(listMissingInd = retList[["listMissingInd"]],
              nbMissingVal = retList[["nbMissingVal"]]))
}