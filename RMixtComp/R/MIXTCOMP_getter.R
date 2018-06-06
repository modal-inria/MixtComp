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
#' # path to files
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RMixtComp")
#' 
#' resGetData <- getData(c(pathToData, pathToDescriptor))
#' 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#'                    nbIter = 100,
#'                    nbGibbsBurnInIter = 50,
#'                    nbGibbsIter = 50,
#'                    ratioInitialization = 0.1)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # get completedData
#' getCompletedData <- completed(res)
#' }
getCompletedData <- function(outMixtComp, with.z_class = FALSE)
{
  completedData <- do.call(cbind, lapply(outMixtComp$variable$data, function(x){x$completed}))
  
  if(!with.z_class)
    completedData = completedData[,-which(colnames(completedData)=="z_class"), drop = FALSE]
  
  return(as.data.frame(completedData))
}



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
#' # path to files
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RMixtComp")
#' 
#' resGetData <- getData(c(pathToData, pathToDescriptor))
#' 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#'                    nbIter = 100,
#'                    nbGibbsBurnInIter = 50,
#'                    nbGibbsIter = 50,
#'                    ratioInitialization = 0.1)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # get class
#' estimatedClass <- getZ_class(res)
#' }
getZ_class <- function(outMixtComp)
{
  return(outMixtComp$variable$data$z_class$completed)
}


#' @name getType
#' @aliases getVarNames
#' 
#' @title Getters
#'
#' @description Get the type of model, names for each variable
#'
#' @usage getType(outMixtComp, with.z_class = FALSE)
#' getVarNames(outMixtComp, with.z_class = FALSE)
#'
#' @param outMixtComp output object of \link{mixtCompCluster} or \link{mixtCompPredict} fucntions.
#' @param with.z_class if TRUE, the type of z_class is returned.
#'
#' @return a vector containing the type of models, names associated with each individual.
#'
#' @examples 
#' \dontrun{
#' # path to files
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RMixtComp")
#' 
#' resGetData <- getData(c(pathToData, pathToDescriptor))
#' 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#'                    nbIter = 100,
#'                    nbGibbsBurnInIter = 50,
#'                    nbGibbsIter = 50,
#'                    ratioInitialization = 0.1)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # get type
#' type <- getType(res)
#' 
#' # get variable names
#' varNames <- getVarNames(res)
#' 
#' }
getType <- function(outMixtComp, with.z_class = FALSE)
{
  type <- unlist(outMixtComp$variable$type)
  
  if(!with.z_class)
    type = type[-which(names(type)=="z_class")]
  
  return(type)
}

#' @title Get the empiric tik
#'
#' @description Get the empiric tik
#'
#' @param outMixtComp output object of \link{mixtCompCluster} or \link{mixtCompPredict} fucntions.
#'
#' @return a matrix containing the tik for each individuals (in row) and each class (in column).
#'
#' @examples 
#' \dontrun{
#' # path to files
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RMixtComp")
#' 
#' resGetData <- getData(c(pathToData, pathToDescriptor))
#' 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#'                    nbIter = 100,
#'                    nbGibbsBurnInIter = 50,
#'                    nbGibbsIter = 50,
#'                    ratioInitialization = 0.1)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # get tik
#' tik <- getTik(res)
#' }
getTik <- function(outMixtComp)
{
  return(outMixtComp$variable$data$z_class$stat)
}



#' @title Get the estimated parameter
#'
#' @description Get the estimated parameter
#'
#' @param varName name of the variable to get parameter
#' @param outMixtComp output object of \link{mixtCompCluster} or \link{mixtCompPredict} fucntions.
#'
#' @return the parameter of the variable
#'
#' @examples 
#' \dontrun{
#' # path to files
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RMixtComp")
#' 
#' resGetData <- getData(c(pathToData, pathToDescriptor))
#' 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#'                    nbIter = 100,
#'                    nbGibbsBurnInIter = 50,
#'                    nbGibbsIter = 50,
#'                    ratioInitialization = 0.1)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # get param
#' param <- getParam("poisson1", res)
#' }
getParam <- function(varName, outMixtComp)
{
  type <- outMixtComp$variable$type[[varName]]
  
  if(is.null(type))
  {
    warning("Bad variable name.")
    return(c())
  }
    
  nbClass <- outMixtComp$mixture$nbCluster
  
  param <- switch(type,
                  "Ordinal" = outMixtComp$variable$param[[varName]]$muPi$stat[,1],
                  "Categorical_pjk" = {
                    nbModalities <- length(outMixtComp$variable$param[[varName]]$NumericalParam$stat[,1])/nbClass
                    matrix(outMixtComp$variable$param[[varName]]$NumericalParam$stat[,1], nrow = nbClass, byrow = TRUE, dimnames = list(paste0("k:",1:nbClass), paste0("modality ",1:nbModalities)))
                    },
                  "Gaussian_sjk" = matrix(outMixtComp$variable$param[[varName]]$NumericalParam$stat[,1], nrow = nbClass, byrow = TRUE, dimnames = list(paste0("k:",1:nbClass), c("mean", "sd"))),
                  "Poisson_k" = outMixtComp$variable$param[[varName]]$NumericalParam$stat[,1],
                  "Rank" = list(pi = outMixtComp$variable$param[[varName]]$pi$stat, mu = outMixtComp$variable$param[[varName]]$mu$stat),
                  "Functional" = list(alpha = outMixtComp$variable$param[[varName]]$alpha$stat[,1],
                                      beta = outMixtComp$variable$param[[varName]]$beta$stat[,1],
                                      sd = outMixtComp$variable$param[[varName]]$sd$stat[,1]),
                  "LatentClass" = outMixtComp$variable$param[[varName]]$pi$stat[,1],
                  "Weibull" = matrix(outMixtComp$variable$param[[varName]]$NumericalParam$stat[,1], nrow = nbClass, byrow = TRUE, dimnames = list(paste0("k:",1:nbClass), c("k (shape)", "lambda (scale)"))),
                  warning("Not yet implemented."))
  
  return(param)
}


#' @name getBIC
#' @aliases getICL
#' 
#' @usage getBIC(outMixtComp)
#' getICL(outMixtComp)
#' 
#' @title Get criterion value
#'
#' @description Get criterion value
#'
#' @param outMixtComp output object of \link{mixtCompCluster} or \link{mixtCompPredict} fucntions.
#'
#' @return value of the criterion
#'
#' @examples 
#' \dontrun{
#' # path to files
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RMixtComp")
#' 
#' resGetData <- getData(c(pathToData, pathToDescriptor))
#' 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#'                    nbIter = 100,
#'                    nbGibbsBurnInIter = 50,
#'                    nbGibbsIter = 50,
#'                    ratioInitialization = 0.1)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # get criterion
#' bic <- getBIC(res)
#' icl <- getICL(res)
#' }
getBIC <- function(outMixtComp)
{
  if(is.null(outMixtComp$mixture$BIC))
    return(NaN)
  
  return(outMixtComp$mixture$BIC)
}

getICL <- function(outMixtComp)
{
  if(is.null(outMixtComp$mixture$ICL))
    return(NaN)
  
  return(outMixtComp$mixture$ICL)
}

