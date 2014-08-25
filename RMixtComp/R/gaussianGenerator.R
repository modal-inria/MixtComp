gaussianGenerator <- function(nbSamples,
                              nbVariables,
                              z,
                              params,
                              missingParams)
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
    vals <- list()
    for (j in 1:nbVariables)
    {
      missType <- match(1,
                        rmultinom(1,
                                  1,
                                  missingParams))
      if (missType == 1) # normal, no missing value
      {
        vals[[j]] <- rnorm(1,
                         mean = params[2*z[i]-1, j],
                         sd   = params[2*z[i]  , j])
      }
      else if (missType == 2) # missing
      {
        vals[[j]] <- "?"
      }
      else if (missType == 3) # missing interval
      {
        bounds <- sort(rnorm(2,
                             mean = params[2*z[i]-1, j],
                             sd   = params[2*z[i]  , j]))
        vals[[j]] <- paste("[",
                           paste(bounds,
                                 collapse = ":"),
                           "]",
                           sep ="") # formatting for the data file
      }
      else if (missType == 4) # missing left unbounded
      {
        bounds <- list("-inf",
                       rnorm(1,
                             mean = params[2*z[i]-1, j],
                             sd   = params[2*z[i]  , j]))
        vals[[j]] <- paste("[",
                           paste(bounds,
                                 collapse = ":"),
                           "]",
                           sep ="") # formatting for the data file
      }
      else if (missType == 5) # missing right unbounded
      {
        bounds <- list(rnorm(1,
                             mean = params[2*z[i]-1, j],
                             sd   = params[2*z[i]  , j]),
                       "+inf")
        vals[[j]] <- paste("[",
                           paste(bounds,
                                 collapse = ":"),
                           "]",
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
  close(descriptorFile)
}