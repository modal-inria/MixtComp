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


#' @title Get the estimated parameter
#'
#' @description Get the estimated parameter
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \code{mixtCompLearn} or
#' \code{mixtCompPredict} functions from \code{RMixtComp} package or \code{rmcMultiRun} from \code{RMixtCompIO} package.
#' @param var name of the variable to get parameter
#'
#' @return the parameter of the variable
#'
#' @examples
#' if (requireNamespace("RMixtCompIO", quietly = TRUE)) {
#'   dataLearn <- list(
#'     var1 = as.character(c(rnorm(50, -2, 0.8), rnorm(50, 2, 0.8))),
#'     var2 = as.character(c(rnorm(50, 2), rpois(50, 8)))
#'   )
#'
#'   model <- list(
#'     var1 = list(type = "Gaussian", paramStr = ""),
#'     var2 = list(type = "Poisson", paramStr = "")
#'   )
#'
#'   algo <- list(
#'     nClass = 2,
#'     nInd = 100,
#'     nbBurnInIter = 100,
#'     nbIter = 100,
#'     nbGibbsBurnInIter = 100,
#'     nbGibbsIter = 100,
#'     nInitPerClass = 3,
#'     nSemTry = 20,
#'     confidenceLevel = 0.95,
#'     ratioStableCriterion = 0.95,
#'     nStableCriterion = 10,
#'     mode = "learn"
#'   )
#'
#'   resLearn <- RMixtCompIO::rmcMultiRun(algo, dataLearn, model, nRun = 3)
#'
#'   # get estimated parameters for variable var1
#'   param <- getParam(resLearn, "var1")
#'   prop <- getProportion(resLearn)
#' }
#'
#' @seealso \code{\link{plotDataBoxplot}}  \code{\link{plotDataCI}}
#'
#' @author Quentin Grimonprez
#' @family getter
#' @export
getParam <- function(outMixtComp, var) {
  if (!(var %in% getVarNames(outMixtComp, with.z_class = TRUE))) {
    stop("Wrong variable names.")
  }

  type <- outMixtComp$variable$type[[var]]
  type <- ifelse(type == "Func_SharedAlpha_CS", "Func_CS", type)

  if (is.null(type)) {
    warning("Bad variable name.")
    return(c())
  }

  nbClass <- length(outMixtComp$variable$param$z_class$stat[, 1])

  param <- switch(type,
    "Multinomial" = getParamMultinomial(outMixtComp$variable$param[[var]], nbClass),
    "Gaussian" = getParamNumerical(outMixtComp$variable$param[[var]], nbClass, c("mean", "sd")),
    "Poisson" = getParamNumerical(outMixtComp$variable$param[[var]], nbClass, "lambda"),
    "NegativeBinomial" = getParamNumerical(outMixtComp$variable$param[[var]], nbClass, c("n", "p")),
    "Weibull" = getParamNumerical(outMixtComp$variable$param[[var]], nbClass, c("k (shape)", "lambda (scale)")),
    "Rank_ISR" = getParamRank_ISR(outMixtComp$variable$param[[var]], nbClass),
    "Func_CS" = getParamFunc_CS(outMixtComp$variable$param[[var]], nbClass),
    "LatentClass" = getParamNumerical(outMixtComp$variable$param[[var]], nbClass, "pi"),
    warning("Not yet implemented.")
  )

  return(param)
}


#' @rdname getParam
#' @export
getProportion <- function(outMixtComp) {
  drop(getParam(outMixtComp, "z_class"))
}

# @author Quentin Grimonprez
getParamNumerical <- function(param, nbClass, colNames) {
  className <- unique(sapply(strsplit(rownames(param$stat), split = ","), function(x) {
    gsub("k: ", "", x[1])
  }))
  matrix(param$stat[, 1], nrow = nbClass, byrow = TRUE, dimnames = list(paste0("k: ", className), colNames))
}

# @author Quentin Grimonprez
getParamFunc_CS <- function(param, nbClass) {
  nSub <- getnSub(param$paramStr)
  nCoeff <- getnCoeff(param$paramStr)

  list(
    alpha = getParamNumerical(
      param$alpha,
      nbClass,
      unlist(lapply(paste0("s:", seq_len(nSub)), function(x) paste0(x, c(", alpha0", ", alpha1"))))
    ),
    beta = getParamNumerical(
      param$beta,
      nbClass,
      unlist(lapply(paste0("s:", seq_len(nSub)), function(x) paste0(x, paste0(", c:", seq_len(nCoeff)))))
    ),
    sd = getParamNumerical(param$sd, nbClass, paste0("s:", seq_len(nSub)))
  )
}

# @author Quentin Grimonprez
getnCoeff <- function(paramStr) {
  as.numeric(gsub("nCoeff:", "", strsplit(gsub(" ", "", paramStr), ",")[[1]][2]))
}

# @author Quentin Grimonprez
getnSub <- function(paramStr) {
  as.numeric(gsub("nSub:", "", strsplit(gsub(" ", "", paramStr), ",")[[1]][1]))
}

# @author Quentin Grimonprez
getParamRank_ISR <- function(param, nbClass) {
  mu <- t(sapply(param$mu$stat, function(x) {
    x$rank
  }))
  rownames(mu) <- paste0("k: ", seq_len(nbClass))

  list(
    pi = getParamNumerical(param$pi, nbClass, "pi"),
    mu = mu
  )
}

# @author Quentin Grimonprez
getParamMultinomial <- function(param, nbClass) {
  nbModalities <- length(param$stat[, 1]) / nbClass

  modalities <- gsub("k: .*, modality: ", "", rownames(param$stat)[seq_len(nbModalities)])
  getParamNumerical(param, nbClass, paste0("modality ", modalities))
}
