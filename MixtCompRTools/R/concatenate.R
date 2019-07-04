# MixtComp version 4.0  - july 2019
# Copyright (C) Inria - Université de Lille - CNRS

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
# @author Quentin Grimonprez
concatenateParam <- function(resParent, resChild, classParentOfChild)
{
  nbClassParent <- resParent$algo$nClass
  nbClassChild <- resChild$algo$nClass

  if((classParentOfChild <= 0) || (classParentOfChild > nbClassParent))
    stop("Wrong classParentOfChild value.")

  classParent <- resParent$variable$data$z_class$completed
  nbIndChild <- resChild$algo$nInd

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
# @author Quentin Grimonprez
concatenateParamOneModel <- function(paramParent, paramChild, classParentOfChild, model)
{
  model = ifelse(model == "Func_SharedAlpha_CS", "Func_CS", model)
  model = ifelse(model %in% c("Gaussian", "Weibull", "NegativeBinomial"), "2NumParam", model)

  out <- switch(model,
                "LatentClass" = concatenateParamZclass(paramParent, paramChild, classParentOfChild),
                "2NumParam" = concatenateParamNumeric2Param(paramParent, paramChild, classParentOfChild),
                "Multinomial" = concatenateParamPoisson(paramParent, paramChild, classParentOfChild),
                "Func_CS" = concatenateParamFunctional(paramParent, paramChild, classParentOfChild),
                "Poisson" = concatenateParamPoisson(paramParent, paramChild, classParentOfChild),
                "Rank_ISR" = concatenateParamRank(paramParent, paramChild, classParentOfChild))

  return(out)
}

# @author Quentin Grimonprez
concatenateParamZclass <- function(paramParent, paramChild, classParentOfChild)
{
  param <- list(stat = rbind(paramParent$stat[-classParentOfChild, 1, drop = FALSE], paramParent$stat[classParentOfChild, 1] * paramChild$stat[, 1, drop = FALSE]), paramStr = "")

  return(param)
}

# models with 2 parameters: for gaussian, weibull, negative binomial
# @author Quentin Grimonprez
concatenateParamNumeric2Param <- function(paramParent, paramChild, classParentOfChild)
{
  param <- rbind(paramParent$stat[-((2*classParentOfChild) - 1:0), 1, drop = FALSE], paramChild$stat[, 1, drop = FALSE])
  out <- list(stat = param, paramStr = paramParent$paramStr)

  return(out)
}

# @author Quentin Grimonprez
concatenateParamPoisson <- function(paramParent, paramChild, classParentOfChild)
{
  param <- rbind(paramParent$stat[-classParentOfChild, 1, drop = FALSE], paramChild$stat[, 1, drop = FALSE])
  out <- list(stat = param, paramStr = paramParent$paramStr)

  return(out)
}

# @author Quentin Grimonprez
concatenateParamCategorical <- function(paramParent, paramChild, classParentOfChild)
{
  nbModality <- as.numeric(gsub("nModality: ", "", paramParent$paramStr))
  param <- rbind(paramParent$stat[-(classParentOfChild*nbModality-0:(nbModality-1)), 1, drop = FALSE], paramChild$stat[, 1, drop = FALSE])
  out <- list(stat = param, paramStr = paramParent$paramStr)

  return(out)
}

# @author Quentin Grimonprez
concatenateParamFunctional <- function(paramParent, paramChild, classParentOfChild)
{
  out <- list()
  for(name in c("alpha", "beta", "sd"))
  {
    classParent <- as.numeric(gsub("k: ", "", gsub(",.*$", "", rownames(paramParent[[name]]$stat)))) + 1

    param <- rbind(paramParent[[name]]$stat[classParent != classParentOfChild, 1, drop = FALSE], paramChild[[name]]$stat[, 1, drop = FALSE])
    out[[name]] = list(stat = param)
  }
  out$paramStr = paramParent$paramStr

  return(out)
}

# @author Quentin Grimonprez
concatenateParamRank <- function(paramParent, paramChild, classParentOfChild)
{
  param <- c(paramParent$mu$stat[-classParentOfChild], paramChild$mu$stat)
  out <- list(mu = list(stat = param),
              pi = list(stat = rbind(paramParent$pi$stat[-classParentOfChild, 1, drop = FALSE], paramChild$pi$stat[, 1, drop = FALSE])),
              paramStr = paramParent$paramStr)

  return(out)
}
