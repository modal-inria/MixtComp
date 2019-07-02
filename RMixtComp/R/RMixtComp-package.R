#' @import RMixtCompIO scales plotly
#' @importFrom parallel detectCores
#' @importFrom grDevices n2mfrow
#' @importFrom graphics axis lines plot abline legend matplot par title
#' @importFrom stats optimize pnorm qnorm qpois qweibull dnorm qnbinom
#' @importFrom ggplot2 ggplot aes aes_string ylim labs theme theme_minimal element_text element_blank 
#' @importFrom ggplot2 geom_tile geom_text geom_bar geom_point geom_rect geom_ribbon geom_line geom_histogram scale_fill_gradient scale_fill_manual position_dodge scale_color_discrete
#' @importFrom utils head
#' 
#' @title RMixtComp
#' @docType package
#' @aliases RMixtComp-package
#' @name RMixtComp-package
#' @description  
#' MixtComp (Mixture Composer) is a model-based clustering package for mixed data originating from the Modal team (Inria Lille).
#' 
#' It has been engineered around the idea of easy and quick integration of all new univariate models, under the conditional independence assumption. 
#' Five basic models (Gaussian, Multinomial, Poisson, Weibull, NegativeBinomial) are implemented, as well as two advanced models (Func_CS and Rank_ISR).
#' MixtComp has the ability to natively manage missing data (completely or by interval). MixtComp is used as an R package, but its internals are coded in C++ using state of the art libraries for faster computation.
#' 
#' Online SaaS version: \url{https://massiccc.lille.inria.fr/}
#' 
#' 
#' @details
#' Main functions are \link{mixtCompLearn} for clustering, \link{mixtCompPredict} for predicting the cluster of new samples with a model learnt with \link{mixtCompLearn}. 
#' \link{createAlgo} gives you default values for required parameters.
#' 
#' Read the help page of \link{mixtCompLearn} for available models and data format. A summary of these information can be accessed with the function \link{availableModels}.
#' 
#' Getters are available to easily access some results (see. \link{mixtCompLearn} for output format): \link{getBIC}, \link{getICL}, \link{getCompletedData}, \link{getParam},
#'  \link{getTik}, \link{getEmpiricTik}, \link{getPartition}, \link{getType}, \link{getModel}, \link{getVarNames}.
#' 
#' Datasets with running examples are provided: \link{titanic}, \link{CanadianWeather}, \link{prostate}, \link{simData}.
#' 
#' You can compute discriminative powers and similarities with functions: \link{computeDiscrimPowerClass}, \link{computeDiscrimPowerVar}, \link{computeSimilarityClass}, \link{computeSimilarityVar}.
#' 
#' Graphics functions are \link{plot.MixtComp}, \link{plot.MixtCompLearn}, \link{heatmapClass}, \link{heatmapTikSorted}, \link{heatmapVar}, \link{histMisclassif}, \link{plotConvergence}, 
#' \link{plotDataBoxplot}, \link{plotDataCI}, \link{plotDiscrimClass}, \link{plotDiscrimVar}, \link{plotProportion}, \link{plotCrit}.
#' 
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters: you can use createAlgo function
#' algo <- list(nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # run RMixtComp for learning
#' resLearn <- mixtCompLearn(simData$dataLearn$matrix, simData$model$unsupervised, algo, nClass = 2:4)
#' 
#' # run RMixtComp for predicting
#' resPred <- mixtCompPredict(simData$dataPredict$matrix, simData$model$unsupervised, algo, resLearn)
#' 
#' }
#' 
#' @seealso \code{\link{mixtCompLearn}} \code{\link{availableModels}}. Other clustering packages: \code{Rmixmod}, \code{blockcluster}
#' 
#' @keywords package
NULL
