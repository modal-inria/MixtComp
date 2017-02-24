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
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#'
#'
#' # Plot like Figure 1 (left)
#' plotDiscrimVbles(res)
#'
#' # Plot like Figure 1 (right)
#' plotDiscrimClass(res)
#'
#' # Plot like Figure 2 (left)
#' heatmapVbles(res)
#'
#' # Plot like Figure 2 (right)
#' heatmapClass(res)
#'
#' # Plot like Figure 3 (left)
#' histMisclassif(res)
#'
#' # Plot like Figure 3 (right)
#' heatmapTikSorted(res)
#' 
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
#' @param confidenceLevel quantile for confidence interval of estimated parameters.
#'
#' @return A list containing 3 lists :
#' \describe{
#'  \item{strategy}{a copy of \emph{mcStrategy} parameter.}
#'  \item{mixture}{information about the mixture (see \emph{Details}).}
#'  \item{variable}{information about the estimated parameters (see \emph{Details}).}
#' }
#' 
#' @details 
#' Details about the output object of \emph{mixtCompCluster} and \emph{mixtCompPredict} functions.
#' 
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
#'   }
#' }
#' 
#' 
#' 
#' @section Output object:
#' The output list \emph{mixture} contains the \emph{BIC}, \emph{ICL}, number of samples (\emph{nbSample} or \emph{nbInd}),
#' the number of class (\emph{nCluster}), the observed loglikelihood (\emph{lnObservedLikelihood}),
#' the completed loglikelihood (\emph{lnCompletedLikelihood}), the number of free parameters of the mixture (\emph{nbFreeParameters}),
#' the function used to create the output (\emph{mode} = "learn" for \link{mixtCompCluster} or "predict" for \link{mixtCompPredict}),
#' something mathematic (\emph{IdClass}), the execution time (\emph{runTime}) and the warnings generated (\emph{warnlog}).
#' 
#' 
#' The output list \emph{variable} contains 3 lists : \emph{data}, \emph{type} and \emph{param}. 
#' Each of these lists contains a list for each variable (the name of each list is the name of the variable) and for the class of samples (\emph{z_class}).
#' The \emph{type} list contains the model used for each variable. Each list of the \emph{data} list contains
#'  the completed data in the \emph{completed} element and some statistics about them (\emph{stat}). The estimated parameter can be found in the \emph{stat} 
#'  element in the \emph{param} list (see Section \emph{View of an output object}).
#' 
#' 
#' @section View of an output object:
#' Example of output object with variables named "categorical", "gaussian", "ordinal",...
#' 
#' \tabular{lll}{
#' output  \cr
#' |_______ \tab strategy \tab __ nbBurnInIter\cr
#' |        \tab          \tab |__ nbIter\cr
#' |        \tab          \tab |__ nbGibbsBurnInIter\cr
#' |        \tab          \tab |__ nbGibbsIter\cr
#' |        \tab          \tab |__ parameterEdgeAuthorized\cr
#' | \cr
#' |_______ \tab mixture \tab __ BIC \cr
#' |        \tab         \tab |_ ICL\cr
#' |        \tab         \tab |_ lnCompletedLikelihood\cr
#' |        \tab         \tab |_ lnObservedLikelihood \cr
#' |        \tab         \tab |_ IdClass  \cr
#' |        \tab         \tab |_ mode \cr
#' |        \tab         \tab |_ runTime \cr
#' |        \tab         \tab |_ nbSample \cr
#' |        \tab         \tab |_ nbInd  \cr
#' |        \tab         \tab |_ nbFreeParameters \cr
#' |        \tab         \tab |_ nbCluster \cr
#' |        \tab         \tab |_ warnlog \cr
#' }
#' \tabular{llllll}{
#' |  \cr
#' |_______ \tab variable \tab __ type \tab __ z_class  \cr
#'          \tab          \tab |       \tab |_ categorical \cr
#'          \tab          \tab |       \tab |_ gaussian \cr
#'          \tab          \tab |       \tab |_ ...   \cr
#'          \tab          \tab |       \tab \cr
#'          \tab          \tab |_ data \tab __ z_class \tab __ completed\cr
#'          \tab          \tab |       \tab |          \tab |_ stat \cr
#'          \tab          \tab |       \tab |_ categorical \tab __ completed\cr
#'          \tab          \tab |       \tab |              \tab |_ stat \cr
#'          \tab          \tab |       \tab |_ ...         \tab \cr
#'          \tab          \tab |       \tab \cr
#'          \tab          \tab |_ param \tab __ z_class \tab __ pi \tab __ stat\cr
#'          \tab          \tab          \tab |          \tab       \tab |_ log \cr
#'          \tab          \tab          \tab |          \tab       \tab |_ paramStr \cr
#'          \tab          \tab          \tab |_ categorical \tab __ NumericalParam \tab __ stat\cr
#'          \tab          \tab          \tab |              \tab                   \tab |_ log \cr
#'          \tab          \tab          \tab |              \tab                   \tab |_ paramStr \cr
#'          \tab          \tab          \tab |_ gaussian \tab __ NumericalParam \tab __ stat\cr
#'          \tab          \tab          \tab |           \tab                   \tab |_ log \cr
#'          \tab          \tab          \tab |           \tab                   \tab |_ paramStr \cr
#'          \tab          \tab          \tab |_ ordinal \tab __ muPi \tab __ stat\cr
#'          \tab          \tab          \tab |          \tab         \tab |_ log \cr
#'          \tab          \tab          \tab |          \tab         \tab |_ paramStr \cr
#'          \tab          \tab          \tab |_ ...     \tab         \tab \cr
#' }                  
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
