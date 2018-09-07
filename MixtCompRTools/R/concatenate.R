computeModelLnCompletedLikelihood <- function(resParent, resChild, classParentOfChild)
{
  # likelihood from parent classification
  partParent <- resParent$variable$data$z_class$completed

  classParentToKeep <- (1:resParent$mixture$nbCluster)[-classParentOfChild]
  lnCompLikelihoodParent <- 0
  for(k in classParentToKeep)
    lnCompLikelihoodParent = lnCompLikelihoodParent + sum(resParent$mixture$lnProbaGivenClass[partParent == k, k])


  # likelihood from child classification
  proportionParent <- resParent$variable$param$z_class$pi$stat[,1]

  lnCompLikelihoodChild <- resChild$mixture$lnCompletedLikelihood + resChild$mixture$nbInd * log(proportionParent[classParentOfChild])


  lnCompLikelihood <- lnCompLikelihoodParent + lnCompLikelihoodChild

  return(lnCompLikelihood)
}

# Concatenation of param element of 2 MixtComp output
#
# During a hierarchical MixtComp, given the ouput oh the parent and one child, create the param output containing the model generated
#
# @param resParent result object of a MixtComp run.
# @param resChild result object of a MixtComp run. This run is performed on the individuals of one cluster of resParent
# @param classParentOfChild cluster of the parent used for the child
#
# @return a param output (in MixtComp format)
#
concatenateParam <- function(resParent, resChild, classParentOfChild)
{
  nbClassParent <- resParent$mixture$nbCluster
  nbClassChild <- resChild$mixture$nbCluster

  if((classParentOfChild <= 0) || (classParentOfChild > nbClassParent))
    stop("Wrong classParentOfChild value.")

  classParent <- resParent$variable$data$z_class$completed
  nbIndChild <- resChild$mixture$nbInd

  if(nbIndChild != sum(classParent == classParentOfChild))
    stop("The size of the parent's cluster and the size of the child's clusters don't match.")

  varName <- names(resParent$variable$param)
  param <- list()
  for(name in varName)
    param[[name]] = concatenateParamOneModel(resParent$variable$param[[name]], resParent$variable$param[[name]], classParentOfChild, resParent$variable$type[[name]])

  return(param)
}


# concatenation of two parameters output in one
# paramParent = outPutMixtComp$variable$param$Variable
# model = model associated with the variable ("Gaussian_sjk", ...)
concatenateParamOneModel <- function(paramParent, paramChild, classParentOfChild, model)
{
  model = ifelse(model == "FunctionalSharedAlpha", "Functional", model)
  model = ifelse(model %in% c("Gaussian_sjk", "Weibull", "NegativeBinomial"), "2NumParam", model)

  out <- switch(model,
                "LatentClass" = concatenateParamZclass(paramParent, paramChild, classParentOfChild),
                "2NumParam" = concatenateParamNumeric2Param(paramParent, paramChild, classParentOfChild),
                "Categorical_pjk" = concatenateParamPoisson(paramParent, paramChild, classParentOfChild),
                "Functional" = concatenateParamFunctional(paramParent, paramChild, classParentOfChild),
                "Poisson_k" = concatenateParamPoisson(paramParent, paramChild, classParentOfChild),
                "Rank" = concatenateParamRank(paramParent, paramChild, classParentOfChild))

  return(out)
}


concatenateParamZclass <- function(paramParent, paramChild, classParentOfChild)
{
  param <- list(pi = list(stat = rbind(paramParent$pi$stat[-classParentOfChild,], paramParent$pi$stat[classParentOfChild, 1] * paramChild$pi$stat), paramStr = ""))

  return(param)
}

# models with 2 parameters: for gaussian, weibull, negative binomial
concatenateParamNumeric2Param <- function(paramParent, paramChild, classParentOfChild)
{
  param <- rbind(paramParent$NumericalParam$stat[-((2*classParentOfChild) - 1:0),], paramChild$NumericalParam$stat)
  out <- list(NumericalParam = list(stat = param, paramStr = paramParent$NumericalParam$paramStr))

  return(out)
}

concatenateParamPoisson <- function(paramParent, paramChild, classParentOfChild)
{
  param <- rbind(paramParent$NumericalParam$stat[-classParentOfChild,], paramChild$NumericalParam$stat)
  out <- list(NumericalParam = list(stat = param, paramStr = paramParent$NumericalParam$paramStr))

  return(out)
}

concatenateParamCategorical <- function(paramParent, paramChild, classParentOfChild)
{
  nbModality <- as.numeric(gsub("nModality: ", "", paramParent$NumericalParam$paramStr))
  param <- rbind(paramParent$NumericalParam$stat[-(classParentOfChild*nbModality-0:(nbModality-1)),], paramChild$NumericalParam$stat)
  out <- list(NumericalParam = list(stat = param, paramStr = paramParent$NumericalParam$paramStr))

  return(out)
}

concatenateParamFunctional <- function(paramParent, paramChild, classParentOfChild)
{
  out <- list()
  for(name in c("alpha", "beta", "sd"))
  {
    classParent <- as.numeric(gsub("k: ", "", gsub(",.*$", "", rownames(paramParent[[name]]$stat)))) + 1

    param <- rbind(paramParent[[name]]$stat[classParent != classParentOfChild,], paramChild[[name]]$stat)
    out[[name]] = list(stat = param, paramStr = paramParent[[name]]$paramStr)
  }

  return(out)
}


concatenateParamRank <- function(paramParent, paramChild, classParentOfChild)
{
  param <- c(paramParent$mu$stat[-classParentOfChild], paramChild$mu$stat)
  out <- list(mu = list(stat = param, paramStr = paramParent$mu$paramStr))

  return(out)
}
