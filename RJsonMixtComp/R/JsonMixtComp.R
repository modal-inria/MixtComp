# getData version json ----------------------------------------------------


#' @title Create a json file for JsonMixtComp
#' 
#' @description Create a json file containing the data and options required by JsonMixtComp.
#' 
#' @param outputJsonFile \emph{lm} output of \link{getData} function
#' @param dataList output of getData function.
#' @param nbClass the number of class of the mixture model.
#' @param confidenceLevel quantile for confidence interval of estimated parameters.
#' @param mcStrategy a list containing the parameters of the SEM-Gibbs algorithm (see \emph{Details}).
#' @param mode "learn" or "predict". Mode used in JsonMixtComp.
#' @param outMixtCompFile (only if mode = "predict"). Output json file of JsonMixtComp in learn mode.
#' 
#' @return a json file to use with JsonMixtComp.
#' 
#' @export
createJsonMixtCompFile <- function(outputJsonFile, dataList, nbClass, confidenceLevel = 0.95, mcStrategy = list(nbBurnInIter = 100, 
                                                                                                                nbIter = 100, 
                                                                                                                nbGibbsBurnInIter = 100, 
                                                                                                                nbGibbsIter = 100, 
                                                                                                                parameterEdgeAuthorized = TRUE), mode = c("learn", "predict"), outMixtCompFile = NULL)
{
  
  mode = match.arg(mode)
  if(is.null(outMixtCompFile) & mode == "predict")
    stop("For predict, param is required")
  
  # unbox transform a vector of length 1 in a non vector object in json
  # data must not be unbox when there is only one sample
  # instead of using auto_unbox = TRUE, we manually precise what needs to be unboxed
  for(i in 1:length(dataList))
  {
    dataList[[i]]$data <- as.character(dataList[[i]]$data)
    dataList[[i]]$model <- jsonlite::unbox(dataList[[i]]$model)
    dataList[[i]]$id <- jsonlite::unbox(dataList[[i]]$id)
    dataList[[i]]$paramStr <- jsonlite::unbox(dataList[[i]]$paramStr)
  }
  
  if(is.null(mcStrategy$parameterEdgeAuthorized))
    mcStrategy$parameterEdgeAuthorized = FALSE
  
  mcStrategy$nbBurnInIter = jsonlite::unbox(mcStrategy$nbBurnInIter)
  mcStrategy$nbIter = jsonlite::unbox(mcStrategy$nbIter)
  mcStrategy$nbGibbsBurnInIter = jsonlite::unbox(mcStrategy$nbGibbsBurnInIter)
  mcStrategy$nbGibbsIter = jsonlite::unbox(mcStrategy$nbGibbsIte)
  mcStrategy$parameterEdgeAuthorized = jsonlite::unbox(mcStrategy$parameterEdgeAuthorized)
  
  if(mode == "learn")
    arg_list_json <- toJSON(list(by_row = FALSE, resGetData_lm = dataList, mcStrategy = mcStrategy, nbClass = nbClass, confidenceLevel = confidenceLevel, mode = mode), auto_unbox = TRUE)
  
  if(mode == "predict")
    arg_list_json <- toJSON(list(by_row = jsonlite::unbox(FALSE), resGetData_lm = dataList, mcStrategy = mcStrategy, nbClass = jsonlite::unbox(nbClass),
                                 confidenceLevel = jsonlite::unbox(confidenceLevel), pathParamList = jsonlite::unbox(outMixtCompFile), mode = jsonlite::unbox(mode)))
  
  
  write(x = arg_list_json, outputJsonFile)
  
  return(outputJsonFile)
}



# mixtComp cluster --------------------------------------------------------

#' @export
JsonMixtCompCluster <- function(dataList, mcStrategy, nbClass, confidenceLevel, jsonInputFile, jsonOutputFile)
{
  checkModel(sapply(dataList, function(x){x$model}))
  checkPath(jsonInputFile, jsonOutputFile)
  
  createJsonMixtCompFile(jsonInputFile, dataList, nbClass, confidenceLevel, mcStrategy, mode = "learn")
  
  nameExe <- ifelse(Sys.info()["sysname"] == "Windows", "JsonMixtComp.exe", "JsonMixtComp")
  pathToJsonMixtComp <- system.file("exeMixtComp", nameExe, package = "RJsonMixtComp")
  
  system(paste(pathToJsonMixtComp, jsonInputFile, jsonOutputFile))
  
  resLearn <- fromJSON(jsonOutputFile)
  
  resLearn = convertJsonRobject(resLearn, confidenceLevel, mode = "learn")
  
  return(resLearn)
}



# mixtComp predict --------------------------------------------------------


#' @name JsonMixtCompCluster
#' @aliases JsonMixtCompPredict
#'
#' @usage JsonMixtCompCluster(dataList, mcStrategy, nbClass, confidenceLevel, jsonInputFile,
#'                     jsonOutputFile)
#' JsonMixtCompPredict(dataList, mcStrategy, nbClass, confidenceLevel, jsonInputFile, 
#'                     jsonOutputFile, jsonMixtCompLearnFile)
#'
#' @title Learn and predict using RJsonMixtComp
#' 
#' @description Estimate the parameter of a mixture model or predict the cluster of new samples.
#' 
#' 
#' @param dataList \emph{lm} output of \link{getData} function.
#' @param mcStrategy a list containing the parameters of the SEM-Gibbs algorithm (see \emph{Details}).
#' @param nbClass the number of class of the mixture model.
#' @param confidenceLevel quantile for confidence interval of estimated parameters.
#' @param jsonMixtCompLearnFile (only for JsonMixtCompPredict) path of the output json file of JsonMixtCompLearn function.
#' @param jsonInputFile path of the input json file to save.
#' @param jsonOutputFile path of the output json file to save.
#' 
#' @return A json file and a R list containing 3 lists :
#' \describe{
#'  \item{strategy}{a copy of \emph{mcStrategy} parameter.}
#'  \item{mixture}{information about the mixture (see \emph{Details}).}
#'  \item{variable}{information about the estimated parameters (see \emph{Details}).}
#' }
#' 
#' @details Details about the output object of \emph{mixtCompCluster} and \emph{mixtCompPredict} functions.
#' 
#' The path for outputs files must not contain ":" or "~".
#' 
#' Rank data and ordinal data are currently not working.
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
#' @seealso \link{getData} 
#' @export
JsonMixtCompPredict <- function(dataList, mcStrategy, nbClass, confidenceLevel, jsonInputFile, jsonOutputFile, jsonMixtCompLearnFile)
{
  checkModel(sapply(dataList, function(x){x$model}))
  checkPath(jsonInputFile, jsonOutputFile, jsonMixtCompLearnFile)
  
  createJsonMixtCompFile(jsonInputFile, dataList, nbClass, confidenceLevel, mcStrategy, mode = "predict", outMixtCompFile = jsonMixtCompLearnFile)
  
  
  nameExe <- ifelse(Sys.info()["sysname"] == "Windows", "JsonMixtComp.exe", "JsonMixtComp")
  pathToJsonMixtComp <- system.file("exeMixtComp", nameExe, package = "RJsonMixtComp")
  
  system(paste(pathToJsonMixtComp, jsonInputFile, jsonOutputFile))
  
  resPredict <- fromJSON(jsonOutputFile)
  
  resPredict = convertJsonRobject(resPredict, confidenceLevel, mode = "predict")
  
  return(resPredict)
}

