completeCategoricalData <- function(nbSamples,
                                    nbVariables,
                                    nbModalities,
                                    z,
                                    params,
                                    minModality)
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
      vals[i, j] <- match(1, sampleVals) + minModality - 1
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
                                   missingParams,
                                   minModality)
{
  nbMissing <- 0
  for (i in 1:nrow(data))
  {
    for (j in 1:ncol(data))
    {
      missType <- match(1,
                        rmultinom(1,
                                  1,
                                  missingParams))
      if (missType == 2) # completely missing
      {
        data[i, j] <- "?"
        nbMissing <- nbMissing + 1
      }
      else if (missType == 3) # missing finite value
      {
        nbSampleMod <- sample(nbModalities, 1) # number of modalities to be drawn
        modalities <- sort(sample(minModality : (nbModalities + minModality - 1), nbSampleMod)) # modalities drawn
        data[i, j] <- paste("{",
                            paste(modalities,
                                  collapse = ","),
                            "}",
                            sep ="") # formatting for the data file
        nbMissing <- nbMissing + 1
      }
    }
  }
  return(list(data = data,
              nbMissing = nbMissing))
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

writeCategoricalDataDescriptor <- function(nbVariables)
{
  data <- matrix(nrow = 2, ncol = nbVariables)
  data[1,] <- rep("Categorical_pjk", nbVariables)
  data[2,] <- paste("categorical",
                    1:nbVariables,
                    sep = "")
  
  write.table(data,
              file = "dataGen/categoricalDescriptor.csv",
              quote = FALSE,
              sep = ";",
              row.names = FALSE,
              col.names = FALSE,
              dec = ".")
}

categoricalGenerator <- function(nbSamples,
                                 nbVariables,
                                 nbModalities,
                                 z,
                                 params,
                                 missingParams,
                                 minModality)
{
  data <- completeCategoricalData(nbSamples,
                                  nbVariables,
                                  nbModalities,
                                  z,
                                  params,
                                  minModality)
  writeGaussianData("dataGen/categoricalData.complete.csv",
                    data)
  retList <- missingCategoricalData(data,
                                    nbModalities,
                                    z,
                                    params,
                                    missingParams,
                                    minModality)
  data <- retList[["data"]]
  nbMissing <- retList[["nbMissing"]]
  writeCategoricalData("dataGen/categoricalData.csv",
                       data)
  writeCategoricalDataDescriptor(nbVariables)
  return(nbMissing)
}