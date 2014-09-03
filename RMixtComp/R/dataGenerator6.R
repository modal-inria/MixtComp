dataGenerator6 <- function()
{
  nbSamples <- 500
  
  nbVariablesCat <- 6
  nbVariablesGauss <- 6
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
  
  missingGaussian <- c(0.95, # present
                       0.05, # missing
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
  
  gaussianParams <- matrix(data = c(-50., -50., -50., -50., -50., -50.,
                                    5., 5., 5., 5., 5., 5.,
                                    
                                    0., 0., 0., 0., 0., 0.,
                                    5., 5., 5., 5., 5., 5.,
                                    
                                    50., 50., 50., 50., 50., 50., 
                                    5., 5., 5., 5., 5., 5.),
                           nrow = 2 * nbClasses,
                           ncol = nbVariablesGauss,
                           byrow = TRUE)
  
  proportions <- c(0.1,
                   0.3,
                   0.6)
  
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
  
  write.table(z,
              file = "classIn.csv",
              row.names=FALSE,
              col.names=FALSE)
}