# MixtComp version 4.0  - july 2019
# Copyright (C) Inria - Université de Lille - CNRS

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# 
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>
 

#' @import RMixtCompIO RMixtCompUtilities
#' @importFrom parallel detectCores
#' @importFrom graphics plot
#' @importFrom utils head
#' @importFrom ggplot2 ggplot aes_string labs geom_point geom_line
#' @importFrom plotly plot_ly add_trace layout %>%
#' @title RMixtComp
#' @docType package
#' @aliases RMixtComp-package
#' @name RMixtComp-package
#' @description  
#' MixtComp (Mixture Composer, \url{https://github.com/modal-inria/MixtComp}) is a model-based clustering package for mixed data.
#' 
#' It has been engineered around the idea of easy and quick integration of all new univariate models, under the conditional independence assumption. 
#' Five basic models (Gaussian, Multinomial, Poisson, Weibull, NegativeBinomial) are implemented, as well as two advanced models (Func_CS and Rank_ISR).
#' MixtComp has the ability to natively manage missing data (completely or by interval).
#' 
#' Online SaaS version (not up-to-date): \url{https://massiccc.lille.inria.fr/}
#' 
#' 
#' @details
#' Main functions are \link{mixtCompLearn} for clustering, \link{mixtCompPredict} for predicting the cluster of new samples with a model learnt with \link{mixtCompLearn}. 
#' \link{createAlgo} gives you default values for required parameters.
#' 
#' Read the help page of \link{mixtCompLearn} for available models and data format. A summary of these information can be accessed with the function \link{availableModels}.
#' 
#' All utility functions (getters, graphical) are in the \code{RMixtCompUtilities} package.
#' 
#' In order to have an overview of the output, you can use \link{print.MixtCompLearn}, \link{summary.MixtCompLearn} and \link{plot.MixtCompLearn} functions,
#' 
#' Getters are available to easily access some results (see. \link{mixtCompLearn} for output format): \link{getBIC}, \link{getICL}, \link{getCompletedData}, \link{getParam},
#'  \link{getTik}, \link{getEmpiricTik}, \link{getPartition}, \link{getType}, \link{getModel}, \link{getVarNames}.
#' 
#' 
#' You can compute discriminative powers and similarities with functions: \link{computeDiscrimPowerClass}, \link{computeDiscrimPowerVar}, \link{computeSimilarityClass}, \link{computeSimilarityVar}.
#' 
#' Graphics functions are \link{plot.MixtComp}, \link{plot.MixtCompLearn}, \link{heatmapClass}, \link{heatmapTikSorted}, \link{heatmapVar}, \link{histMisclassif}, \link{plotConvergence}, 
#' \link{plotDataBoxplot}, \link{plotDataCI}, \link{plotDiscrimClass}, \link{plotDiscrimVar}, \link{plotProportion}, \link{plotCrit}.
#' 
#' Datasets with running examples are provided: \link{titanic}, \link{CanadianWeather}, \link{prostate}, \link{simData}.
#' 
#' @examples 
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
#' # run RMixtComp for learning using only 3 variables
#' resLearn <- mixtCompLearn(simData$dataLearn$matrix, simData$model$unsupervised[1:3], algo,
#'                           nClass = 2:4)
#' 
#' summary(resLearn)
#' plot(resLearn)
#' 
#' # run RMixtComp for predicting
#' resPred <- mixtCompPredict(simData$dataPredict$matrix, simData$model$unsupervised[1:3], algo,
#'                            resLearn)
#' 
#' partitionPred <- getPartition(resPred)
#' print(resPred)
#' 
#' 
#' @seealso \code{\link{mixtCompLearn}} \code{\link{availableModels}} \code{RMixtCompUtilities}, \code{RMixtCompIO}. Other clustering packages: \code{Rmixmod}, \code{blockcluster}
#' 
#' @keywords package
NULL
