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


#' @title Create algo object
#'
#' @description create an algo object required by \code{mixtCompLearn} and \code{mixtCompPredict} from \code{RMixtComp}.
#'
#' @param nbBurnInIter Number of iterations of the burn-in part of the SEM algorithm.
#' @param nbIter Number of iterations of the SEM algorithm.
#' @param nbGibbsBurnInIter Number of iterations of the burn-in part of the Gibbs algorithm.
#' @param nbGibbsIter Number of iterations of the Gibbs algorithm.
#' @param nInitPerClass Number of individuals used to initialize each cluster.
#' @param nSemTry Number of try of the algorithm for avoiding an error.
#' @param confidenceLevel confidence level for confidence bounds for parameter estimation
#' @param ratioStableCriterion stability partition required to stop earlier the SEM
#' @param nStableCriterion number of iterations of partition stability to stop earlier the SEM
#'
#' @return a list with the parameters values
#'
#' @examples
#' # default values
#' algo <- createAlgo()
#'
#' # change some values
#' algo <- createAlgo(nbIter = 200)
#'
#' @author Quentin Grimonprez
#' @export
createAlgo <- function(nbBurnInIter = 50, nbIter = 50, nbGibbsBurnInIter = 50, nbGibbsIter = 50,
                       nInitPerClass = 50, nSemTry = 20, confidenceLevel = 0.95,
                       ratioStableCriterion = 0.99, nStableCriterion = 20) {
  list(
    nbBurnInIter = nbBurnInIter,
    nbIter = nbIter,
    nbGibbsBurnInIter = nbGibbsBurnInIter,
    nbGibbsIter = nbGibbsIter,
    nInitPerClass = nInitPerClass,
    nSemTry = nSemTry,
    confidenceLevel = confidenceLevel,
    ratioStableCriterion = ratioStableCriterion,
    nStableCriterion = nStableCriterion
  )
}


#' @name computeDiscrimPowerVar
#'
#' @title Discriminative power
#'
#' @description Compute the discriminative power of each variable or class
#'
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \code{mixtCompLearn} or \code{mixtCompPredict} functions from \code{RMixtComp} package or \code{rmcMultiRun} from \code{RMixtCompIO} package.
#' @param class NULL or a number of classes. If NULL, return the discriminative power of variables globally otherwise return the discriminative power of variables in the given class
#'
#' @return the discriminative power
#'
#' @details
#' The discriminative power of variable j is defined by 1 - C(j)
#' \deqn{C(j)=  - \sum_{k=1}^K sum_{i=1}^n P(Z_i=k|x_{ij}) \log(P(Z_i=k|x_{ij})) / (n*\log(K))}
#'
#' A high value (close to one) means that the variable is highly discriminating. A low value (close to zero) means that the variable is poorly discriminating.
#'
#' The discriminative power of variable j in class k is defined by 1 - C(j)
#' \deqn{C(j)=  - sum_{i=1}^n (P(Z_i!=k|x_{ij}) \log(P(Z_i!=k|x_{ij})) + P(Z_i=k|x_{ij}) \log(P(Z_i=k|x_{ij}))) / (n*\log(2))}
#'
#'
#' The discriminative power of class k is defined by 1 - D(k)
#' \deqn{D(k) =  -\sum_{i=1}^n P(Z_i=k|x_i) \log(P(Z_i=k|x_i)) / (n*\exp(-1))}
#'
#'
#' @examples
#' require(RMixtCompIO) # for learning a mixture model
#' dataLearn <- list(
#'   var1 = as.character(c(rnorm(50, -2, 0.8), rnorm(50, 2, 0.8))),
#'   var2 = as.character(c(rnorm(50, 2), rpois(50, 8)))
#' )
#'
#' model <- list(
#'   var1 = list(type = "Gaussian", paramStr = ""),
#'   var2 = list(type = "Poisson", paramStr = "")
#' )
#'
#' algo <- list(
#'   nClass = 2,
#'   nInd = 100,
#'   nbBurnInIter = 100,
#'   nbIter = 100,
#'   nbGibbsBurnInIter = 100,
#'   nbGibbsIter = 100,
#'   nInitPerClass = 3,
#'   nSemTry = 20,
#'   confidenceLevel = 0.95,
#'   ratioStableCriterion = 0.95,
#'   nStableCriterion = 10,
#'   mode = "learn"
#' )
#'
#' resLearn <- rmcMultiRun(algo, dataLearn, model, nRun = 3)
#'
#'
#' discVar <- computeDiscrimPowerVar(resLearn)
#' discVarInClass1 <- computeDiscrimPowerVar(resLearn, class = 1)
#' discClass <- computeDiscrimPowerClass(resLearn)
#'
#' # graphic representation of discriminant variables
#' plotDiscrimVar(resLearn)
#' # graphic representation of discriminant classes
#' plotDiscrimClass(resLearn)
#'
#' @author Matthieu Marbac
#' @seealso \code{\link{plotDiscrimClass}} \code{\link{plotDiscrimVar}}
#' @export
computeDiscrimPowerVar <- function(outMixtComp, class = NULL) {
  if (is.null(class)) {
    return(1 - colSums(outMixtComp$mixture$IDClass))
  } else {
    return(computeDiscrimPowerVarPerClass(outMixtComp, class))
  }
}



# @title Discriminative power of variables in a class
#
# @description Compute the discriminative power of each variable for a given class
#
# @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \code{mixtCompLearn} or \code{mixtCompPredict} functions from \code{RMixtComp} package or \code{rmcMultiRun} from \code{RMixtCompIO} package.
# @param class class for which discriminating power is required
#
# The discriminative power of variable j in class k is defined by 1 - C(j)
# \deqn{C(j)=  - sum_{i=1}^n P(Z_i!=k|x_{ij}) \log(P(Z_i!=k|x_{ij})) / (n*\log(2))}
#
# @author Quentin Grimonprez
computeDiscrimPowerVarPerClass <- function(outMixtComp, class) {
  if (length(class) > 1) {
    stop("class must be NULL or an integer between 1 and the number of classes.")
  }
  if (!(class %in% seq_len(nrow(outMixtComp$mixture$IDClass)))) {
    stop("class must be NULL or an integer between 1 and the number of classes.")
  }

  K <- nrow(outMixtComp$mixture$IDClass)

  entropyKJ <- outMixtComp$mixture$IDClass[class, ] * log(K)
  entropyKJbar <- outMixtComp$mixture$IDClassBar[class, ] * log(K)

  1 - (entropyKJ + entropyKJbar) / log(2)
}

#' @rdname computeDiscrimPowerVar
#' @export
computeDiscrimPowerClass <- function(outMixtComp) {
  return(1 - (-colMeans(log(outMixtComp$variable$data$z_class$stat**outMixtComp$variable$data$z_class$stat)) / exp(-1)))
}


#' @name computeSimilarityVar
#'
#' @title Similarity
#'
#' @description Compute the similarity between variables (or classes)
#'
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \code{mixtCompLearn} or \code{mixtCompPredict} functions from \code{RMixtComp} package or \code{rmcMultiRun} from \code{RMixtCompIO} package.
#'
#' @return a similarity matrix
#'
#' @details
#' The similarities between variables j and h is defined by Delta(j,h)
#' \deqn{Delta(j,h)^2 = 1 - \sqrt{(1/n) * \sum_{i=1}^n \sum_{k=1}^K (P(Z_i=k|x_{ij}) - P(Z_i=k|x_{ih}))^2}}
#'
#' The similarities between classes k and g is defined by 1 - Sigma(k,g)
#' \deqn{Sigma(k,g)^2 = (1/n) * \sum_{i=1}^n (P(Z_i=k|x_i) - P(Z_i=g|x_i))^2}
#'
#' @examples
#' require(RMixtCompIO) # for learning a mixture model
#' dataLearn <- list(
#'   var1 = as.character(c(rnorm(50, -2, 0.8), rnorm(50, 2, 0.8))),
#'   var2 = as.character(c(rnorm(50, 2), rpois(50, 8)))
#' )
#'
#' model <- list(
#'   var1 = list(type = "Gaussian", paramStr = ""),
#'   var2 = list(type = "Poisson", paramStr = "")
#' )
#'
#' algo <- list(
#'   nClass = 2,
#'   nInd = 100,
#'   nbBurnInIter = 100,
#'   nbIter = 100,
#'   nbGibbsBurnInIter = 100,
#'   nbGibbsIter = 100,
#'   nInitPerClass = 3,
#'   nSemTry = 20,
#'   confidenceLevel = 0.95,
#'   ratioStableCriterion = 0.95,
#'   nStableCriterion = 10,
#'   mode = "learn"
#' )
#'
#' resLearn <- rmcMultiRun(algo, dataLearn, model, nRun = 3)
#'
#' simVar <- computeSimilarityVar(resLearn)
#' simClass <- computeSimilarityClass(resLearn)
#'
#' @author Quentin Grimonprez
#' @seealso \code{\link{heatmapVar}} \code{\link{heatmapClass}}
#' @export
computeSimilarityVar <- function(outMixtComp) {
  simil <- outMixtComp$mixture$delta

  return(simil)
}

#' @rdname computeSimilarityVar
#' @export
computeSimilarityClass <- function(outMixtComp) {
  simil <- 1 - sqrt(sapply(
    seq_len(outMixtComp$algo$nClass),
    function(k) colMeans(sweep(outMixtComp$variable$data$z_class$stat, 1, outMixtComp$variable$data$z_class$stat[, k], "-")**2)
  ))

  if (is.null(outMixtComp$algo$dictionary$z_class)) {
    colnames(simil) <- rownames(simil) <- paste("Class", seq_len(outMixtComp$algo$nClass))
  } else {
    colnames(simil) <- rownames(simil) <- outMixtComp$algo$dictionary$z_class$old
  }

  return(simil)
}


#' Available models
#'
#' Get information about models implemented in MixtComp
#'
#' @return a data.frame containing models implemented in MixtComp
#' \describe{
#' \item{model}{model name}
#' \item{data.type}{data type}
#' \item{format}{Special format required for individuals}
#' \item{missing.formats}{accepted formats (separated by a ;) for missing values}
#' \item{hyperparameter}{Required hyperparameters in the paramStr elements of model object}
#' \item{comments}{comments about the model}
#' \item{reference}{link to article}
#' }
#'
#' @seealso \code{mixtCompLearn}
#' @author Quentin Grimonprez
#'
#' @examples
#' availableModels()
#'
#' @export
availableModels <- function() {
  data.frame(
    "model" = c(
      "Gaussian", "Weibull", "Multinomial", "Poisson", "NegativeBinomial", "Rank_ISR", "Func_CS", "Func_SharedAlpha_CS"
    ),
    "data type" = c(
      "Real", "Positive real", "Categorical", "Positive integer", "Positive integer", "Rank", "Functional", "Functional"
    ),
    "format" = c(
      "",
      "",
      "Numbers from 1 to number of modalities",
      "",
      "",
      "o1,o2,..,oN",
      "time1:value1,time2:value2,...,timeN:valueN", "time1:value1,time2:value2,...,timeN:valueN"
    ),
    "missing formats" = c(
      "?; [a:b]; [-inf:b]; [a:inf]",
      "?; [a:b]; [a:+inf]",
      "?; {a,..,b}",
      "?; [a:b]; [a:+inf]",
      "?; [a:b]; [a:+inf]",
      "?,?,?,?; 4,3,?,?; {2 3}, 4,{2 3}, 1",
      "",
      ""
    ),
    "hyperparameter" = c("", "", "", "", "", "", "nSub:a, nCoeff:b", "nSub:a, nCoeff:b"),
    "comments" = c(
      "",
      "a,b>=0",
      "",
      "a,b>=0",
      "a,b>=0",
      "o1,o2,..,oN: o1 is an integer corresponding to the object's number ranked in 1st position",
      paste(
        "nSub = number of subregressions. nCoeff = number of coefficients of each polynomial regression.",
        "Between individuals, functionals can have different time values and length."
      ),
      "Func_CS with parameter alpha shared between classes: subregressions start and finish at the same time between clusters."
    ),
    "reference" = c(
      "", "", "", "", "",
      "https://hal.inria.fr/hal-00743384",
      "https://arxiv.org/abs/1312.6967",
      "https://arxiv.org/abs/1312.6967"
    )
  )
}




# delete log from mixtcomp output and completed data in order to gain disk space
#
# @param outMixtComp mixtComp or mixtCompLearn object
# @param completed if TRUE delete completed data
# @param log if TRUE delete logs of parameters estimation
#
# @return outMixtComp without some elements
#
# @details
# Some functions such as plotConvergence or plotDataCI will not work on a reduced output
reduceRMixtCompOutput <- function(outMixtComp, completed = TRUE, log = TRUE) {
  outMixtComp <- reduceRMixtCompOutputIntern(outMixtComp, completed, log)

  if ("MixtCompLearn" %in% class(outMixtComp)) {
    for (i in outMixtComp$nClass) {
      outMixtComp$res[[i]] <- reduceRMixtCompOutputIntern(outMixtComp$res[[i]], completed, log)
    }
  }


  return(outMixtComp)
}

reduceRMixtCompOutputIntern <- function(outMixtComp, completed = TRUE, log = TRUE) {
  for (nomVar in names(outMixtComp$variable$param)) {
    if ((nomVar != "z_class") && completed) {
      outMixtComp$variable$data[[nomVar]]$completed <- NULL
      outMixtComp$variable$data[[nomVar]]$data <- NULL
      outMixtComp$variable$data[[nomVar]]$time <- NULL
    }

    if (log) {
      if (outMixtComp$variable$type[[nomVar]] %in% c(
        "LatentClass", "Gaussian", "Multinomial", "Poisson", "Weibull", "NegativeBinomial")) {
        outMixtComp$variable$param[[nomVar]]$log <- NULL
      }

      if (outMixtComp$variable$type[[nomVar]] %in% "Rank_ISR") {
        outMixtComp$variable$param[[nomVar]]$mu$log <- NULL
        outMixtComp$variable$param[[nomVar]]$pi$log <- NULL
      }

      if (outMixtComp$variable$type[[nomVar]] %in% c("Func_CS", "Func_SharedAlpha_CS")) {
        outMixtComp$variable$param[[nomVar]]$alpha$log <- NULL
        outMixtComp$variable$param[[nomVar]]$beta$log <- NULL
        outMixtComp$variable$param[[nomVar]]$sd$log <- NULL
      }
    }
  }

  return(outMixtComp)
}


# Compute ICL
#
# compute ICL using formula:
# ICL = loglikObs - nbFreeParameters/2*log(n) - entropy
#
# @param outMixtComp mixtComp or mixtCompLearn object
#
# @return icl
#
# @details
# compute ICL using formula:
# ICL = loglikObs - nbFreeParameters/2*log(n) - entropy
#
# ICL computed in MixtComp uses the formula:
# ICL = loglikComp - nbFreeParameters/2*log(n)
#
# loglikComp can be not well estimated in some case (functional)
icl <- function(outMixtComp) {
  loglikObs <- outMixtComp$mixture$lnObservedLikelihood
  nbFree <- outMixtComp$mixture$nbFreeParameters
  n <- outMixtComp$algo$nInd
  logTik <- getTik(outMixtComp)
  entropy <- -sum(exp(logTik) * logTik)

  loglikObs - nbFree / 2 * log(n) - entropy
}
