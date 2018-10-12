# packages to import for the NAMESPACE file
#' @import jsonlite
#' 

#' @title RMixtComp
#' @docType package
#' @aliases RMixtComp-package
#' @name RJMixtComp-package
#' @description  
#' Statistics analysis with MixtComp
#'
#' 
#' @details
#' Use JMixtComp
#' 
#' 
#' @author Quentin Grimonprez
#' 
#' Maintainer: Quentin Grimonprez <quentin.grimonprez@@inria.fr>
#'  
#' 
#' @examples 
#' \donttest{
#' pathToData <- system.file("extdata", "data.json", package = "RJMixtComp")
#' pathToDescriptor <- system.file("extdata", "desc.json", package = "RJMixtComp")
#' 
#' data <- as.data.frame(fromJSON(pathToData))
#' descriptor <- fromJSON(pathToDescriptor)
#' strategy <- list(nbBurnInIter = 50, nbIter = 50, nbGibbsBurnInIter = 20,
#'                  nbGibbsIter = 20, nInitPerClass = 10, nSemTry = 5, confidenceLevel = 0.95)
#' 
#' resLearn <- JsonMixtCompLearn(data, descriptor, algo = strategy, nClass = 2,
#'                               confidenceLevel = 0.95, inputPath = ".", outputFile = "reslearn.json")
#' 
#' 
#' file.remove("./algo.json", "./descriptor.json", "./data.json")
#' 
#' resPredict <- JsonMixtCompPredict(data, descriptor, algo = strategy, nClass = 2,
#'                                   confidenceLevel = 0.95, inputPath = ".", 
#'                                   paramFile = "reslearn.json", outputFile = "respredict.json")
#' 
#' 
#' file.remove("./algo.json", "./descriptor.json", "./data.json", "reslearn.json",
#'             "respredict.json", "progress")
#' }
#' 
#' 
#' @keywords package
NULL