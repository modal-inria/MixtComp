#' @title Learn and predict using RMixtComp
#' 
#' @description Estimate the parameter of a mixture model or predict the cluster of new samples.
#' 
#' @param data a data.frame, a matrix or a named list containing the data (see \emph{Data format} section).
#' @param desc a named list containing models and hyperparameters (see \emph{Desc format} section).
#' @param algo a list containing the parameters of the SEM-Gibbs algorithm (see \emph{Details}).
#' @param nClass the number of class of the mixture model.
#' @param resLearn output of \emph{mixtCompCluster} (only for \emph{mixtCompPredict} function).
#'
#' @return A list containing 3 lists :
#' \describe{
#'  \item{strategy}{a copy of \emph{mcStrategy} parameter.}
#'  \item{mixture}{information about the mixture (see \emph{Details}).}
#'  \item{variable}{information about the estimated parameters and completed data (see \emph{Details}).}
#' }
#' 
#' @details 
#' Details about the output object of \emph{mixtCompCluster} and \emph{mixtCompPredict} functions.
#' 
#' \emph{mcStrategy} is a list containing the different number of iterations for the algorithm. 
#' The algorithm is decomposed in a burn-in phase and a normal phase. 
#' Estimates from the burn-in phase are not shown in output.
#' \describe{
#'   \item{nbBurnInIter}{Number of iterations of the burn-in part of the SEM algorithm.}
#'   \item{nbIter}{Number of iterations of the SEM algorithm.}
#'   \item{nbGibbsBurnInIter}{Number of iterations of the burn-in part of the Gibbs algorithm.}
#'   \item{nbGibbsIter}{Number of iterations of the Gibbs algorithm.}
#'   \item{nInitPerClass}{Number of individuals used to initialize each cluster (default = 10).}
#'   \item{nSemTry}{Number of try of the algorithm for avoiding an error.}
#' }
#' You can use a void list, in this case, default values are used.
#'  
#' 
#' 
#' @section Output object:
#' The output list contains three elements \emph{mixture}, \emph{variable} and \emph{strategy}. \emph{mixture} is a list containing some criterion and some parameters. \emph{strategy} contains the parameter \emph{mcStrategy}. 
#' And \emph{variable} contains parameters and completed data.
#' 
#' The \emph{mixture} contains
#' \describe{
#'   \item{BIC}{value of BIC}
#'   \item{ICL}{value of ICL}
#'   \item{nbFreeParameters}{number of free parameters of the mixture}
#'   \item{lnObservedLikelihood}{observed loglikelihood}
#'   \item{lnCompletedLikelihood}{completed loglikelihood}
#'   \item{IDClass}{entropy used to compute the discriminative power (see code of \emph{plotDiscrimVbles})}
#'   \item{delta}{entropy used to compute the similarities between variables (see code of \emph{heatmapVbles})}
#'   \item{completedProbabilityLogBurnIn}{evolution of the completed log-probability during the burn-in period (can be used to check the convergence and determine the ideal number of iteration)}
#'   \item{completedProbabilityLogRun}{evolution of the completed log-probability  after the burn-in period (can be used to check the convergence and determine the ideal number of iteration)} 
#'   \item{runTime}{execution time in seconds} 
#'   \item{lnProbaGivenClass}{log-proportion + log-probability of x_i for each class}
#' }
#' 
#' 
#' The output list \emph{variable} contains 3 lists : \emph{data}, \emph{type} and \emph{param}. 
#' 
#' Each of these lists contains a list for each variable (the name of each list is the name of the variable) and for the class of samples (\emph{z_class}).
#' The \emph{type} list contains the model used for each variable. 
#' 
#' Each list of the \emph{data} list contains the completed data in the \emph{completed} element and some statistics about them (\emph{stat}). 
#' 
#' The estimated parameter can be found in the \emph{stat} element in the \emph{param} list (see Section \emph{View of an output object}). 
#' For more details about the parameters of each model, you can refer to \link{rnorm}, \link{rpois}, \link{rweibull}, \link{rnbinom}, \link{rmultinom}, or references in the \emph{References} section.
#' 
#' 
#'  
#' 
#' @section View of an output object:
#' Example of output object with variables named "categorical", "gaussian", "rank", "functional", "poisson", "nBinom" and "weibull" with respectively
#'  \emph{Multinomial}, \emph{Gaussian}, \emph{Rank_ISR}, \emph{Func_CS} (or \emph{Func_SharedAlpha_CS}), \emph{Poisson}, \emph{NegativeBinomial} and \emph{Weibull} as model. 
#' 
#' \tabular{lll}{
#' output  \cr
#' |_______ \tab algo \tab __ nbBurnInIter\cr
#' |        \tab      \tab |_ nbIter\cr
#' |        \tab      \tab |_ nbGibbsBurnInIter\cr
#' |        \tab      \tab |_ nbGibbsIter\cr
#' |        \tab      \tab |_ nInitPerClass\cr
#' |        \tab      \tab |_ nSemTry\cr
#' |        \tab      \tab |_ mode \cr
#' |        \tab      \tab |_ nInd  \cr
#' |        \tab      \tab |_ confidenceLevel  \cr
#' |        \tab      \tab |_ nClass \cr
#' | \cr
#' |_______ \tab mixture \tab __ BIC \cr
#' |        \tab         \tab |_ ICL\cr
#' |        \tab         \tab |_ lnCompletedLikelihood\cr
#' |        \tab         \tab |_ lnObservedLikelihood \cr
#' |        \tab         \tab |_ IDClass  \cr
#' |        \tab         \tab |_ delta  \cr
#' |        \tab         \tab |_ runTime \cr
#' |        \tab         \tab |_ nbFreeParameters \cr
#' |        \tab         \tab |_ completedProbabilityLogBurnIn \cr
#' |        \tab         \tab |_ completedProbabilityLogRun \cr
#' |        \tab         \tab |_ lnProbaGivenClass \cr
#' }
#' \tabular{llllll}{
#' |  \cr
#' |_______ \tab variable \tab __ type \tab __ z_class  \cr
#'          \tab          \tab |       \tab |_ categorical \cr
#'          \tab          \tab |       \tab |_ gaussian \cr
#'          \tab          \tab |       \tab |_ ...   \cr
#'          \tab          \tab |       \tab \cr
#'          \tab          \tab |_ data \tab __ z_class \tab __ completed\cr
#'          \tab          \tab |       \tab |          \tab |_ stat \cr
#'          \tab          \tab |       \tab |_ categorical \tab __ completed\cr
#'          \tab          \tab |       \tab |              \tab |_ stat \cr
#'          \tab          \tab |       \tab |_ ...         \tab \cr
#'          \tab          \tab |       \tab |_ functional \tab __ data\cr
#'          \tab          \tab |       \tab               \tab |_ time \cr
#'          \tab          \tab |       \tab \cr
#'          \tab          \tab |_ param \tab __ z_class \tab __ stat\cr
#'          \tab          \tab          \tab |          \tab |_ log \cr
#'          \tab          \tab          \tab |          \tab |_ paramStr \cr
#'          \tab          \tab          \tab |_ functional \tab __ alpha \tab __ stat\cr
#'          \tab          \tab          \tab |             \tab |        \tab |_ log \cr
#'          \tab          \tab          \tab |             \tab |_ beta  \tab __ stat\cr
#'          \tab          \tab          \tab |             \tab |        \tab |_ log \cr
#'          \tab          \tab          \tab |             \tab |_ sd    \tab __ stat\cr
#'          \tab          \tab          \tab |             \tab |        \tab |_ log \cr
#'          \tab          \tab          \tab |             \tab |_ paramStr \cr
#'          \tab          \tab          \tab |_ rank \tab __ mu \tab __ stat\cr
#'          \tab          \tab          \tab |       \tab |     \tab |_ log \cr
#'          \tab          \tab          \tab |       \tab |_ pi \tab __ stat\cr
#'          \tab          \tab          \tab |       \tab |     \tab |_ log \cr
#'          \tab          \tab          \tab |       \tab |_ paramStr \cr
#'          \tab          \tab          \tab |       \tab       \tab \cr
#'          \tab          \tab          \tab |_ gaussian \tab __ stat\cr
#'          \tab          \tab          \tab |           \tab |_ log \cr
#'          \tab          \tab          \tab |           \tab |_ paramStr \cr
#'          \tab          \tab          \tab |_ poisson  \tab __ stat\cr
#'          \tab          \tab          \tab |           \tab |_ log \cr
#'          \tab          \tab          \tab |           \tab |_ paramStr \cr
#'          \tab          \tab          \tab |_ ...
#'
#' }                  
#'
#'                   
#' @references 
#' Julien Jacques, Christophe Biernacki. \emph{Model-based clustering for multivariate partial ranking data}. Journal of Statistical Planning and Inference, Elsevier, 2014, 149, pp.201-217.
#' Allou Samé, Faicel Chamroukhi, Gérard Govaert, Patrice Aknin. \emph{Model-based clustering and segmentation of time series with change in regime}. Advances in Data Analysis and Classification, 2011, 5(4):301-321
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
#' # run RMixtCompt in unsupervised clustering mode + data as matrix
#' resLearn <- mixtCompLearn(simDataLearn$matrix, desc$unsupervised, algo, nbClass = 2:4)
#' 
#' # run RMixtCompt in supervised clustering mode + data as matrix
#' resLearn <- mixtCompLearn(simDataLearn$data.frame, desc$supervised, algo, nbClass = 2:4)
#' 
#' # run RMixtCompt in predict mode + data as list
#' resLearn <- mixtCompPredict(simDataPredict$list, desc$unsupervised, algo, nbClass = 2)
#' 
#' }
#' 
#' 
#' 
#' @export
mixtCompLearn <- function(data, desc, algo = createAlgo(), nClass, crit = c("BIC", "ICL"))
{
  crit = match.arg(crit)
  indCrit <- ifelse(crit == "BIC", 1, 2)
  
  dataList <- formatData(data)
  desc <- formatDesc(desc)  
  
  nClass = unique(nClass)
  algo$nInd = length(dataList[[1]])
  algo$mode = "learn"
  
  resLearn <- list()
  for(i in seq_along(nClass))
  {
    algo$nClass = nClass[i]
    
    resLearn[[i]] <- rmc(algo, dataList, desc, list())
    class(resLearn[[i]]) = "MixtComp"
  }
  
  allCrit <- sapply(resLearn, function(x) {c(getBIC(x), getICL(x))})
  colnames(allCrit) = c(nClass)
  rownames(allCrit) = c("BIC", "ICL")
  indBestClustering <- which.max(allCrit[indCrit, ])
  
  if(length(indBestClustering) != 0)
  {
    res <- c(resLearn[[indBestClustering]], list(criterion = crit, crit = allCrit, nClass = nClass, res = resLearn))
  }else{
    res <- list(warnLog = "Unable to select a model. Check $res[[i]]$warnLog for details", criterion = crit, crit = allCrit, nClass = nClass, res = resLearn)
  }
  class(res) = c("MixtCompLearn", "MixtComp")
  
  return(res)
}


#' @rdname mixtCompLearn
#' @export
mixtCompPredict <- function(data, desc, algo = createAlgo(), resLearn, nClass = NULL)
{
  dataList <- formatData(data)
  desc <- formatDesc(desc)

  algo$nInd = length(dataList[[1]])
  algo$nClass = checkNClass(nClass, resLearn)
  algo$mode = "predict"
  
  if("MixtCompLearn" %in% class(resLearn))
    resPredict <- rmc(algo, dataList, desc, resLearn$res[[which(resLearn$nClass == algo$nClass)]])
  else
    resPredict <- rmc(algo, dataList, desc, resLearn)
  
  class(resPredict) = "MixtComp"
  
  return(resPredict)
}


# format the descriptor list for rmc function:
# - add paramStr when missing
# - ensure the list format of each element
formatDesc <- function(desc)
{
  desc = lapply(desc, function(x){
    if(!is.list(x))
      x = list(type = x)
    if(!("paramStr" %in% names(x)))
      x$paramStr = ""
    
    return(x)
  })
  
  return(desc)
}


# format data.frame or matrix in list of character
# keep list in list format
formatData <- function(data)
{
  if(is.data.frame(data) || is.matrix(data))
  {
    out <- lapply(1:ncol(data), function(i) as.character(data[,i]))
    names(out) = colnames(data)
  }else{
    out <- data
  }

  return(out)
}


# check the number of class given by the user in mixtCompPredict
checkNClass <- function(nClass, resLearn)
{
  if(is.null(nClass))
    nClass = resLearn$algo$nClass
  
  if(length(nClass) > 1)
  {
    nClass = nClass[1]
    warning("Several nClass given. Only the first is used.")
  }
  
  if("MixtCompLearn" %in% class(resLearn))
  {
    if(!(nClass %in% resLearn$nClass))
    {
      nClass = resLearn$algo$nClass
      warning("The provided value of nClass does not match any results in the resLearn object. nClass is set to the value with the best criterion value.")
    }
  }else{
    if(nClass != resLearn$algo$nClass)
    {
      nClass = resLearn$algo$nClass
      warning("The provided value of nClass does not match the value in the resLearn object. nClass is set to the value of the resLearn object.")
    }
  }
  
  return(nClass)
}
