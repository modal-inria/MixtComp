# MixtComp version 4 - july 2019
# Copyright (C) Inria - Université de Lille - CNRS 

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# 
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>
 

#' @title Get the completed data from MixtComp object
#'
#' @description Get the completed data from MixtComp object (does not manage functional models)
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \code{mixtCompLearn} or \code{mixtCompPredict} functions from \code{RMixtComp} package or \code{rmcMultiRun} from \code{RMixtCompIO} package.
#' @param var Name of the variables for which to extract the completed data. Default is NULL (all variables are extracted)
#' @param with.z_class if TRUE, z_class is returned with the data.
#'
#' @return  a matrix with the data completed by MixtComp (z_class is in the first column and then variables are sorted in alphabetic order, it may differ from the original order of the data).
#'
#' @examples 
#' require(RMixtCompIO) # for learning a mixture model
#' dataLearn <- list(var1 = as.character(c(rnorm(50, -2, 0.8), rnorm(50, 2, 0.8))),
#'                   var2 = as.character(c(rnorm(50, 2), rpois(50, 8))))
#'                   
#' # add missing values
#' dataLearn$var1[12] = "?"
#' dataLearn$var2[72] = "?"
#'                   
#' model <- list(var1 = list(type = "Gaussian", paramStr = ""),
#'               var2 = list(type = "Poisson", paramStr = ""))
#' 
#' algo <- list(
#'   nClass = 2,
#'   nInd = 100,
#'   nbBurnInIter = 100,
#'   nbIter = 100,
#'   nbGibbsBurnInIter = 100,
#'   nbGibbsIter = 100,
#'   nInitPerClass = 3,
#'   nSemTry = 20,
#'   confidenceLevel = 0.95,
#'   ratioStableCriterion = 0.95,
#'   nStableCriterion = 10,
#'   mode = "learn"
#' )
#' 
#' resLearn <- rmcMultiRun(algo, dataLearn, model, nRun = 3)
#' 
#' # get completedData
#' completedData <- getCompletedData(resLearn)
#' completedData2 <- getCompletedData(resLearn, var = "var1")
#' 
#' @author Quentin Grimonprez
#' @family getter
#' @export
getCompletedData <- function(outMixtComp, var = NULL, with.z_class = FALSE)
{
  if(is.null(var))
    var = getVarNames(outMixtComp, with.z_class)
  
  mcVar <- getVarNames(outMixtComp, with.z_class = TRUE)
  if(any(!(var %in% mcVar)))
    stop("some elements of var are not in outMixtComp")
  
  completedData <- do.call(cbind, lapply(var, 
                                         function(x){
                                           if("completed" %in% names(outMixtComp$variable$data[[x]]))
                                           {
                                             if(outMixtComp$variable$type[[x]] != "Rank_ISR")
                                             {
                                               df <- data.frame(outMixtComp$variable$data[[x]]$completed)
                                             }else{
                                               df <- data.frame(apply(outMixtComp$variable$data[[x]]$completed, 1, function(x) {paste(x, collapse = ",")}))
                                             }
                                             names(df) = x
                                             
                                             return(df)
                                           }
                                           else
                                             return(data.frame()[1:outMixtComp$algo$nInd, ])
                                           }))
  
  rownames(completedData) = NULL

  return(completedData)
}



#' @title Get the estimated class from MixtComp object
#'
#' @description Get the estimated class from MixtComp object
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \code{mixtCompLearn} or \code{mixtCompPredict} functions from \code{RMixtComp} package or \code{rmcMultiRun} from \code{RMixtCompIO} package.
#' @param empiric if TRUE, use the partition obtained at the end of the gibbs algorithm. If FALSE, use the partition obtained with the observed probabilities. 
#'
#' @return a vector containing the estimated class for each individual.
#'
#' @examples 
#' require(RMixtCompIO) # for learning a mixture model
#' dataLearn <- list(var1 = as.character(c(rnorm(50, -2, 0.8), rnorm(50, 2, 0.8))),
#'                   var2 = as.character(c(rnorm(50, 2), rpois(50, 8))))
#'                   
#' model <- list(var1 = list(type = "Gaussian", paramStr = ""),
#'               var2 = list(type = "Poisson", paramStr = ""))
#' 
#' algo <- list(
#'   nClass = 2,
#'   nInd = 100,
#'   nbBurnInIter = 100,
#'   nbIter = 100,
#'   nbGibbsBurnInIter = 100,
#'   nbGibbsIter = 100,
#'   nInitPerClass = 3,
#'   nSemTry = 20,
#'   confidenceLevel = 0.95,
#'   ratioStableCriterion = 0.95,
#'   nStableCriterion = 10,
#'   mode = "learn"
#' )
#' 
#' resLearn <- rmcMultiRun(algo, dataLearn, model, nRun = 3)
#' 
#' # get class
#' estimatedClass <- getPartition(resLearn)
#' 
#' @author Quentin Grimonprez
#' @family getter
#' @export
getPartition <- function(outMixtComp, empiric = TRUE)
{
  if(empiric)
    return(outMixtComp$variable$data$z_class$completed)
  
  
  tik <- getTik(outMixtComp)
  tik[is.na(tik)] = -Inf
  part <- apply(tik, 1, which.max)
  
  return(part)
}


#' @name getType
#' 
#' @title Names and Types Getters
#'
#' @description getType returns the type output of a MixtComp object, getModel returns the model object, getVarNames returns the name for each variable
#'
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \code{mixtCompLearn} or \code{mixtCompPredict} functions from \code{RMixtComp} package or \code{rmcMultiRun} from \code{RMixtCompIO} package.
#' @param with.z_class if TRUE, the type of z_class is returned.
#'
#' @return a vector containing the type of models, names associated with each individual.
#'
#' @examples 
#' require(RMixtCompIO) # for learning a mixture model
#' dataLearn <- list(var1 = as.character(c(rnorm(50, -2, 0.8), rnorm(50, 2, 0.8))),
#'                   var2 = as.character(c(rnorm(50, 2), rpois(50, 8))))
#'                   
#' model <- list(var1 = list(type = "Gaussian", paramStr = ""),
#'               var2 = list(type = "Poisson", paramStr = ""))
#' 
#' algo <- list(
#'   nClass = 2,
#'   nInd = 100,
#'   nbBurnInIter = 100,
#'   nbIter = 100,
#'   nbGibbsBurnInIter = 100,
#'   nbGibbsIter = 100,
#'   nInitPerClass = 3,
#'   nSemTry = 20,
#'   confidenceLevel = 0.95,
#'   ratioStableCriterion = 0.95,
#'   nStableCriterion = 10,
#'   mode = "learn"
#' )
#' 
#' resLearn <- rmcMultiRun(algo, dataLearn, model, nRun = 3)
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
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \code{mixtCompLearn} or \code{mixtCompPredict} functions from \code{RMixtComp} package or \code{rmcMultiRun} from \code{RMixtCompIO} package.
#' @param log if TRUE, log(tik) are returned
#'
#' @return a matrix containing the tik for each individuals (in row) and each class (in column).
#'
#' @details 
#' \emph{getTik} returns a posteriori probabilities computed with the returned parameters. \emph{getEmpiricTik} returns an estimation based on the sampled z_i during the algorithm. 
#'
#' @examples 
#' require(RMixtCompIO) # for learning a mixture model
#' dataLearn <- list(var1 = as.character(c(rnorm(50, -2, 0.8), rnorm(50, 2, 0.8))),
#'                   var2 = as.character(c(rnorm(50, 2), rpois(50, 8))))
#'                   
#' model <- list(var1 = list(type = "Gaussian", paramStr = ""),
#'               var2 = list(type = "Poisson", paramStr = ""))
#' 
#' algo <- list(
#'   nClass = 2,
#'   nInd = 100,
#'   nbBurnInIter = 100,
#'   nbIter = 100,
#'   nbGibbsBurnInIter = 100,
#'   nbGibbsIter = 100,
#'   nInitPerClass = 3,
#'   nSemTry = 20,
#'   confidenceLevel = 0.95,
#'   ratioStableCriterion = 0.95,
#'   nStableCriterion = 10,
#'   mode = "learn"
#' )
#' 
#' resLearn <- rmcMultiRun(algo, dataLearn, model, nRun = 3)
#' 
#' # get tik
#' tikEmp <- getEmpiricTik(resLearn)
#' tik <- getTik(resLearn, log = FALSE)
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
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \code{mixtCompLearn} or \code{mixtCompPredict} functions from \code{RMixtComp} package or \code{rmcMultiRun} from \code{RMixtCompIO} package.
#'
#' @return value of the criterion
#'
#' @examples 
#' require(RMixtCompIO) # for learning a mixture model
#' dataLearn <- list(var1 = as.character(c(rnorm(50, -2, 0.8), rnorm(50, 2, 0.8))),
#'                   var2 = as.character(c(rnorm(50, 2), rpois(50, 8))))
#'                   
#' model <- list(var1 = list(type = "Gaussian", paramStr = ""),
#'               var2 = list(type = "Poisson", paramStr = ""))
#' 
#' algo <- list(
#'   nClass = 2,
#'   nInd = 100,
#'   nbBurnInIter = 100,
#'   nbIter = 100,
#'   nbGibbsBurnInIter = 100,
#'   nbGibbsIter = 100,
#'   nInitPerClass = 3,
#'   nSemTry = 20,
#'   confidenceLevel = 0.95,
#'   ratioStableCriterion = 0.95,
#'   nStableCriterion = 10,
#'   mode = "learn"
#' )
#' 
#' resLearn <- rmcMultiRun(algo, dataLearn, model, nRun = 3)
#' 
#' # get criterion
#' bic <- getBIC(resLearn)
#' icl <- getICL(resLearn)
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

