missingZData <- function(z,
                         nbClasses,
                         missingParams)
{
  data <- matrix(nrow = length(z),
                 ncol = 1)
  for (i in 1:length(z))
  {
    missType <- match(1,
                      rmultinom(1,
                                1,
                                missingParams)) # type of missing value
    if (missType == 1) # data is present
    {
      data[i, 1] <- z[i]
    }
    if (missType == 2) # completely missing
    {
      data[i, 1] <- "?"
    }
    else if (missType == 3) # missing finite value
    {
      nbSampleMod <- sample(nbClasses, 1) - 1 # number of classes to be drawn in addition to the true one
      sampledClass <- sample(nbClasses, nbSampleMod) # sampled classes
      sampledClass <- sampledClass[which(sampledClass != z[i])]
      sampledClass <- c(z[i], sampledClass)
      modalities <- sort(sampledClass)
      data[i, 1] <- paste("{",
                          paste(modalities,
                                collapse = ","),
                          "}",
                          sep ="") # formatting for the data file
    }
  }
  colnames(data) <- "z_class"
  return(data)
}

writeZData <- function(fileName,
                       data)
{
  write.table(data,
              file = fileName,
              quote = FALSE,
              sep = ";",
              row.names = FALSE,
              dec = ".")
}

writeZDataDescriptor <- function(fileName)
{
  data <- matrix(nrow = 2,
                 ncol = 1)
  data[1,] <- "LatentClass"
  data[2,] <- "z_class"
  write.table(data,
              file = fileName,
              quote = FALSE,
              sep = ";",
              row.names = FALSE,
              col.names = FALSE,
              dec = ".")
}

zGenerator <- function(prefix,
                       z,
                       nbClasses,
                       missingZ)
{
  data <- missingZData(z,
                       nbClasses,
                       missingZ)
  writeZData(paste(prefix,
                   "zData.csv",
                   sep = "/"),
             data)
  
  writeZDataDescriptor(paste(prefix,
                             "zDescriptor.csv",
                             sep = "/"))
}