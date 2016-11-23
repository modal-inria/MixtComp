#'
#' @title Get the completed data from MixtComp object
#'
#' @description Get the completed data from MixtComp object
#'
#' @param outMixtComp output object of \link{mixtCompCluster} or \link{mixtCompPredict} fucntions.
#' @param with.z_class if TRUE, z_class is returned with the data.
#'
#' @return  a matrix with the data completed by MixtComp (z_class is in the first column and then variables are sorted in alphabetic order, it may differ from the original order of the data).
#'
#' @examples 
#' \dontrun{
#' # load the data
#' resGetData <- getData(c("dataTrain.csv", "descriptor.csv")) 
#' resGetNewData <- getData(c("dataTest.csv", "descriptor.csv")) 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#' nbIter = 100,
#' nbGibbsBurnInIter = 50,
#' nbGibbsIter = 50,
#' parameterEdgeAuthorized = FALSE)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # get completedData
#' completedData <- completed(res)
#' }
#'
#' @export
#'
completed <- function(outMixtcomp, with.z_class = FALSE)
{
  completedData <- do.call(cbind, lapply(outMixtcomp$variable$data, function(x){x$completed}))
  
  if(!with.z_class)
    completedData = completedData[,-1]
  
  return(as.data.frame(completedData))
}


#'
#' @title Get the estimated class from MixtComp object
#'
#' @description Get the estimated class from MixtComp object
#'
#' @param outMixtComp output object of \link{mixtCompCluster} or \link{mixtCompPredict} fucntions.
#'
#' @return a vector containing the estimated class for each individual.
#'
#' @examples 
#' \dontrun{
#' # load the data
#' resGetData <- getData(c("dataTrain.csv", "descriptor.csv")) 
#' resGetNewData <- getData(c("dataTest.csv", "descriptor.csv")) 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#' nbIter = 100,
#' nbGibbsBurnInIter = 50,
#' nbGibbsIter = 50,
#' parameterEdgeAuthorized = FALSE)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # get class
#' estiamtedClass <- getZ_class(res)
#' }
#'
#' @export
#'
getZ_class <- function(outMixtcomp)
{
  return(outMixtcomp$variable$data$z_class$completed)
}


#'
#' @title Get the type of model for each variable
#'
#' @description Get the type of model for each variable
#'
#' @param outMixtComp output object of \link{mixtCompCluster} or \link{mixtCompPredict} fucntions.
#' @param with.z_class if TRUE, the type of z_class is returned.
#'
#' @return a vector containing the type of models associated with each individual.
#'
#' @examples 
#' \dontrun{
#' # load the data
#' resGetData <- getData(c("dataTrain.csv", "descriptor.csv")) 
#' resGetNewData <- getData(c("dataTest.csv", "descriptor.csv")) 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#' nbIter = 100,
#' nbGibbsBurnInIter = 50,
#' nbGibbsIter = 50,
#' parameterEdgeAuthorized = FALSE)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # get type
#' type <- getType(res)
#' }
#'
#' @export
#'
getType <- function(outMixtcomp, with.z_class = FALSE)
{
  type <- unlist(outMixtcomp$variable$type)
  
  if(!with.z_class)
    type = type[-1]
  
  return(type)
}