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


#' Convergence of algorithm
#'
#' Plot the evolution of the completed loglikelihood during the SEM algorithm. The vertical line denotes the end of the
#' burn-in phase.
#'
#' @param output object returned by \emph{mixtCompLearn} function from \emph{RMixtComp} or \emph{rmcMultiRun} function
#' from \emph{RMixtCompIO}
#' @param ... graphical parameters
#'
#' @details
#' This function can be used to check the convergence and choose the parameters nbBurnInIter and nbIter from mcStrategy.
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
#' # plot
#' plotConvergence(resLearn)
#'
#' @author Quentin Grimonprez
#' @family plot
#' @export
plotConvergence <- function(output, ...) {
  plot(c(output$mixture$completedProbabilityLogBurnIn, output$mixture$completedProbabilityLogRun),
    xlab = "Iteration",
    ylab = "Completed loglikelihood", type = "l", main = "Completed loglikelihood during and\n after the burn-in period", ...
  )
  abline(v = length(output$mixture$completedProbabilityLogBurnIn) + 0.5, lty = "dotted", col = "red")
}

#' Evolution of parameters
#'
#' Plot the evolution of estimated parameters after the burn-in phase.
#'
#' @param output object returned by \emph{mixtCompLearn} function from \emph{RMixtComp} or \emph{rmcMultiRun} function
#' from \emph{RMixtCompIO}
#' @param var name of the variable
#' @param ... graphical parameters
#'
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
#' # plot
#' plotParamConvergence(resLearn, "var1")
#' plotParamConvergence(resLearn, "var2")
#'
#' @author Quentin Grimonprez
#' @family plot
#' @export
plotParamConvergence <- function(output, var, ...) {
  if (!(var %in% names(output$variable$type))) {
    stop("This variable does not exist in the mixture model.")
  }

  type <- output$variable$type[[var]]
  nbCluster <- output$algo$nClass

  type <- ifelse(
    type %in% c("Gaussian", "Weibull", "Multinomial", "Poisson", "NegativeBinomial", "LatentClass"),
    "Simple",
    type
  )
  type <- ifelse(type %in% c("Func_SharedAlpha_CS"), "Func_CS", type)

  switch(type,
    "Simple" = plotLog(output$variable$param[[var]]$log, nbCluster, var, ...),
    "Rank_ISR" = plotLog(output$variable$param[[var]]$pi$log, nbCluster, var, ...),
    "Func_CS" = {
      plotLog(output$variable$param[[var]]$alpha$log, nbCluster, var, ...)
      title("alpha", line = -1, outer = TRUE)
      plotLog(output$variable$param[[var]]$beta$log, nbCluster, var, ...)
      title("beta", line = -1, outer = TRUE)
      plotLog(output$variable$param[[var]]$sd$log, nbCluster, var, ...)
      title("sd", line = -1, outer = TRUE)
    },
    warning(paste0("Not (yet) available for model ", type))
  )
}


# @author Quentin Grimonprez
plotLog <- function(paramLog, nbCluster, var, ...) {
  nrowParam <- nrow(paramLog)
  nbParamPerClass <- nrowParam / nbCluster

  oldPar <- par(mfrow = n2mfrow(nbParamPerClass))
  on.exit(par(oldPar))
  for (i in seq_len(nbParamPerClass)) {
    ind <- nbParamPerClass * (seq_len(nbCluster)) + (i - nbParamPerClass)
    name <- rownames(paramLog)[ind[1]]
    name <- gsub("^[^,]*, ", "", name)

    matplot(t(paramLog[ind, , drop = FALSE]), type = "l", main = paste0(var, ": ", name), xlab = "Iteration", ylab = name)
    legend("left", paste0("class ", seq_len(nbCluster)), lty = 1:5, col = 1:6)
  }
}
