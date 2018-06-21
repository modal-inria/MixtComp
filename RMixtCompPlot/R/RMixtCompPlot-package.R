#' 
#' @import jsonlite scales plotly ClusVis
#' @importFrom grDevices n2mfrow
#' @importFrom graphics axis lines plot abline legend matplot par
#' @importFrom stats optimize pnorm qnorm qpois qweibull dnorm rnorm
#'
#' @title RMixtCompPlot
#' @docType package
#' @aliases RMixtCompPlot-package
#' @name RMixtCompPlot-package
#' @description  
#' Mixture Composer is a project to build mixture models with heterogeneous data
#' sets. The conditional independence property enables composing all the
#' data sets in a single mixture model.
#'
#' 
#' @details
#' \tabular{ll}{
#' Package: \tab MixtCompPlot\cr
#' Type: \tab Package\cr
#' Version: \tab 0.1\cr
#' Date: \tab 2018-06-07\cr
#' }
#' 
#' 
#' @author Matthieu MARBAC, Quentin GRIMONPREZ
#' 
#' Maintainer: Quentin GRIMONPREZ  <quentin.grimonprez@@inria.fr>
#'  
#' 
#' @examples 
#' \dontrun{
#'
#' library(RMixtComp)
#'
#' # path to files
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RMixtComp")
#' 
#' resGetData <- getData(c(pathToData, pathToDescriptor))
#' 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- createMcStrategy()
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # run RMixtCompt for predicting
#' resPred <- mixtCompPredict(resGetData$lm, res$variable$param, mcStrategy, nbClass = 2, 
#'                            confidenceLevel = 0.95)
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
#' 
#' @keywords package
NULL

