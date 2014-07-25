dataGenerator <- function()
{
  nbSamples <- 500
  
  nbVariablesCat <- 6
  nbVariablesGauss <- 2
  nbModalities <- 3
  nbClasses <- 2
  
  missingCategorical <- c(0.7, # present
                          0.15, # missing
                          0.15) # missing finite value
  missingGaussian <- c(0.6, # present
                       0.1, # missing
                       0.1, # missing interval
                       0.1, # missing left unbounded
                       0.1) # missing right unbounded
  
  categoricalParams <- matrix(data = c(0.3,0.3,0.3,0.3,0.3,0.3,
                                       0.6,0.6,0.6,0.6,0.6,0.6,
                                       0.1,0.1,0.1,0.1,0.1,0.1,
                                       
                                       0.6,0.6,0.6,0.6,0.6,0.6,
                                       0.1,0.1,0.1,0.1,0.1,0.1,
                                       0.3,0.3,0.3,0.3,0.3,0.3),
                              nrow = nbModalities * nbClasses,
                              ncol = nbVariablesCat,
                              byrow = TRUE)
  
  gaussianParams <- matrix(data = c(100.,200.,
                                    0.5,10.,
                                    
                                    0.,12.,
                                    0.1,30.),
                           nrow = 2 * nbClasses,
                           ncol = nbVariablesGauss,
                           byrow = TRUE)
  
  proportions <- c(0.3, 0.7)
  
  zDis <- rmultinom(nbSamples,
                    1,
                    proportions)
  z <- rep(0, nbSamples)
  for (i in 1:nbSamples)
  {
    z[i] <- match(1, zDis[, i])
  }
  
  categoricalGenerator(nbSamples,
                       nbVariablesCat,
                       nbModalities,
                       z,
                       categoricalParams,
                       missingCategorical) 
  gaussianGenerator(nbSamples,
                    nbVariablesGauss,
                    z,
                    gaussianParams,
                    missingGaussian)
}