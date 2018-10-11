#' @title Learn and predict using RJsonMixtComp
#' 
#' @description Estimate the parameter of a mixture model or predict the cluster of new samples.
#' 
#' 
#' @param data a data.frame containing the data
#' @param descriptor a data.frame containing the models for each variable. The colnames of \emph{descriptor} must be in \emph{data}
#' @param nClass the number of class of the mixture model.
#' @param algo a list containing the parameters of the SEM-Gibbs algorithm (see \emph{Details}).
#' @param confidenceLevel quantile for confidence interval of estimated parameters.
#' @param inputPath path of the folder to save the input files.
#' @param outputFile path of the output json file to save.
#' @param paramFile (only for JsonMixtCompPredict) path of the output json file of JsonMixtCompCluster function.
#' 
#' @return A json file and a R list containing 3 lists :
#' \describe{
#'  \item{strategy}{a copy of \emph{algo} parameter.}
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
#' 
#' strategy <- list(nbBurnInIter = 50, nbIter = 50, nbGibbsBurnInIter = 20,
#'                  nbGibbsIter = 20, nInitPerClass = 10, nSemTry = 5,
#'                  confidenceLevel = 0.95)
#' 
#' resLearn <- JsonMixtCompLearn(data, descriptor, algo = strategy, nClass = 2,
#'                               inputPath = ".", outputFile = "reslearn.json")
#' 
#' 
#' file.remove("./algo.json", "./descriptor.json", "./data.json")
#' 
#' resPredict <- JsonMixtCompPredict(data, descriptor, algo = strategy, nClass = 2,
#'                                   inputPath = ".", paramFile = "reslearn.json",
#'                                   outputFile = "respredict.json")
#' 
#' 
#' file.remove("./algo.json", "./descriptor.json", "./data.json", "reslearn.json",
#'             "respredict.json", "progress")
#' }
#' 
#' 
#' @export
JsonMixtCompLearn <- function(data, descriptor, algo = list(nbBurnInIter = 100,
                                                            nbIter = 100,
                                                            nbGibbsBurnInIter = 50,
                                                            nbGibbsIter = 50,
                                                            nInitPerClass = 10,
                                                            nSemTry = 10, 
                                                            confidenceLevel = 0.95), nClass, inputPath, outputFile)
{
  
  algoFile <- paste0(inputPath,"/algo.json")
  write(createAlgoJson(nClass, nInd = nrow(data), algo, mode = "learn"), algoFile)
  
  dataFile <- paste0(inputPath,"/data.json")
  write(createDataJson(data), dataFile)
  
  descriptorFile <- paste0(inputPath,"/descriptor.json")
  write(createDescriptorJson(descriptor), descriptorFile)
  
  
  nameExe <- ifelse(Sys.info()["sysname"] == "Windows", "jmc.exe", "jmc")
  pathToJMixtComp <- system.file("exeMixtComp", nameExe, package = "RJsonMixtComp")
  
  system(paste(pathToJMixtComp, algoFile, dataFile, descriptorFile, outputFile))
  
  resLearn <- fromJSON(outputFile)
  
  resLearn = convertOutput(resLearn)
  class(resLearn) = "MixtComp"
  
  return(resLearn)
}


#' @rdname JsonMixtCompLearn
#' @export
JsonMixtCompPredict <- function(data, descriptor, algo = list(nbBurnInIter = 100,
                                                              nbIter = 100,
                                                              nbGibbsBurnInIter = 50,
                                                              nbGibbsIter = 50,
                                                              nInitPerClass = 10,
                                                              nSemTry = 10, 
                                                              confidenceLevel = 0.95), nClass, inputPath, paramFile, outputFile)
{
  algoFile <- paste0(inputPath,"/algo.json")
  write(createAlgoJson(nClass, nInd = nrow(data), algo, mode = "predict"), algoFile)
  
  dataFile <- paste0(inputPath,"/data.json")
  write(createDataJson(data), dataFile)
  
  descriptorFile <- paste0(inputPath,"/descriptor.json")
  write(createDescriptorJson(descriptor), descriptorFile)
  
  nameExe <- ifelse(Sys.info()["sysname"] == "Windows", "jmc.exe", "jmc")
  pathToJMixtComp <- system.file("exeMixtComp", nameExe, package = "RJsonMixtComp")
  
  system(paste(pathToJMixtComp, algoFile, dataFile, descriptorFile, paramFile, outputFile))
  
  resPredict <- fromJSON(outputFile)
  
  resPredict = convertOutput(resPredict)
  class(resPredict) = "MixtComp"
  
  return(resPredict)
}

