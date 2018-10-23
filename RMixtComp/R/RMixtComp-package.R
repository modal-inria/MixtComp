#' @useDynLib RMixtComp
#' @import Rcpp doParallel scales plotly
#' @importFrom grDevices n2mfrow
#' @importFrom graphics axis lines plot abline legend matplot par title
#' @importFrom stats optimize pnorm qnorm qpois qweibull dnorm qnbinom
#' @importFrom stats rbinom rmultinom rnorm rpois runif rweibull rnbinom
#' @importFrom parallel makeCluster detectCores stopCluster
#' @importFrom foreach foreach %dopar%
#' @importFrom ggplot2 ggplot aes ylim labs theme theme_minimal element_text element_blank geom_tile geom_text geom_bar geom_point geom_rect scale_fill_gradient scale_fill_manual 
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
#' Graphics functions are \link{heatmapClass}, \link{heatmapTikSorted}, \link{heatmapVar}, \link{histMisclassif}, \link{plotConvergence}, \link{plotDataBoxplot}, \link{plotDataCI}, \link{plotDiscrimClass}, \link{plotDiscrimVar}.
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
#' resLearn <- mixtCompLearn(simDataLearn$matrix, simDesc$unsupervised, algo, nClass = 2:4)
#' 
#' # run RMixtCompt for predicting
#' resPred <- mixtCompPredict(simDataPredict$matrix, simDesc$unsupervised, algo, resLearn)
#' 
#' }
#' 
#' @seealso \link{mixtCompLearn}
#' 
#' @keywords package
NULL
