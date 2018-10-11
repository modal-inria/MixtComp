#' @title Learn and predict using RMixtComp
#' 
#' @description Estimate the parameter of a mixture model or predict the cluster of new samples. It manages heterogeneous data as well as missing and incomplete data.
#' 
#' @param data a data.frame, a matrix or a named list containing the data (see \emph{Details} \emph{Data format} sections).
#' @param desc a named list containing models and hyperparameters (see \emph{Details} section).
#' @param algo a list containing the parameters of the SEM-Gibbs algorithm (see \emph{Details}).
#' @param nClass the number of class of the mixture model. Can be a vector.
#' @param resLearn output of \emph{mixtCompCluster} (only for \emph{mixtCompPredict} function).
#'
#' @return An object of classes MixtCompLearn and MixtComp for \emph{mixtCompLearn} function. An object of class MixtComp for \emph{mixtCompPredict}.
#' 
#' @details 
#' The \emph{data} object can be a matrix, a data.frame or a list. 
#' In the case of a matrix or data.frame, each column must be names and corresponds to a variable. In the case of a list, each element correponds to a variable, each element must be named.
#' Missing and incomplete data are managed, see section \emph{Data format} for how to format them.
#' 
#' The \emph{desc} object is a named list containing the variables to use in the model. All variables listed in the \emph{desc} object must be in the \emph{data} object. \emph{desc} can contain less variables than \emph{data}.
#' An element of the list is the model's name to use (see below for the list of available models). 
#' For example, \code{desc <- list(real1 = "Gaussian", counting1 = "Poisson")} indicates a mixture model with 2 variables named real1 and counting1 with Gaussian and Poisson as model. 
#' Some models require hyperparameters in this case, the model is described by a list of 2 elements: type containing the model name and paramStr containing the hyperparameters.
#' For example: \code{desc <- list(func1 = list(type = "Gaussian", paramStr = "nSub: 4, nCoeff: 2"), counting1 = "Poisson")}
#' 
#' Eight models are available in RMixtComp: \emph{Gaussian}, \emph{Multinomial}, \emph{Poisson}, \emph{NegativeBinomial}, \emph{Weibull}, \emph{Func_CS}, \emph{Func_SharedAlpha_CS}, \emph{Rank_ISR}. 
#' \emph{Func_CS} and \emph{Func_SharedAlpha_CS} models require hyperparameters: the number of subregressions of functional and the number of coefficients of each subregression. 
#' These hyperparameters are specified by: \emph{nSub: i, nCoeff: k} in the \emph{paramStr} field of the \emph{desc} object.
#' The \emph{Func_SharedAlpha_CS} is a variant of the \emph{Func_CS} model with the alpha parameter shared between clusters. It means that the start and end of each subregression will be the same across the clusters.
#' 
#' 
#' To perform a (semi-)supervised clustering, user can add a variable named \emph{z_class} in the data and descriptor objects with \emph{LatentClass} as model in the descriptor object.
#' 
#' 
#' The \emph{algo} object is a list containing the different number of iterations for the algorithm. 
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
#' 
#' 
#' @section Data format:
#' - Gaussian data:
#' Gaussian data are real values with the dot as decimal separator.
#' Missing data are indicated by a \emph{?}. Partial data can be provided through intervals denoted by 
#' \emph{[a:b]} where \emph{a} (resp. \emph{b}) is a real or \emph{-inf} (resp. \emph{+inf}).
#' 
#' 
#' - Categorical Data:
#' Categorical data must be consecutive integer with 1 as minimal value. Missing data are indicated by a \emph{?}.
#' For partial data, a list of possible values can be provided by \emph{{a_1,\dots,a_j}}, 
#' where \emph{a_i} denotes a categorical value.
#' 
#' - Poisson and NegativeBinomial Data:
#' Poisson and NegativeBinomial data must be positive integer. Missing data are indicated by a \emph{?}.
#' 
#' - Weibull Data:
#' Weibull data are real positive values with the dot as decimal separator.
#' Missing data are indicated by a \emph{?}. Partial data can be provided through intervals denoted by 
#' \emph{[a:b]} where \emph{a} and \emph{b} are  positive reals.  \emph{b} can be \emph{+inf}.
#' 
#' 
#' - Rank data:
#' The format of a rank is: \emph{o_1, \dots, o_j} where o_1 is an integer corresponding to the the number of the object ranked in 1st position.
#' For example: 4,2,1,3 means that the fourth object is ranked first then the second object is in second position and so on.
#' Missing data can be specified by replacing and object by a \emph{?} or a list of potential object, for example: \emph{4, \{2 3\}, \{2 1\}, ?} means that 
#' the object ranked in second position is either the object number 2 or the object number 3, then the object ranked in third position is either the object 2 or 1 and the last one can be anything.
#' A totally missing rank is spedified by \emph{?,?,\dots,?}
#' 
#' - Functional data:
#' The format of a fonctional data is: \emph{time_1:value_1,\dots, time_j:value_j}. Between individuals, functional data can have different length and different time.
#' \emph{i} is the number of subregressions in a functional data and \emph{k} the number of coefficients
#'  of each regression (2 = linear, 3 = quadratic, ...). Missing data are not supported.
#'
#' 
#' 
#' @section MixtComp object:
#' A MixtComp object is a result of a single run of MixtComp algorithm. It is a list containing three elements \emph{mixture}, \emph{variable} and \emph{algo}. 
#' If MixtComp fails to run, the list contains a single element: warnLog containing error messages.
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
#' The \emph{algo} list contains a copy of \emph{algo} parameter with extra elements: nInd, nClass, mode ("learn" or "predict").
#' 
#' 
#' The \emph{variable} list contains 3 lists : \emph{data}, \emph{type} and \emph{param}. 
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
#' @section View of a MixtComp object:
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
#' @section MixtCompLearn object:
#' The MixtCompLearn object is the result of a run of the \emph{mixtCompLearn} function. It is a list containing \emph{nClass}: the vector of number of classes given by user, \emph{res} a list of MixtComp object (one per element of \emph{nbClass}),
#' \emph{crit} the criterion used to choose the best model, \emph{criterion} a matrix containing BIC and ICL for each run, and finally the elements of the MixtComp object with the best criterion value (\emph{algo}, \emph{mixture}, \emph{variable} or \emph{warnLog}). 
#'
#'                   
#' @references 
#' Julien Jacques, Christophe Biernacki. \emph{Model-based clustering for multivariate partial ranking data}. Journal of Statistical Planning and Inference, Elsevier, 2014, 149, pp.201-217.
#' 
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
