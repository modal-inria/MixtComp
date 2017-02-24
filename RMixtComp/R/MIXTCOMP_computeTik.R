
computeTik <- function(outputMixtComp)
{
  
  variables <- names(outputMixtComp$variable$param)[-1]
  proportion <- outputMixtComp$variable$param$z_class$pi$stat[,1]
  
  probaVariable <- lapply(variables, computeProbaOneVariable, outputMixtComp)
  
  tik <- Reduce("*", probaVariable)
  tik = t(apply(tik, 1, function(x){x * proportion}))
  tik = tik/rowSums(tik)  
  
  return(tik)
}

# fait appel à la bonne fonction de calcul de proba en fonction du modèle de la variable
computeProbaOneVariable <- function(name, outputMixtComp)
{
  
  typeVar <- outputMixtComp$variable$type[[name]]
  
  proba <- switch(typeVar,
                  "Gaussian_sjk"   = computeProbaGaussian(name, outputMixtComp),
                  "Categorical_pjk"= computeProbaCategorical(name, outputMixtComp))
  
  return(proba)
}


# cas gaussien
computeProbaGaussian <- function(name, outputMixtComp)
{
  param <- outputMixtComp$variable$param[[name]]$NumericalParam$stat[,1]
  donneesVariable <- outputMixtComp$variable$data[[name]]$completed
  nbClass <- outputMixtComp$mixture$nbCluster
  
  proba <- matrix(0, nrow = length(donneesVariable), ncol = nbClass)
  for(i in 1:nbClass)
  {
    proba[,i] <- dnorm(donneesVariable, param[(i-1)*2 + 1], param[2*i])
  }
  
  return(proba)
}



# cas catégoriel
computeProbaCategorical <- function(name, outputMixtComp)
{
  param <- outputMixtComp$variable$param[[name]]$NumericalParam$stat[,1]
  donneesVariable <- outputMixtComp$variable$data[[name]]$completed
  nbClass <- outputMixtComp$mixture$nbCluster
  nbModalities <- length(param)/nbClass
  
  proba <- matrix(0, nrow = length(donneesVariable), ncol = nbClass)
  for(i in 1:nbClass)
  {
    proba[,i] <- param[nbModalities * (i-1) + 1:nbModalities][donneesVariable]
  }
  
  return(proba)
}
