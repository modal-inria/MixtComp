#' @useDynLib RMixtCompIO
#' @import Rcpp doParallel
#' @importFrom parallel makeCluster stopCluster
#' @importFrom foreach foreach %dopar%
#' @importFrom stats rbinom rmultinom rnbinom rnorm rpois runif rweibull
#' 
#' @title RMixtCompIO
#' @docType package
#' @aliases RMixtCompIO-package
#' @name RMixtCompIO-package
#' @description  
#' MixtComp (Mixture Composer) is a model-based clustering package for mixed data originating from the Modal team (Inria Lille).
#' 
#' It has been engineered around the idea of easy and quick integration of all new univariate models, under the conditional independence assumption. 
#' Five basic models (Gaussian, Multinomial, Poisson, Weibull, NegativeBinomial) are implemented, as well as two advanced models (Func_CS and Rank_ISR).
#' MixtComp has the ability to natively manage missing data (completely or by interval). MixtComp is used as an R package, but its internals are coded in C++ using state of the art libraries for faster computation.
#' This package contains the minimal R interface of the C++ library.
#' 
#' Online SaaS version: \url{https://massiccc.lille.inria.fr/}
#' 
#' 
#' @details
#' The main function is \link{rmcMultiRun} that runs a SEM algorithm to learn a mixture model.
#' 
#' @examples 
#' \donttest{
#' dataLearn <- list(var1 = as.character(c(rnorm(50, -2, 0.8), rnorm(50, 2, 0.8))),
#'                   var2 = as.character(c(rnorm(50, 2), rpois(50, 8))))
#'                   
#' dataPredict <- list(var1 = as.character(c(rnorm(10, -2, 0.8), rnorm(10, 2, 0.8))),
#'                     var2 = as.character(c(rnorm(10, 2), rpois(10, 8))))
#'                   
#' model <- list(var1 = list(type = "Gaussian", paramStr = ""),
#'               var2 = list(type = "Poisson", paramStr = ""))#' 
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
#' 
#' # define the algorithm's parameters
#' algo <- list(nClass = , 
#'              nInd = ,
#'              nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # run RMixtComp in unsupervised clustering mode + data as matrix
#' resLearn <- rmcMultiRun(algo, dataLearn, model, nRun = 3)
#' 
#' 
#' # run RMixtCompt in predict mode + data as list
#' algo$nInd = 20
#' resPredict <- rmcMultiRun(algo, data, model, resLearn)
#' 
#' }
#' 
#' @seealso \code{\link{rmcMultiRun}}. Other clustering packages: \code{Rmixmod}, \code{blockcluster}
#' 
#' @keywords package
NULL
