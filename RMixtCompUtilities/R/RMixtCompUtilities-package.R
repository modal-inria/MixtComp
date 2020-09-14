# MixtComp version 4 - july 2019
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
 

#' @import scales plotly
#' @importFrom stats optimize pnorm qnbinom qnorm qpois qweibull uniroot
#' @importFrom grDevices n2mfrow
#' @importFrom graphics axis lines plot abline legend matplot par title
#' @importFrom ggplot2 ggplot aes aes_string ylim labs theme theme_minimal element_text element_blank 
#' @importFrom ggplot2 geom_tile geom_text geom_bar geom_point geom_rect geom_ribbon geom_line geom_histogram 
#' @importFrom ggplot2 scale_fill_gradient scale_fill_manual position_dodge scale_color_discrete scale_y_continuous scale_fill_discrete
#' @importFrom utils head
#' 
#' @title RMixtCompUtilities
#' @docType package
#' @aliases RMixtCompUtilities-package
#' @name RMixtCompUtilities-package
#' @description  
#' MixtComp (Mixture Composer, \url{https://github.com/modal-inria/MixtComp}) is a model-based clustering package for mixed data originating from the Modal team (Inria Lille).
#' 
#' It has been engineered around the idea of easy and quick integration of all new univariate models, under the conditional independence assumption. 
#' Five basic models (Gaussian, Multinomial, Poisson, Weibull, NegativeBinomial) are implemented, as well as two advanced models (Func_CS and Rank_ISR).
#' MixtComp has the ability to natively manage missing data (completely or by interval). MixtComp is used as an R package, but its internals are coded in C++ using state of the art libraries for faster computation.
#' 
#'  
#' Online SaaS version (not up-to-date): \url{https://massiccc.lille.inria.fr/}
#' 
#' This package contains plots, getters and format functions to simplify the use of \code{RMixtComp} and \code{RMixtCompIO} packages. It is recommended to use \code{RMixtComp} (instead of \code{RMixtCompIO}) which is more user-friendly.
#' 
#' @details
#' \link{createAlgo} gives you default values for required parameters.
#' 
#' \code{convertFunctionalToVector}, \code{createFunctional} and \code{refactorCategorical} functions help to transform data to the required format.
#' 
#' Getters are available to easily access some results: \link{getBIC}, \link{getICL}, \link{getCompletedData}, \link{getParam},
#'  \link{getTik}, \link{getEmpiricTik}, \link{getPartition}, \link{getType}, \link{getModel}, \link{getVarNames}.
#' 
#' 
#' You can compute discriminative powers and similarities with functions: \link{computeDiscrimPowerClass}, \link{computeDiscrimPowerVar}, \link{computeSimilarityClass}, \link{computeSimilarityVar}.
#' 
#' Graphics functions are \link{plot.MixtComp}, \link{heatmapClass}, \link{heatmapTikSorted}, \link{heatmapVar}, \link{histMisclassif}, \link{plotConvergence}, 
#' \link{plotDataBoxplot}, \link{plotDataCI}, \link{plotDiscrimClass}, \link{plotDiscrimVar}, \link{plotProportion}.
#' 
#' 
#' @seealso \code{RMixtComp} \code{RMixtCompIO} \code{Rmixmod}, \code{blockcluster} packages
#' 
#' @keywords package
NULL
