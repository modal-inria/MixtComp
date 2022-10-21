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


#' @title MixtComp Object Summaries
#'
#' @description Summary of a \emph{MixtComp} object
#'
#' @param object \emph{MixtComp} object
#' @param ... Not used.
#'
#'
#' @method summary MixtComp
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
#' summary(resLearn)
#'
#' @seealso \code{mixtCompLearn} \code{\link{print.MixtComp}}
#' @author Quentin Grimonprez
#' @export
summary.MixtComp <- function(object, ...) {
  cat("########### MixtComp Run ###########\n")
  if (!is.null(object$warnLog)) {
    cat("Run failed:\n")
    cat(object$warnLog)
  } else {
    discVar <- sort(computeDiscrimPowerVar(object), decreasing = TRUE)
    nameDiscVar <- names(discVar)
    cat("Number of individuals:", object$algo$nInd, "\n")
    cat("Number of variables:", length(object$variable$type) - 1, "\n")
    cat("Number of clusters:", object$algo$nClass, "\n")
    cat("Mode:", object$algo$mode, "\n")
    cat("Time:", object$mixture$runTime$total, "s\n")
    cat(
      "SEM burn-in iterations done:",
      paste0(length(object$mixture$completedProbabilityLogBurnIn), "/", object$algo$nbBurnInIter),
      "\n"
    )
    cat("SEM run iterations done:", paste0(length(object$mixture$completedProbabilityLogRun), "/", object$algo$nbIter), "\n")
    cat("Observed log-likelihood:", object$mixture$lnObservedLikelihood, "\n")
    cat("BIC:", object$mixture$BIC, "\n")
    cat("ICL:", object$mixture$ICL, "\n")
    cat("Discriminative power:\n")
    print(round(discVar, 3))
    cat("Proportions of the mixture:\n")
    cat(round(getParam(object, "z_class"), 3), "\n")
    cat("Parameters of the most discriminant variables:\n")
    for (name in nameDiscVar[seq_len(min(3, length(nameDiscVar)))]) {
      cat(paste0("- ", name, ":"), object$variable$type[[name]], "\n")
      param <- getParam(object, name)
      if (is.list(param)) {
        print(lapply(param, round, 3))
      } else {
        print(round(param, 3))
      }
    }
  }
  cat("####################################\n")
}


#' @title Print Values
#'
#' @description Print a \emph{MixtComp} object
#'
#' @param x \emph{MixtComp} object
#' @param nVarMaxToPrint number of variables to display (including z_class)
#' @param ... parameter of \code{head} function
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
#' print(resLearn)
#'
#' @method print MixtComp
#'
#' @seealso \code{mixtCompLearn} \code{mixtCompPredict}
#' @author Quentin Grimonprez
#' @export
print.MixtComp <- function(x, nVarMaxToPrint = 5, ...) {
  if (!is.null(x$warnLog)) {
    cat("$warnLog:\n")
    print(x$warnLog)
  } else {
    nVar <- length(x$variable$type)
    nVarToPrint <- max(1, min(nVarMaxToPrint, nVar))

    discVar <- sort(computeDiscrimPowerVar(x), decreasing = TRUE)
    nameDiscVar <- names(discVar)

    nameVar <- c("z_class", nameDiscVar)

    cat("$algo\n")
    algoToPrint <- c("nInd", "nClass", "mode")
    for (i in algoToPrint) {
      cat(paste0("$algo$", i, ":"), x$algo[[i]], "\n")
    }
    cat("\t Other outputs:", setdiff(names(x$algo), algoToPrint), "\n")
    cat("\n")

    cat("$mixture\n")
    mixtureToPrint <- c("lnObservedLikelihood", "nbFreeParameters", "BIC", "ICL")
    for (i in mixtureToPrint) {
      cat(paste0("$mixture$", i, ":"), x$mixture[[i]], "\n")
    }
    cat("\t Other outputs:", setdiff(names(x$mixture), mixtureToPrint), "\n")
    cat("\n")

    cat("$variable\n")
    cat("$variable$type\n")
    for (i in seq_len(nVarToPrint)) {
      cat(paste0("$variable$type$", nameVar[i]), ": ", x$variable$type[[nameVar[i]]], "\n")
    }
    if (nVarToPrint < nVar) {
      cat("\t Other variables:", nameVar[-seq_len(nVarToPrint)], "\n")
    }
    cat("\n")

    cat("$variable$data\n")
    cat("$variable$data$z_class$completed: ", head(x$variable$data$z_class$completed, ...), "...\n")
    cat("$variable$data$z_class$stat:\n ")
    print(head(x$variable$data$z_class$stat, ...))
    cat("\n")
    cat("\t Other variables:", nameVar[-1], "\n")
    cat("\n")

    cat("$variable$param\n")
    for (i in seq_len(nVarToPrint)) {
      cat(paste0("$variable$param$", nameVar[i], ": \n"))
      if ("stat" %in% names(x$variable$param[[nameVar[i]]])) {
        print(head(x$variable$param[[nameVar[i]]]$stat, ...))
      } else {
        for (name in names(x$variable$param[[nameVar[i]]])) {
          if (name != "paramStr") {
            cat(paste0("\t $", name, ":"), "\n")
            print(head(x$variable$param[[nameVar[i]]][[name]]$stat, ...))
          }
        }
      }
    }
    if (nVarToPrint < nVar) {
      cat("\t Other variables:", nameVar[-seq_len(nVarToPrint)], "\n")
    }
  }
}


#'
#' Plot of a \emph{MixtComp} object
#'
#' @param x \emph{MixtComp} object
#' @param nVarMaxToPlot number of variables to display
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param plotData "CI" or "Boxplot". If "CI", uses \link{plotDataCI} function. If "Boxplot", uses \link{plotDataBoxplot}
#' @param ... extra parameter for \link{plotDataCI}
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
#' plot(resLearn)
#'
#' @method plot MixtComp
#'
#' @seealso \code{mixtCompLearn} \code{mixtCompPredict}
#' @family plot
#' @author Quentin Grimonprez
#' @export
plot.MixtComp <- function(x, nVarMaxToPlot = 3, pkg = c("ggplot2", "plotly"), plotData = c("CI", "Boxplot"), ...) {
  pkg <- match.arg(pkg)
  plotData <- match.arg(plotData)

  plotDataFunction <- switch(plotData,
    "CI" = plotDataCI,
    "Boxplot" = plotDataBoxplot
  )

  p <- list()

  if (is.null(x$warnLog)) {
    nVar <- length(x$variable$type) - 1
    nVarToPrint <- max(1, min(nVarMaxToPlot, nVar))

    discVar <- sort(computeDiscrimPowerVar(x), decreasing = TRUE)
    nameDiscVar <- names(discVar)


    p$discrimPowerVar <- plotDiscrimVar(x, pkg = pkg)

    p$proportion <- plotProportion(x, pkg = pkg)

    for (i in seq_len(nVarToPrint)) {
      p[[nameDiscVar[i]]] <- plotDataFunction(x, nameDiscVar[i], pkg = pkg, ...)
    }
  }

  p
}
