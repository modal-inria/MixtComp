#' @title Get the completed data from MixtComp object
#'
#' @description Get the completed data from MixtComp object
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \link{mixtCompLearn} or \link{mixtCompPredict} functions.
#' @param with.z_class if TRUE, z_class is returned with the data.
#'
#' @return  a matrix with the data completed by MixtComp (z_class is in the first column and then variables are sorted in alphabetic order, it may differ from the original order of the data).
#'
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- list(nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' #' # keep only 3 variables
#' model <- simData$model$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")]
#' 
#' # run RMixtCompt for clustering
#' resLearn <- mixtCompLearn(simData$dataLearn$matrix, model, algo, nClass = 2)
#' 
#' # get completedData
#' getCompletedData <- completed(resLearn)
#' }
#' 
#' @author Quentin Grimonprez
#' @family getter
#' @export
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
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \link{mixtCompLearn} or \link{mixtCompPredict} functions.
#'
#' @return a vector containing the estimated class for each individual.
#'
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- list(nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # keep only 3 variables
#' model <- simData$model$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")]
#' 
#' # run RMixtCompt for clustering
#' resLearn <- mixtCompLearn(simData$dataLearn$matrix, model, algo, nClass = 2)
#' 
#' # get class
#' estimatedClass <- getPartition(resLearn)
#' }
#' 
#' @author Quentin Grimonprez
#' @family getter
#' @export
getPartition <- function(outMixtComp)
{
  return(outMixtComp$variable$data$z_class$completed)
}


#' @name getType
#' 
#' @title Names and Types Getters
#'
#' @description getType returns the type output of a MixtComp object, getModel returns the model object, getVarNames returns the name for each variable
#'
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \link{mixtCompLearn} or \link{mixtCompPredict} functions.
#' @param with.z_class if TRUE, the type of z_class is returned.
#'
#' @return a vector containing the type of models, names associated with each individual.
#'
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- list(nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # run RMixtCompt for clustering
#' resLearn <- mixtCompLearn(simData$dataLearn$matrix, simData$model$unsupervised, algo, nClass = 2)
#' 
#' # get type
#' type <- getType(resLearn)
#' 
#' # get model object
#' model <- getModel(resLearn)
#' 
#' # get variable names
#' varNames <- getVarNames(resLearn)
#' 
#' }
#' 
#' @author Quentin Grimonprez
#' @family getter
#' @export
getType <- function(outMixtComp, with.z_class = FALSE)
{
  type <- unlist(outMixtComp$variable$type)
  
  if(!with.z_class)
    type = type[-which(names(type)=="z_class")]
  
  return(type)
}

#' @rdname getType
#' @export
getModel <- function(outMixtComp, with.z_class = FALSE)
{
  model <- outMixtComp$variable$type
  
  if(!with.z_class)
    model = model[-which(names(model)=="z_class")]
  
  for(varName in names(model))
    model[[varName]] = list(type = model[[varName]], paramStr = outMixtComp$variable$param[[varName]]$paramStr)
      
  return(model)
}


#' @rdname getType
#' @export
getVarNames <- function(outMixtComp, with.z_class = FALSE)
{
  varNames <- names(outMixtComp$variable$type)
  
  if(!with.z_class)
    varNames = varNames[varNames!="z_class"]
  
  return(varNames)
}

#' @title Get the tik
#'
#' @description Get the a posteriori probability to belong to each class for each individual
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \link{mixtCompLearn} or \link{mixtCompPredict} functions.
#' @param log if TRUE, log(tik) are returned
#'
#' @return a matrix containing the tik for each individuals (in row) and each class (in column).
#'
#' @details 
#' \emph{getTik} returns a posteriori probabilities computed with the returned parameters. \emph{getEmpiricTik} returns an estimation based on the sampled z_i during the algorithm. 
#'
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- list(nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # keep only 3 variables
#' model <- simData$model$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")]
#' 
#' # run RMixtCompt for clustering
#' resLearn <- mixtCompLearn(simData$dataLearn$matrix, model, algo, nClass = 2)
#' 
#' # get tik
#' tikEmp <- getEmpiricTik(resLearn)
#' tik <- getTik(resLearn, log = FALSE)
#' }
#' 
#' @seealso \code{\link{heatmapTikSorted}}
#' 
#' @author Quentin Grimonprez
#' @family getter
#' @export
getEmpiricTik <- function(outMixtComp)
{
  return(outMixtComp$variable$data$z_class$stat)
}

#' @rdname getEmpiricTik
#' @export
getTik <- function(outMixtComp, log = TRUE){
  logTik <- sweep(outMixtComp$mixture$lnProbaGivenClass, 
                  1, apply(outMixtComp$mixture$lnProbaGivenClass, 1, function(vec) (max(vec) + log(sum(exp(vec - max(vec)))))),
                  "-")
  if(!log)
    return(exp(logTik))
  
  return(logTik)
}




#' @name getBIC
#' 
#' 
#' @title Get criterion value
#'
#' @description Get criterion value
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \link{mixtCompLearn} or \link{mixtCompPredict} functions.
#'
#' @return value of the criterion
#'
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- list(nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # keep only 3 variables
#' model <- simData$model$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")] 
#' 
#' # run RMixtCompt for clustering
#' resLearn <- mixtCompLearn(simData$dataLearn$matrix, model, algo, nClass = 2)
#' 
#' # get criterion
#' bic <- getBIC(resLearn)
#' icl <- getICL(resLearn)
#' }
#' 
#' @author Quentin Grimonprez
#' @family getter
#' @export
getBIC <- function(outMixtComp)
{
  if(is.null(outMixtComp$mixture$BIC))
    return(NaN)
  
  return(outMixtComp$mixture$BIC)
}

#' @rdname getBIC
#' @export
getICL <- function(outMixtComp)
{
  if(is.null(outMixtComp$mixture$ICL))
    return(NaN)
  
  return(outMixtComp$mixture$ICL)
}

