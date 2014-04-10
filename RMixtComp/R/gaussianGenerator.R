gaussianGenerator <- function(nbSamples,
                              nbVariables,
                              z,
                              params)
{  
  compositeFile <- file(description = "gaussianData.csv",
                        open = "w",
                        blocking = TRUE,
                        encoding = getOption("encoding"),
                        raw = FALSE)
  
  descriptorFile <- file(description = "gaussianDescriptor.csv",
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
  vals <- rep("Gaussian_sjk",
              nbVariables)
  
  cat(vals,
      sep = ";",
      file = descriptorFile)
  cat("\n",
      file = descriptorFile)
  vals <- paste("gaussian",
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
      cat(params[2*z[i]-1, j], params[2*z[i]  , j])
      vals[j] <- rnorm(1,
                       mean = params[2*z[i]-1, j],
                       sd   = params[2*z[i]  , j])
    }
    cat(vals,
        sep = ";",
        file = compositeFile)
    cat("\n",
        file = compositeFile)
  }
  
  close(compositeFile)
}