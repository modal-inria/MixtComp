#' @title RMixtComp
#' @docType package
#' @aliases RMixtComp-package
#' @name RMixtComp-package
#' @description  
#' Mixture Composer is a project to build mixture models with heterogeneous data
#' sets. The conditional independence property enable composing all the
#' data sets in a single mixture model.
#'
#' 
#' @details
#' \tabular{ll}{
#' Package: \tab RMixtComp\cr
#' Type: \tab Package\cr
#' Version: \tab 0.1\cr
#' Date: \tab 2016-10-10\cr
#' License: \tab GPL (>=2) \cr
#' }
#' 
#' 
#' @author Vincent Kubicki
#' 
#' Maintainer: Vincent Kubicki  <vincent.kubicki@@inria.fr>
#'  
#' 
#' @examples 
#' \dontrun{
#' # load the train data
#' resGetData <- getData(c("dataTrain.csv", "descriptor.csv")) 
#' # load the test data
#' resGetNewData <- getData(c("dataTest.csv", "descriptor.csv")) 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#' nbIter = 100,
#' nbGibbsBurnInIter = 50,
#' nbGibbsIter = 50,
#' parameterEdgeAuthorized = FALSE)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # run RMixtCompt for predicting
#' resPred <- mixtCompCluster(resGetNewData$lm, res$variable$param, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' }
#' 
#' @seealso \link{mixtCompCluster} \link{mixtCompPredict}
#' 
#' @keywords package
NULL

#'
#' @name mixtCompCluster
#' @aliases mixtCompPredict
#'
#' @usage mixtCompCluster(dataList, mcStrategy, nbClass, confidenceLevel)
#' mixtCompPredict(dataList, paramList, mcStrategy, nbClass, confidenceLevel)
#'
#' @title Learn and predict using RMixtComp
#' 
#' @description Estimate the parameter of a mixture model or predict the cluster of new samples.
#' 
#' @title Learn and predict using RMixtComp
#' 
#' Estimate the parameter of a mixture model or predict the cluster of new samples.
#' 
#' @param dataList \emph{lm} element of the output of \link{getData}.
#' @param paramList parameters of the mixture model (only for \emph{mixtCompPredict} function). Output \emph{$variable$param} from \emph{mixtCompCluster} function.
#' @param mcStrategy a list containing the parameters of the SEM-Gibbs algorithm (see \emph{Details}).
#' @param nbClass the number of class of the mixture model.
#' @param confidenceLevel level of confidence for interval of estimated parameters.
#'
#' @return A list containing 3 lists :
#' \describe{
#'  \item{strategy}{a copy of \emph{mcStrategy} parameter.}
#'  \item{mixture}{information about the mixture.}
#'  \item{variable}{infoRmation about the estimated parameters (see \emph{Details}).}
#' }
#' 
#' @details 
#' \emph{mcStrategy} is a list containing the different number of iterations for the algorithm. 
#' The algorithm is decomposed in a burn-in phase and a normal phase. 
#' Estimates from the burn-in phase are not shown in output.
#' \describe{
#'   \item{nbBurnInIter}{Number of iterations of the burn-in part of the SEM algorithm.}
#'   \item{nbIter}{Number of iterations of the SEM algorithm.}
#'   \item{nbGibbsBurnInIter}{Number of iterations of the burn-in part of the Gibbs algorithm.}
#'   \item{nbGibbsIter}{Number of iterations of the Gibbs algorithm.}
#'   \item{parameterEdgeAuthorized}{\emph{TRUE} or \emph{FALSE} (default value). 
#'   If \emph{TRUE}, a bias is introduced by avoiding some non stable situation (a proportion = 0 for example) by using a minimal value (1e-8).
#'   Using \emph{parameterEdgeAuthorized=TRUE} can significantly reduce the computation time. }
#' }
#' 
#' 
#' 
#' The output list \emph{mixture} contains the \emph{BIC}, \emph{ICL}, number of samples (\emph{nbSample} or \emph{nbInd}),
#' the number of class (\emph{nCluster}), the observed loglikelihood (\emph{lnObservedLikelihood}),
#' the completed loglikelihood (\emph{lnCompletedLikelihood}), the number of free parameters of the mixture (\emph{nbFreeParameters}),
#' the function used to create the output (\emph{mode} = "learn" for \link{mixtCompCluster} or "predict" for \link{mixtCompPredict}),
#' something mathematic (\emph{IdClass}), the execution time (\emph{runTime}) and the warnings generated (\emph{warnlog}).
#' 
#' 
#' The output list \emph{variable} contains 3 lists : \emph{data}, \emph{type} and \emph{param}. 
#' Each of these lists contains a list for each variable (the name of each list is the name of the variable) and for the class of samples (\emph{z_class}).
#' 
#' 
#' 
#' 
#' @examples 
#' \dontrun{
#' # load the data
#' resGetData <- getData(c("dataTrain.csv", "descriptor.csv")) 
#' resGetNewData <- getData(c("dataTest.csv", "descriptor.csv")) 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#' nbIter = 100,
#' nbGibbsBurnInIter = 50,
#' nbGibbsIter = 50,
#' parameterEdgeAuthorized = FALSE)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # run RMixtCompt for predicting
#' resPred <- mixtCompCluster(resGetNewData$lm, res$variable$param, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' }
#' 
#' 
#'
#' @seealso \link{getData} 
#' 
#' @author Vincent Kubicki
#' 
#'
NULL
