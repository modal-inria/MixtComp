#' @useDynLib RMixtComp
#' @import Rcpp
#' @import scales plotly
#' @importFrom grDevices n2mfrow
#' @importFrom graphics axis lines plot abline legend matplot par title
#' @importFrom stats optimize pnorm qnorm qpois qweibull dnorm qnbinom
#' @importFrom stats rbinom rmultinom rnorm rpois runif rweibull rnbinom
#'
#' @title RMixtComp
#' @docType package
#' @aliases RMixtComp-package
#' @name RMixtComp-package
#' @description  
#' Mixture Composer is a project to build mixture models with heterogeneous data
#' sets. The conditional independence property enables composing all the
#' data sets in a single mixture model.
#'
#' 
#' @details
#'
#' Main functions are \link{mixtCompLearn} for clustering, \link{mixtCompPredict} for predict the cluster of new samples with a model learned with \link{mixtCompLearn}. \link{createAlgo} gives you default values for required parameters.
#' 
#' Getters are available to easily access some results (see. \link{mixtCompLearn} for output format): \link{getBIC}, \link{getICL}, \link{getCompletedData}, \link{getParam}, \link{getTik}, \link{getPartition}, \link{getType}, \link{getVarNames}.
#' 
#' Three datasets are provided and contains examples: \link{simData}, \link{titanic}, \link{CanadaianWeather}.
#' 
#' You can compute discriminative power and similiraties with functions: \link{computeDiscrimPowerClass}, \link{computeDiscrimPowerVar}, \link{computeSimilarityClass}, \link{computeSimilarityVar}.
#' 
#' Graphics functions are \link{heatmapClass}, \link{heatmapTikSorted}, \link{heatmapVbles}, \link{histMisclassif}, \link{plotConvergence}, \link{plotDataBoxplot}, \link{plotDataCI}, \link{plotDiscrimClass}, \link{plotDiscrimVbles}.
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
#' resLearn <- mixtCompLearn(simDataLearn$matrix, desc$unsupervised, algo, nbClass = 2:4)
#' 
#' # run RMixtCompt for predicting
#' resPred <- mixtCompPredict(simDataPredict$matrix, desc$unsupervised, algo, resLearn)
#' 
#' }
#' 
#' @seealso \link{mixtCompLearn}
#' 
#' @keywords package
NULL
