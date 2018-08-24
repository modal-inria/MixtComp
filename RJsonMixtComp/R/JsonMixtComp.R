
#' @export
JsonMixtCompLearn <- function(data, descriptor, nClass, mcStrategy = list(nbBurnInIter = 100,
                                                                          nbIter = 100,
                                                                          nbGibbsBurnInIter = 50,
                                                                          nbGibbsIter = 50,
                                                                          nInitPerClass = 10,
                                                                          nSemTry = 10), confidenceLevel = 0.95, inputPath, outputFile)
{
  
  algoFile <- paste0(inputPath,"/algo.json")
  write(createAlgoJson(nClass, nInd = nrow(data), mcStrategy, confidenceLevel, mode = "learn"), algoFile)
  
  dataFile <- paste0(inputPath,"/data.json")
  write(createDataJson(data), dataFile)
  
  descriptorFile <- paste0(inputPath,"/descriptor.json")
  write(createDescriptorJson(descriptor), descriptorFile)
  

  nameExe <- ifelse(Sys.info()["sysname"] == "Windows", "jmc.exe", "jmc")
  pathToJMixtComp <- system.file("exeMixtComp", nameExe, package = "RJsonMixtComp")
  
  system(paste(pathToJMixtComp, algoFile, dataFile, descriptorFile, outputFile))
  
  resLearn <- fromJSON(outputFile)
  
  return(resLearn)
}


#' @name JsonMixtCompLearn
#' @aliases JsonMixtCompPredict
#'
#' @usage JsonMixtCompLearn(data, descriptor, nClass, mcStrategy, confidenceLevel, inputPath,
#'  outputFile)
#' JsonMixtCompPredict(data, descriptor, nClass, mcStrategy, confidenceLevel, inputPath,
#'  paramFile, outputFile)
#'
#' @title Learn and predict using RJsonMixtComp
#' 
#' @description Estimate the parameter of a mixture model or predict the cluster of new samples.
#' 
#' 
#' @param data a data.frame containing the data
#' @param descriptor a data.frame containing the models for each variable. The colnames of \emph{descriptor} must be in \emph{data}
#' @param nClass the number of class of the mixture model.
#' @param mcStrategy a list containing the parameters of the SEM-Gibbs algorithm (see \emph{Details}).
#' @param confidenceLevel quantile for confidence interval of estimated parameters.
#' @param inputPath path of the folder to save the input files.
#' @param outputFile path of the output json file to save.
#' @param paramFile (only for JsonMixtCompPredict) path of the output json file of JsonMixtCompCluster function.
#' 
#' @return A json file and a R list containing 3 lists :
#' \describe{
#'  \item{strategy}{a copy of \emph{mcStrategy} parameter.}
#'  \item{mixture}{information about the mixture (see \emph{Details}).}
#'  \item{variable}{information about the estimated parameters (see \emph{Details}).}
#' }
#' 
#' @details See external documentation for details about the input/output object of \emph{mixtCompLearn} and \emph{mixtCompPredict} functions.
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
#' @export
JsonMixtCompPredict <- function(data, descriptor, nClass, mcStrategy = list(nbBurnInIter = 100,
                                                                            nbIter = 100,
                                                                            nbGibbsBurnInIter = 50,
                                                                            nbGibbsIter = 50,
                                                                            nInitPerClass = 10,
                                                                            nSemTry = 10), confidenceLevel = 0.95, inputPath, paramFile, outputFile)
{
  algoFile <- paste0(inputPath,"/algo.json")
  write(createAlgoJson(nClass, nInd = nrow(data), mcStrategy, confidenceLevel, mode = "predict"), algoFile)
  
  dataFile <- paste0(inputPath,"/data.json")
  write(createDataJson(data), dataFile)
  
  descriptorFile <- paste0(inputPath,"/descriptor.json")
  write(createDescriptorJson(descriptor), descriptorFile)

  nameExe <- ifelse(Sys.info()["sysname"] == "Windows", "jmc.exe", "jmc")
  pathToJMixtComp <- system.file("exeMixtComp", nameExe, package = "RJsonMixtComp")
  
  system(paste(pathToJMixtComp, algoFile, dataFile, descriptorFile, paramFile, outputFile))

  resPredict <- fromJSON(outputFile)

  return(resPredict)
}

