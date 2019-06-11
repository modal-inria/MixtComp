#' @title Get the estimated parameter
#'
#' @description Get the estimated parameter
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \link{mixtCompLearn} or \link{mixtCompPredict} functions.
#' @param var name of the variable to get parameter
#'
#' @return the parameter of the variable
#'
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- list(nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # run RMixtCompt for clustering
#' resLearn <- mixtCompLearn(simData$dataLearn$matrix, simData$model$unsupervised, algo, nClass = 2)
#' 
#' # get param
#' param <- getParam(resLearn, "Gaussian1")
#' }
#' 
#' @seealso \code{\link{plotDataBoxplot}}  \code{\link{plotDataCI}}
#' 
#' @author Quentin Grimonprez
#' @family getter
#' @export
getParam <- function(outMixtComp, var)
{
  if(!(var %in% getVarNames(outMixtComp, with.z_class = TRUE)))
    stop("Wrong variable names.")
  
  type <- outMixtComp$variable$type[[var]]
  type = ifelse(type == "Func_SharedAlpha_CS", "Func_CS", type)
  
  if(is.null(type))
  {
    warning("Bad variable name.")
    return(c())
  }
  
  nbClass <- length(outMixtComp$variable$param$z_class$stat[,1])
  
  param <- switch(type,
                  "Multinomial" = getParamMultinomial(outMixtComp$variable$param[[var]], nbClass),
                  "Gaussian" = getParamNumerical(outMixtComp$variable$param[[var]], nbClass, c("mean", "sd")),
                  "Poisson" = getParamNumerical(outMixtComp$variable$param[[var]], nbClass, "lambda"),
                  "NegativeBinomial" = getParamNumerical(outMixtComp$variable$param[[var]], nbClass, c("n", "p")),
                  "Weibull" = getParamNumerical(outMixtComp$variable$param[[var]], nbClass, c("k (shape)", "lambda (scale)")),
                  "Rank_ISR" = getParamRank_ISR(outMixtComp$variable$param[[var]], nbClass),
                  "Func_CS" = getParamFunc_CS(outMixtComp$variable$param[[var]], nbClass),
                  "LatentClass" = getParamNumerical(outMixtComp$variable$param[[var]], nbClass, "pi"),
                  warning("Not yet implemented."))
  
  return(param)
}

# @author Quentin Grimonprez
getParamNumerical <- function(param, nbClass, colNames)
{
  className <- unique(sapply(strsplit(rownames(param$stat), split = ","), function(x){ gsub("k: ", "", x[1])}))
  matrix(param$stat[,1], nrow = nbClass, byrow = TRUE, dimnames = list(paste0("k: ", className), colNames))
}

# @author Quentin Grimonprez
getParamFunc_CS <- function(param, nbClass)
{
  nSub <- getnSub(param$paramStr)
  nCoeff <- getnCoeff(param$paramStr)
  
  list(alpha = getParamNumerical(param$alpha, nbClass, unlist(lapply(paste0("s:", 1:nSub), function(x) paste0(x, c(", alpha0", ", alpha1"))))),
       beta =  getParamNumerical(param$beta, nbClass, unlist(lapply(paste0("s:", 1:nSub), function(x) paste0(x, paste0(", c:", 1:nCoeff))))),
       sd = getParamNumerical(param$sd, nbClass, paste0("s:", 1:nSub)))
}

# @author Quentin Grimonprez
getnCoeff <- function(paramStr)
{
  as.numeric(gsub("nCoeff:", "", strsplit(gsub(" ", "", paramStr), ",")[[1]][2]))
}

# @author Quentin Grimonprez
getnSub <- function(paramStr)
{
  as.numeric(gsub("nSub:", "", strsplit(gsub(" ", "", paramStr), ",")[[1]][1]))
}

# @author Quentin Grimonprez
getParamRank_ISR <- function(param, nbClass)
{
  mu <- t(sapply(param$mu$stat, function(x){x$rank}))
  rownames(mu) = paste0("k: ", 1:nbClass)
  
  list(pi = getParamNumerical(param$pi, nbClass, "pi"),
       mu = mu)  
}

# @author Quentin Grimonprez
getParamMultinomial <- function(param, nbClass)
{
  nbModalities <- length(param$stat[,1])/nbClass
  
  modalities <- gsub("k: .*, modality: ", "", rownames(param$stat)[1:nbModalities])
  getParamNumerical(param, nbClass, paste0("modality ", modalities))
}
