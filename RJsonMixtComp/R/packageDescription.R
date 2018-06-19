# packages to import for the NAMESPACE file
#' @import jsonlite


# functions to import for the NAMESPACE file.
#' @importFrom utils read.table

#' @title RJsonMixtComp
#' @docType package
#' @aliases RJsonMixtComp-package
#' @name RJsonMixtComp-package
#' @description  
#' Statistics analysis with MixtComp
#'
#' 
#' @details
#' \tabular{ll}{
#' Package: \tab RJsonMixtComp\cr
#' Type: \tab Package\cr
#' Version: \tab 0.1\cr
#' Date: \tab 2017-05-10\cr
#' License: \tab  \cr
#' }
#' 
#' 
#' @author Quentin Grimonprez
#' 
#' Maintainer: Quentin Grimonprez <quentin.grimonprez@@inria.fr>
#'  
#' 
#' @examples 
#' \dontrun{
#' # get the path to the data of the package
#' pathToData <- system.file("extdata", "data.csv", package = "RJsonMixtComp")
#' pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RJsonMixtComp")
#'
#' resGetData <- RJsonMixtComp:::getData(c(pathToData, pathToDescriptor)) 
#'
#' # learn
#' resLearn <- JsonMixtCompCluster(dataList = resGetData$lm,
#'                                 mcStrategy = list(nbBurnInIter = 100, nbIter = 100, 
#'                                                   nbGibbsBurnInIter = 100, nbGibbsIter = 100),
#'                                 nbClass = 2, confidenceLevel = 0.95, 
#'                                 jsonInputFile = "datalearn.json",
#'                                 jsonOutputFile = "reslearn.json")
#'
#'
#'
#' # predict : require a json file output from JsonMixtCompCluster ("reslearn.json" here)
#' resPredict <- JsonMixtCompPredict(dataList = resGetData$lm,
#'                                   mcStrategy = list(nbBurnInIter = 100, nbIter = 100, 
#'                                                     nbGibbsBurnInIter = 100, nbGibbsIter = 100),
#'                                   nbClass = 2, confidenceLevel = 0.95, 
#'                                   jsonInputFile = "datalearn.json",
#'                                   jsonOutputFile = "respredict.json",
#'                                   jsonMixtCompLearnFile = "reslearn.json")
#'
#' # remove created files of the example
#' file.remove(c("reslearn.json", "respredict.json", "datalearn.json"))
#' }
#' 
#' 
#' @keywords package
NULL