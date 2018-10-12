#' @useDynLib RMixtComp
#' @import Rcpp
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
#' 
#' You can compute discriminative power and similiraties with functions: \link{computeDiscrimPowerClass}, \link{computeDiscrimPowerVar}, \link{computeSimilarityClass}, \link{computeSimilarityVar}
#' 
#' All graphics functions are in the \emph{RMixtCompPlot} package. 
#' 
#' 
#' @author Vincent KUBICKI, Matthieu MARBAC, Quentin GRIMONPREZ, Serge IOVLEFF
#' 
#' Maintainer: Vincent Kubicki  <vincent.kubicki@@inria.fr>
#'  
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


#' Simulated Heterogeneous data
#'
#' @name simData
#' @aliases simDataLearn simDataPredict simDesc
#' @docType data
#' @keywords data
#' 
#' @usage data(simData)
#' 
#' @format Three object: \emph{simDataLearn}, \emph{simDataPredict} and \emph{simDesc}.
#' 
#' \emph{simDataLearn} is a list containing the data in the three accepted format (list, data.frame and matrix). Data consists of 200 individuals and 9 variables.
#' 
#' \emph{simDataPredict} is a list containing the data in the three accepted format (list, data.frame and matrix). Data consists of 100 individuals and 8 variables.
#' 
#' \emph{simDesc} is a list containing the descriptor lists used for clustering \emph{simDesc$unsupervised} and classification \emph{simDesc$supervised}.
#' 
#' @examples
#' data(simData)
#' str(simDataLearn)
#' str(simDataPredict)
#' str(simDesc)
#'   
NULL