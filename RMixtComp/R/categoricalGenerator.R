categoricalGenerator <- function(nbSamples,
                                 nbVariables,
                                 nbModalities,
                                 z,
                                 params,
                                 missingParams)
{  
  compositeFile <- file(description = "categoricalData.csv",
                        open = "w",
                        blocking = TRUE,
                        encoding = getOption("encoding"),
                        raw = FALSE)
  
  descriptorFile <- file(description = "categoricalDescriptor.csv",
                         open = "w",
                         blocking = TRUE,
                         encoding = getOption("encoding"),
                         raw = FALSE)
  
  vals <- paste("var",
                1:nbVariables,
                sep = "")
  cat(vals,
      sep = ";",
      file = compositeFile)
  cat("\n",
      file = compositeFile)
  vals <- rep("Categorical_pjk",
              nbVariables)
  
  cat(vals,
      sep = ";",
      file = descriptorFile)
  cat("\n",
      file = descriptorFile)
  vals <- paste("categorical",
                1:nbVariables,
                sep = "")
  cat(vals,
      sep = ";",
      file = descriptorFile)
  cat("\n",
      file = descriptorFile)
  
  for (i in 1:nbSamples)
  {
    vals <- list()
    for (j in 1:nbVariables)
    {
      missType <- match(1,
                        rmultinom(1,
                                  1,
                                  missingParams))
      if (missType == 1) # normal, no missing value
      {
        sampleVals <- rmultinom(1,
                                1,
                                params[(nbModalities * (z[i] - 1) + 1) :
                                       (nbModalities *  z[i]         ) , j])
        vals[[j]] <- match(1, sampleVals) - 1
      }
      else if (missType == 2) # completely missing
      {
        vals[[j]] <- "?"
      }
      else if (missType == 3) # missing finite value
      {
        nbSampleMod <- sample(nbModalities, 1) # number of modalities to be drawn
        modalities <- sort(sample(0 : (nbModalities - 1), nbSampleMod)) # modalities drawn
        vals[[j]] <- paste("{",
                           paste(modalities,
                                 collapse = ","),
                           "}",
                           sep ="") # formatting for the data file
      }
    }
    cat(as.character(vals),
        sep = ";",
        file = compositeFile)
    cat("\n",
        file = compositeFile)
  }
  close(compositeFile) 
}