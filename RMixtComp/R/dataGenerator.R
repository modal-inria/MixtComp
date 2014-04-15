dataGenerator <- function()
{
  nbSamples <- 5000
  
  nbVariables <- 2
  nbModalities <- 2
  nbClasses <- 3
  
  categoricalParams <- matrix(data = c(0.5,0.1,
                                       0.5,0.9,
                                       
                                       0.1,0.4,
                                       0.9,0.6,
                                       
                                       0.6,0.2,
                                       0.4,0.8),
                              nrow = nbModalities * nbClasses,
                              ncol = nbVariables,
                              byrow = TRUE)
  
  gaussianParams <- matrix(data = c(100.,200.,
                                    0.5,10.,
                                    
                                    0.,12.,
                                    0.1,30.,
                                    
                                    -100.,-350.,
                                    0.2,5.),
                           nrow = 2 * nbClasses,
                           ncol = nbVariables,
                           byrow = TRUE)
  
  proportions <- c(0.2, 0.7, 0.1)
  
  zDis <- rmultinom(nbSamples,
                    1,
                    proportions)
  z <- rep(0, nbSamples)
  for (i in 1:nbSamples)
  {
    z[i] <- match(1, zDis[, i])
  }
  
  gaussianGenerator(nbSamples,
                    nbVariables,
                    z,
                    gaussianParams)
  categoricalGenerator(nbSamples,
                       nbVariables,
                       nbModalities,
                       z,
                       categoricalParams)  
}