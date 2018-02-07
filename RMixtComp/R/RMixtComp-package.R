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
#' Version: \tab 0.2\cr
#' Date: \tab 2018-02-07\cr
#' }
#' 
#' 
#' @author Vincent KUBICKI, Matthieu MARBAC, Quentin GRIMONPREZ, Serge IOVLEFF
#' 
#' Maintainer: Vincent Kubicki  <vincent.kubicki@@inria.fr>
#'  
#' 
#' @examples 
#' \dontrun{
#' # path to files
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descriptor.csv", package = "RMixtComp")
#' 
#' resGetData <- getData(c(pathToData, pathToDescriptor))
#' 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#'                    nbIter = 100,
#'                    nbGibbsBurnInIter = 50,
#'                    nbGibbsIter = 50)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # run RMixtCompt for predicting
#' resPred <- mixtCompPredict(resGetData$lm, res$variable$param, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
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
#'  \item{variable}{information about the estimated parameters and completed data (see \emph{Details}).}
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
#' The convergence of the SEM can be checked with \emph{completedProbabilityLogBurnIn} and \emph{completedProbabilityLogRun}.
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
#' Example of output object with variables named "categorical", "gaussian", "ordinal", "rank", "functional" and "poisson".
#' 
#' \tabular{lll}{
#' output  \cr
#' |_______ \tab strategy \tab __ nbBurnInIter\cr
#' |        \tab          \tab |__ nbIter\cr
#' |        \tab          \tab |__ nbGibbsBurnInIter\cr
#' |        \tab          \tab |__ nbGibbsIter\cr
#' | \cr
#' |_______ \tab mixture \tab __ BIC \cr
#' |        \tab         \tab |_ ICL\cr
#' |        \tab         \tab |_ lnCompletedLikelihood\cr
#' |        \tab         \tab |_ lnObservedLikelihood \cr
#' |        \tab         \tab |_ IdClass  \cr
#' |        \tab         \tab |_ delta  \cr
#' |        \tab         \tab |_ mode \cr
#' |        \tab         \tab |_ runTime \cr
#' |        \tab         \tab |_ nbSample \cr
#' |        \tab         \tab |_ nbInd  \cr
#' |        \tab         \tab |_ nbFreeParameters \cr
#' |        \tab         \tab |_ nbCluster \cr
#' |        \tab         \tab |_ warnLog \cr
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
#'          \tab          \tab          \tab |_ poisson  \tab __ NumericalParam \tab __ stat\cr
#'          \tab          \tab          \tab |           \tab                   \tab |_ log \cr
#'          \tab          \tab          \tab |           \tab                   \tab |_ paramStr \cr
#'          \tab          \tab          \tab |_ ordinal \tab __ muPi \tab __ stat\cr
#'          \tab          \tab          \tab |          \tab         \tab |_ log \cr
#'          \tab          \tab          \tab |          \tab         \tab |_ paramStr \cr
#'          \tab          \tab          \tab |_ functional \tab __ alpha \tab __ stat\cr
#'          \tab          \tab          \tab |             \tab |        \tab |_ log \cr
#'          \tab          \tab          \tab |             \tab |        \tab |_ paramStr \cr
#'          \tab          \tab          \tab |             \tab |_ beta  \tab __ stat\cr
#'          \tab          \tab          \tab |             \tab |        \tab |_ log \cr
#'          \tab          \tab          \tab |             \tab |        \tab |_ paramStr \cr
#'          \tab          \tab          \tab |             \tab |_ sd    \tab __ stat\cr
#'          \tab          \tab          \tab |             \tab          \tab |_ log \cr
#'          \tab          \tab          \tab |             \tab          \tab |_ paramStr \cr
#'          \tab          \tab          \tab |_ rank \tab __ mu \tab __ stat\cr
#'          \tab          \tab          \tab         \tab |     \tab |_ log \cr
#'          \tab          \tab          \tab         \tab |     \tab |_ paramStr \cr
#'          \tab          \tab          \tab         \tab |_ pi \tab __ stat\cr
#'          \tab          \tab          \tab         \tab       \tab |_ log \cr
#'          \tab          \tab          \tab         \tab       \tab |_ paramStr \cr
#'          \tab          \tab          \tab         \tab       \tab \cr
#' }                  
#'                   
#' @examples 
#' \dontrun{
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descriptor.csv", package = "RMixtComp")
#' 
#' resGetData <- getData(c(pathToData, pathToDescriptor))
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#'                    nbIter = 100,
#'                    nbGibbsBurnInIter = 50,
#'                    nbGibbsIter = 50)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # run RMixtCompt for predicting
#' resPred <- mixtCompPredict(resGetData$lm, res$variable$param, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
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
