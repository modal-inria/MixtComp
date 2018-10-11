# packages to import for the NAMESPACE file
#' @import jsonlite
#' 

#' @title RJsonMixtComp
#' @docType package
#' @aliases RJsonMixtComp-package
#' @name RJsonMixtComp-package
#' @description  
#' Statistics analysis with MixtComp
#'
#' 
#' @details
#' Use JsonMixtComp
#' 
#' 
#' @author Quentin Grimonprez
#' 
#' Maintainer: Quentin Grimonprez <quentin.grimonprez@@inria.fr>
#'  
#' 
#' @examples 
#' \donttest{
#' pathToData <- system.file("extdata", "data.json", package = "RJsonMixtComp")
#' pathToDescriptor <- system.file("extdata", "desc.json", package = "RJsonMixtComp")
#' 
#' data <- as.data.frame(fromJSON(pathToData))
#' descriptor <- as.data.frame(lapply(fromJSON(pathToDescriptor), unlist))
#' strategy <- list(nbBurnInIter = 50, nbIter = 50, nbGibbsBurnInIter = 20,
#'                  nbGibbsIter = 20, nInitPerClass = 10, nSemTry = 5)
#' 
#' resLearn <- JsonMixtCompLearn(data, descriptor, nClass = 2, mcStrategy = strategy,
#'                               confidenceLevel = 0.95, inputPath = ".", outputFile = "reslearn.json")
#' 
#' 
#' file.remove("./algo.json", "./descriptor.json", "./data.json")
#' 
#' resPredict <- JsonMixtCompPredict(data, descriptor, nClass = 2, mcStrategy = strategy,
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