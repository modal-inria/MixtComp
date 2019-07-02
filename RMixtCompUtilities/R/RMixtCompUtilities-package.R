#' @import scales plotly
#' @importFrom stats optimize pnorm qnbinom qnorm qpois qweibull
#' @importFrom grDevices n2mfrow
#' @importFrom graphics axis lines plot abline legend matplot par title
#' @importFrom ggplot2 ggplot aes aes_string ylim labs theme theme_minimal element_text element_blank 
#' @importFrom ggplot2 geom_tile geom_text geom_bar geom_point geom_rect geom_ribbon geom_line geom_histogram scale_fill_gradient scale_fill_manual position_dodge scale_color_discrete
#' @importFrom utils head
#' 
#' @title RMixtCompUtilities
#' @docType package
#' @aliases RMixtCompUtilities-package
#' @name RMixtCompUtilities-package
#' @description  
#' MixtComp (Mixture Composer) is a model-based clustering package for mixed data originating from the Modal team (Inria Lille).
#' 
#' It has been engineered around the idea of easy and quick integration of all new univariate models, under the conditional independence assumption. 
#' Five basic models (Gaussian, Multinomial, Poisson, Weibull, NegativeBinomial) are implemented, as well as two advanced models (Func_CS and Rank_ISR).
#' MixtComp has the ability to natively manage missing data (completely or by interval). MixtComp is used as an R package, but its internals are coded in C++ using state of the art libraries for faster computation.
#' 
#'  
#' Online SaaS version: \url{https://massiccc.lille.inria.fr/}
#' 
#' This package containtains plots, getters and format functions to simplify the use of RMixtComp.
#' 
#' @details
#' \link{createAlgo} gives you default values for required parameters.
#' 
#' Getters are available to easily access some results: \link{getBIC}, \link{getICL}, \link{getCompletedData}, \link{getParam},
#'  \link{getTik}, \link{getEmpiricTik}, \link{getPartition}, \link{getType}, \link{getModel}, \link{getVarNames}.
#' 
#' 
#' You can compute discriminative powers and similarities with functions: \link{computeDiscrimPowerClass}, \link{computeDiscrimPowerVar}, \link{computeSimilarityClass}, \link{computeSimilarityVar}.
#' 
#' Graphics functions are \link{plot.MixtComp}, \link{heatmapClass}, \link{heatmapTikSorted}, \link{heatmapVar}, \link{histMisclassif}, \link{plotConvergence}, 
#' \link{plotDataBoxplot}, \link{plotDataCI}, \link{plotDiscrimClass}, \link{plotDiscrimVar}, \link{plotProportion}, \link{plotCrit}.
#' 
#' @examples 
#' \donttest{
#' library(RMixtComp)
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
#' @seealso \code{RMixtComp} \code{Rmixmod}, \code{blockcluster}
#' 
#' @keywords package
NULL
