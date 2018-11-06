#' @title Get the estimated parameter
#'
#' @description Get the estimated parameter
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \link{mixtCompLearn} or \link{mixtCompPredict} functions.
#' @param varName name of the variable to get parameter
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
#' @seealso \link{plotDataBoxplot}  \link{plotDataCI}  
#' 
#' @family getter
#' @export
getParam <- function(outMixtComp, varName)
{
  type <- outMixtComp$variable$type[[varName]]
  type = ifelse(type == "Func_SharedAlpha_CS", "Func_CS", type)
  
  if(is.null(type))
  {
    warning("Bad variable name.")
    return(c())
  }
  
  nbClass <- length(outMixtComp$variable$param$z_class$stat[,1])
  
  param <- switch(type,
                  "Multinomial" = getParamMultinomial(outMixtComp$variable$param[[varName]], nbClass),
                  "Gaussian" = getParamNumerical(outMixtComp$variable$param[[varName]], nbClass, c("mean", "sd")),
                  "Poisson" = getParamNumerical(outMixtComp$variable$param[[varName]], nbClass, "lambda"),
                  "NegativeBinomial" = getParamNumerical(outMixtComp$variable$param[[varName]], nbClass, c("n", "p")),
                  "Weibull" = getParamNumerical(outMixtComp$variable$param[[varName]], nbClass, c("k (shape)", "lambda (scale)")),
                  "Rank_ISR" = getParamRank_ISR(outMixtComp$variable$param[[varName]], nbClass),
                  "Func_CS" = getParamFunc_CS(outMixtComp$variable$param[[varName]], nbClass),
                  "LatentClass" = getParamNumerical(outMixtComp$variable$param[[varName]], nbClass, "pi"),
                  warning("Not yet implemented."))
  
  return(param)
}

getParamNumerical <- function(param, nbClass, colNames)
{
  matrix(param$stat[,1], nrow = nbClass, byrow = TRUE, dimnames = list(paste0("k:",1:nbClass), colNames))
}


getParamFunc_CS <- function(param, nbClass)
{
  nSub <- getnSub(param$paramStr)
  nCoeff <- getnCoeff(param$paramStr)
  
  list(alpha = getParamNumerical(param$alpha, nbClass, unlist(lapply(paste0("s:", 1:nSub), function(x) paste0(x, c(", alpha0", ", alpha1"))))),
       beta =  getParamNumerical(param$beta, nbClass, unlist(lapply(paste0("s:", 1:nSub), function(x) paste0(x, paste0(", c:", 1:nCoeff))))),
       sd = getParamNumerical(param$sd, nbClass, paste0("s:", 1:nSub)))
}

getnCoeff <- function(paramStr)
{
  as.numeric(gsub("nCoeff: ", "", strsplit(paramStr, ",")[[1]][2]))
}

getnSub <- function(paramStr)
{
  as.numeric(gsub("nSub: ", "", strsplit(paramStr, ",")[[1]][1]))
}

getParamRank_ISR <- function(param, nbClass)
{
  mu <- t(sapply(param$mu$stat, function(x){x$rank}))
  rownames(mu) = paste0("k:",1:nbClass)
  
  list(pi = getParamNumerical(param$pi, nbClass, "pi"),
       mu = mu)  
}


getParamMultinomial <- function(param, nbClass)
{
  nbModalities <- length(param$stat[,1])/nbClass
  getParamNumerical(param, nbClass, paste0("modality ",1:nbModalities))
}



