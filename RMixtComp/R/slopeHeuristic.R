# MixtComp version 4 - july 2019
# Copyright (C) Inria - Université de Lille - CNRS

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>


#' Slope heuristic
#'
#' Criterion to choose the number of clusters
#'
#' @param object output of \code{\link{mixtCompLearn}}
#' @param K0 number of class for computing the constant value (see details)
#' 
#' @return the values of the slope heuristic
#' 
#' @details 
#' The slope heuristic criterion is: LL_k - 2 C * D_k, with LL_k the loglikelihood for k classes, D_k the number of free parameters for k classes, 
#' C is the slope of the linear regression between D_k and LL_k for (k> K0)
#'  
#' @references 
#' Cathy Maugis, Bertrand Michel.  Slope heuristics for variable selection and clustering via Gaussian mixtures. [Research Report] RR-6550, INRIA. 2008. inria-00284620v2
#' Jean-Patrick Baudry, Cathy Maugis, Bertrand Michel. Slope Heuristics: Overview and Implementation. 2010. hal-00461639
#'
#' @examples 
#' \donttest{
#' data(titanic)
#' 
#' ## Use the MixtComp format
#' dat <- titanic
#' 
#' # refactor categorical data: survived, sex, embarked and pclass
#' dat$sex <- refactorCategorical(dat$sex, c("male", "female", NA), c(1, 2, "?"))
#' dat$embarked <- refactorCategorical(dat$embarked, c("C", "Q", "S", NA), c(1, 2, 3, "?"))
#' dat$survived <- refactorCategorical(dat$survived, c(0, 1, NA), c(1, 2, "?"))
#' dat$pclass <- refactorCategorical(dat$pclass, c("1st", "2nd", "3rd"), c(1, 2, 3))
#' 
#' # replace all NA by ?
#' dat[is.na(dat)] = "?" 
#' 
#' # create model
#' model <- list(pclass = "Multinomial",
#'               survived = "Multinomial",
#'               sex = "Multinomial",
#'               age = "Gaussian",
#'               sibsp = "Poisson",
#'               parch = "Poisson",
#'               fare = "Gaussian",
#'               embarked = "Multinomial")
#' 
#' # create algo
#' algo <- createAlgo()
#' 
#' # run clustering
#' resLearn <- mixtCompLearn(dat, model, algo, nClass = 2:25, criterion = "ICL", nRun = 3)
#' 
#' out <- slopeHeuristic(resLearn, K0 = 6)
#' 
#' }
#'   
#' @author Quentin Grimonprez
#'
#' @export
slopeHeuristic <- function(object, K0 = floor(max(object$nClass) * 0.4))
{
  if(!("MixtCompLearn" %in% class(object)))
    stop("object must be a MixtCompLearn object.")
  
  
  # the linear regression is made for element in K >= K0
  # index of K0
  indD0 <- which(object$nClass >= K0)[1]
  
  logLikeObs <- sapply(object$res, function(x) ifelse(!is.null(x$mixture$lnObservedLikelihood), x$mixture$lnObservedLikelihood, NA))
  nFreeParam <- sapply(object$res, function(x) ifelse(!is.null(x$mixture$nbFreeParameters), x$mixture$nbFreeParameters, NA))

  C <- computeCSlopeHeuristic(logLikeObs, nFreeParam, indD0)
  
  crit <- logLikeObs - 2 * C * nFreeParam 
  names(crit) = object$nClass
  
  return(crit)
}



# compute constant value of the slpe heuristic
#
# slope coefficients of the regression between logLikeObs and nFreeParam
computeCSlopeHeuristic <- function(logLikeObs, nFreeParam, indD0)
{
  logLikeObsTemp <- logLikeObs[indD0:length(logLikeObs)]
  nFreeParamTemp <- nFreeParam[indD0:length(logLikeObs)]
  
  toKeep <- !is.na(logLikeObsTemp) & !is.infinite(logLikeObsTemp)
  logLikeObsTemp = logLikeObsTemp[toKeep]
  nFreeParamTemp = nFreeParamTemp[toKeep]
  
  # slope of linear regression between logLikeObs and nFreeParam
  C <- cov(logLikeObsTemp, nFreeParamTemp)/var(nFreeParamTemp)
  
  return(C)
}