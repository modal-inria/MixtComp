# Compute the new model with the given parent and child
#
# Compute the parameter of the model (in supervised) with the clusters of child replacing one cluster of the parent
#
# @param resParent result object of a MixtComp run.
# @param resChild result object of a MixtComp run. This run is performed on the individuals of one cluster of resParent
# @param classParentOfChild cluster of the parent used for the child
# @param dataList output of getData
# @param mcStrategy
#
# @return mixtComp res
computeNewModel <- function(resParent, resChild, classParentOfChild, dataList, mcStrategy)
{
  oldZ_class = resParent$variable$data$z_class$completed
  newZ_class = rep(NA, length(oldZ_class))

  newZ_class[oldZ_class == classParentOfChild] = resChild$variable$data$z_class$completed + (resParent$mixture$nbCluster - 1)
  newZ_class[oldZ_class != classParentOfChild] <- refactorCategoricalData(oldZ_class[oldZ_class != classParentOfChild],
                                                                          unique(oldZ_class[oldZ_class != classParentOfChild]), seq_along(unique(oldZ_class[oldZ_class != classParentOfChild])))

  z_classPresent <- any(sapply(dataList, function(x){x$id}) == "z_class")
  if(z_classPresent)
  {
    dataList[[which(sapply(dataList, function(x){x$id}) == "z_class")]]$data = newZ_class
  }else{
    dataList[[length(dataList) + 1]]$data = list(data = newZ_class, id = "z_class", model = "LatentClass", paramStr = "")
  }

  res <- RMixtComp::mixtCompCluster(dataList,  mcStrategy, nbClass = resParent$mixture$nbCluster + 1, confidenceLevel = 0.95)

  return(res)
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
    param[[name]] = concatenateParamOneModel(resParent$variable$param[[name]], resChild$variable$param[[name]], classParentOfChild, resParent$variable$type[[name]])

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
  param <- list(pi = list(stat = rbind(paramParent$pi$stat[-classParentOfChild, 1, drop = FALSE], paramParent$pi$stat[classParentOfChild, 1] * paramChild$pi$stat[, 1, drop = FALSE]), paramStr = ""))

  return(param)
}

# models with 2 parameters: for gaussian, weibull, negative binomial
concatenateParamNumeric2Param <- function(paramParent, paramChild, classParentOfChild)
{
  param <- rbind(paramParent$NumericalParam$stat[-((2*classParentOfChild) - 1:0), 1, drop = FALSE], paramChild$NumericalParam$stat[, 1, drop = FALSE])
  out <- list(NumericalParam = list(stat = param, paramStr = paramParent$NumericalParam$paramStr))

  return(out)
}

concatenateParamPoisson <- function(paramParent, paramChild, classParentOfChild)
{
  param <- rbind(paramParent$NumericalParam$stat[-classParentOfChild, 1, drop = FALSE], paramChild$NumericalParam$stat[, 1, drop = FALSE])
  out <- list(NumericalParam = list(stat = param, paramStr = paramParent$NumericalParam$paramStr))

  return(out)
}

concatenateParamCategorical <- function(paramParent, paramChild, classParentOfChild)
{
  nbModality <- as.numeric(gsub("nModality: ", "", paramParent$NumericalParam$paramStr))
  param <- rbind(paramParent$NumericalParam$stat[-(classParentOfChild*nbModality-0:(nbModality-1)), 1, drop = FALSE], paramChild$NumericalParam$stat[, 1, drop = FALSE])
  out <- list(NumericalParam = list(stat = param, paramStr = paramParent$NumericalParam$paramStr))

  return(out)
}

concatenateParamFunctional <- function(paramParent, paramChild, classParentOfChild)
{
  out <- list()
  for(name in c("alpha", "beta", "sd"))
  {
    classParent <- as.numeric(gsub("k: ", "", gsub(",.*$", "", rownames(paramParent[[name]]$stat)))) + 1

    param <- rbind(paramParent[[name]]$stat[classParent != classParentOfChild, 1, drop = FALSE], paramChild[[name]]$stat[, 1, drop = FALSE])
    out[[name]] = list(stat = param, paramStr = paramParent[[name]]$paramStr)
  }

  return(out)
}


concatenateParamRank <- function(paramParent, paramChild, classParentOfChild)
{
  param <- c(paramParent$mu$stat[-classParentOfChild], paramChild$mu$stat)
  out <- list(mu = list(stat = param, paramStr = paramParent$mu$paramStr),
              pi = list(stat =  rbind(paramParent$pi$stat[-classParentOfChild, 1, drop = FALSE], paramChild$pi$stat[, 1, drop = FALSE]),  paramStr = paramParent$pi$paramStr))

  return(out)
}
