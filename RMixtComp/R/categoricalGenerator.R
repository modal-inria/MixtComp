categoricalGenerator <- function(nbSamples,
                                 nbVariables,
                                 nbModalities,
                                 z,
                                 params)
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
    vals <- rep(0, nbVariables)
    for (j in 1:nbVariables)
    {
      sampleVals <- rmultinom(1,
                              1,
                              params[(nbModalities * (z[i] - 1) + 1) :
                                     (nbModalities *  z[i]         ) , j])
      vals[j] <- match(1, sampleVals) - 1
    }
    cat(vals,
        sep = ";",
        file = compositeFile)
    cat("\n",
        file = compositeFile)
  }
  
  close(compositeFile) 
}