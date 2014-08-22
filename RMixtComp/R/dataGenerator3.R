dataGenerator3 <- function()
{
  nbSamples <- 500
  
  nbVariablesCat <- 6
  nbVariablesGauss <- 3
  minModality <- 1
  nbModalities <- 3
  nbClasses <- 3
  
  #    missingCategorical <- c(0.7, # present
  #                            0.15, # missing
  #                            0.15) # missing finite value
  #    missingGaussian <- c(0.6, # present
  #                         0.1, # missing
  #                         0.1, # missing interval
  #                         0.1, # missing left unbounded
  #                         0.1) # missing right unbounded
  
  missingCategorical <- c(1., # present
                          0., # missing
                          0.) # missing finite value
  
  missingGaussian <- c(0.995, # present
                       0.005, # missing
                       0., # missing interval
                       0., # missing left unbounded
                       0.) # missing right unbounded

#   missingGaussian <- c(1., # present
#                        0., # missing
#                        0., # missing interval
#                        0., # missing left unbounded
#                        0.) # missing right unbounded
  
  categoricalParams <- matrix(data = c(0.3,0.3,0.3,0.3,0.3,0.3,
                                       0.6,0.6,0.6,0.6,0.6,0.6,
                                       0.1,0.1,0.1,0.1,0.1,0.1,
                                       
                                       0.6,0.6,0.6,0.6,0.6,0.6,
                                       0.1,0.1,0.1,0.1,0.1,0.1,
                                       0.3,0.3,0.3,0.3,0.3,0.3),
                              nrow = nbModalities * nbClasses,
                              ncol = nbVariablesCat,
                              byrow = TRUE)
  
  gaussianParams <- matrix(data = c(-500.,-60., -200.,
                                       5.,  1.,    3.,
    
                                       0.,  12.,-50.,
                                       0.5, 10.,  2.,
                                    
                                    100.   ,200.,300.,
                                    0.1  ,5. ,   3.),
                           nrow = 2 * nbClasses,
                           ncol = nbVariablesGauss,
                           byrow = TRUE)
  
  proportions <- c(0.1, 0.6, 0.3)
  
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
                       missingCategorical,
                       minModality)
  gaussianGenerator(nbSamples,
                    nbVariablesGauss,
                    z,
                    gaussianParams,
                    missingGaussian)
}