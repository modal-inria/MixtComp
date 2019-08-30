# MixtComp version 4.0  - july 2019
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
 

#' @title Learn and predict using RMixtComp
#' 
#' @description Estimate the parameter of a mixture model or predict the cluster of new samples. It manages heterogeneous data as well as missing and incomplete data.
#' 
#' @param data a data.frame, a matrix or a named list containing the data (see \emph{Details} \emph{Data format} sections).
#' @param model a named list containing models and hyperparameters (see \emph{Details} section).
#' @param algo a list containing the parameters of the SEM-Gibbs algorithm (see \emph{Details} or \link{createAlgo}).
#' @param nClass the number of class of the mixture model. Can be a vector for \emph{mixtCompLearn} only.
#' @param criterion "BIC" or "ICL". Criterion used for choosing the best model.
#' @param hierarchicalMode "auto", "yes" or "no". If "auto", it performs a hierarchical version of MixtComp (clustering in two classes then each classes is split in two ...) when a functional variable is present.
#' @param nRun number of runs for every given number of class. If >1, SEM is run \code{nRun} times for every number of class, and the best according to observed likelihood is kept.
#' @param nCore number of cores used for the parallelization of the \emph{nRun} runs.
#' @param resLearn output of \emph{mixtCompLearn} (only for \emph{mixtCompPredict} function).
#' @param verbose if TRUE, print some informations.
#'
#' @return An object of classes MixtCompLearn and MixtComp for \emph{mixtCompLearn} function. An object of class MixtComp for \emph{mixtCompPredict}.
#' 
#' @details 
#' The \emph{data} object can be a matrix, a data.frame or a list. 
#' In the case of a matrix or data.frame, each column must be names and corresponds to a variable. In the case of a list, each element correponds to a variable, each element must be named.
#' Missing and incomplete data are managed, see section \emph{Data format} for how to format them.
#' 
#' The \emph{model} object is a named list containing the variables to use in the model. All variables listed in the \emph{model} object must be in the \emph{data} object. \emph{model} can contain less variables than \emph{data}.
#' An element of the list is the model's name to use (see below for the list of available models). 
#' For example, \code{model <- list(real1 = "Gaussian", counting1 = "Poisson")} indicates a mixture model with 2 variables named real1 and counting1 with Gaussian and Poisson as model. 
#' Some models require hyperparameters in this case, the model is described by a list of 2 elements: type containing the model name and paramStr containing the hyperparameters.
#' For example: \code{model <- list(func1 = list(type = "Func_CS", paramStr = "nSub: 4, nCoeff: 2"), counting1 = "Poisson")}.
#' If the model is NULL, data are supposed to be provided in data.frame or list with R format (numeric, factor, character, NA as missing value). 
#' Models will be imputed as follows: "Gaussian" for numeric variable, "Multinomial" for character or factor variable and "Poisson" for integer variable.
#' A summary of available models (and associated hyperparameters and missing format) can be accessed by calling the \link{availableModels} function.
#' 
#' Eight models are available in RMixtComp: \emph{Gaussian}, \emph{Multinomial}, \emph{Poisson}, \emph{NegativeBinomial}, \emph{Weibull}, \emph{Func_CS}, \emph{Func_SharedAlpha_CS}, \emph{Rank_ISR}. 
#' \emph{Func_CS} and \emph{Func_SharedAlpha_CS} models require hyperparameters: the number of subregressions of functional and the number of coefficients of each subregression. 
#' These hyperparameters are specified by: \emph{nSub: i, nCoeff: k} in the \emph{paramStr} field of the \emph{model} object.
#' The \emph{Func_SharedAlpha_CS} is a variant of the \emph{Func_CS} model with the alpha parameter shared between clusters. It means that the start and end of each subregression will be the same across the clusters.
#' 
#' 
#' To perform a (semi-)supervised clustering, user can add a variable named \emph{z_class} in the data and model objects with \emph{LatentClass} as model in the model object.
#' 
#' 
#' The \emph{algo} object is a list containing the different number of iterations for the algorithm. This list can be generated using the \link{createAlgo} function. 
#' The algorithm is decomposed in a burn-in phase and a normal phase. 
#' Estimates from the burn-in phase are not shown in output.
#' \itemize{
#'   \item{nbBurnInIter: Number of iterations of the burn-in part of the SEM algorithm.}
#'   \item{nbIter: Number of iterations of the SEM algorithm.}
#'   \item{nbGibbsBurnInIter: Number of iterations of the burn-in part of the Gibbs algorithm.}
#'   \item{nbGibbsIter: Number of iterations of the Gibbs algorithm.}
#'   \item{nInitPerClass: Number of individuals used to initialize each cluster (default = 10).}
#'   \item{nSemTry: Number of try of the algorithm for avoiding an error.}
#'   \item{confidenceLevel: confidence level for confidence bounds for parameter estimation}
#'   \item{ratioStableCriterion: stability partition required to stop earlier the SEM }
#'   \item{nStableCriterion: number of iterations of partition stability to stop earlier the SEM}
#' }
#' 
#' 
#' @section Data format:
#' See \url{https://github.com/modal-inria/MixtComp/blob/master/MixtComp/docs/dataFormat.md}.
#' 
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
#' Poisson and NegativeBinomial data must be positive integer. Missing data are indicated by a \emph{?}. Partial data can be provided through intervals denoted by 
#' \emph{[a:b]} where \emph{a} and \emph{b} are  positive integers. \emph{b} can be \emph{+inf}.
#' 
#' - Weibull Data:
#' Weibull data are real positive values with the dot as decimal separator.
#' Missing data are indicated by a \emph{?}. Partial data can be provided through intervals denoted by 
#' \emph{[a:b]} where \emph{a} and \emph{b} are  positive reals. \emph{b} can be \emph{+inf}.
#' 
#' 
#' - Rank data:
#' The format of a rank is: \emph{o_1, \dots, o_j} where o_1 is an integer corresponding to the number of the object ranked in 1st position.
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
#' - z_class:
#' To perform a (semi-)supervised clustering, user can add a variable named `z_class` (with eventually some missing values) with "LatentClass" as model.
#' Missing data are indicated by a \emph{?}. For partial data, a list of possible values can be provided by \emph{{a_1,\dots,a_j}}, where \emph{a_i} denotes a class number.
#' 
#' @section MixtComp object:
#' A MixtComp object is a result of a single run of MixtComp algorithm. It is a list containing three elements \emph{mixture}, \emph{variable} and \emph{algo}. 
#' If MixtComp fails to run, the list contains a single element: warnLog containing error messages.
#' 
#' The \emph{mixture} element contains
#' \itemize{
#'   \item{BIC: value of BIC}
#'   \item{ICL: value of ICL}
#'   \item{nbFreeParameters: number of free parameters of the mixture}
#'   \item{lnObservedLikelihood: observed loglikelihood}
#'   \item{lnCompletedLikelihood: completed loglikelihood}
#'   \item{IDClass: entropy used to compute the discriminative power (see code of \link{plotDiscrimVar})}
#'   \item{delta: similarities between variables (see \link{heatmapVar})}
#'   \item{completedProbabilityLogBurnIn: evolution of the completed log-probability during the burn-in period (can be used to check the convergence and determine the ideal number of iteration)}
#'   \item{completedProbabilityLogRun: evolution of the completed log-probability  after the burn-in period (can be used to check the convergence and determine the ideal number of iteration)} 
#'   \item{runTime: list containing the total execution time in seconds and the execution time of some subpart.} 
#'   \item{lnProbaGivenClass: log-proportion + log-probability of x_i for each class}
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
#' |        \tab      \tab |_ ratioStableCriterion\cr
#' |        \tab      \tab |_ nStableCriterion\cr
#' |        \tab      \tab |_ confidenceLevel  \cr
#' |        \tab      \tab |_ mode \cr
#' |        \tab      \tab |_ nInd  \cr
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
#' See \url{https://github.com/modal-inria/MixtComp/blob/master/MixtComp/docs/objectOutput.md}
#'
#' @section MixtCompLearn object:
#' The MixtCompLearn object is the result of a run of the \emph{mixtCompLearn} function. It is a list containing \emph{nClass}: the vector of number of classes given by user, \emph{res} a list of MixtComp object (one per element of \emph{nbClass}),
#' \emph{criterion} the criterion used to choose the best model, \emph{crit} a matrix containing BIC and ICL for each run, and finally the elements of the MixtComp object with the best criterion value (\emph{algo}, \emph{mixture}, \emph{variable} or \emph{warnLog}). 
#'
#'                   
#' @references 
#' Julien Jacques, Christophe Biernacki. \emph{Model-based clustering for multivariate partial ranking data}. Journal of Statistical Planning and Inference, Elsevier, 2014, 149, pp.201-217.
#' 
#' Allou Samé, Faicel Chamroukhi, Gérard Govaert, Patrice Aknin. \emph{Model-based clustering and segmentation of time series with change in regime}. Advances in Data Analysis and Classification, 2011, 5(4):301-321
#'                    
#' @examples 
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- list(nbBurnInIter = 50,
#'              nbIter = 50,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 20,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # run RMixtComp in unsupervised clustering mode + data as matrix
#' resLearn1 <- mixtCompLearn(simData$dataLearn$matrix, simData$model$unsupervised[1:3], algo,
#'                            nClass = 2:3, nRun = 2, nCore = 1)
#' 
#' # run RMixtComp in supervised clustering mode + data as matrix
#' resLearn2 <- mixtCompLearn(simData$dataLearn$data.frame, simData$model$supervised[1:3], algo, 
#'                            nClass = 2:3, nRun = 2, nCore = 1)
#' 
#' # run RMixtComp in predict mode + data as list
#' resPredict <- mixtCompPredict(simData$dataPredict$list, simData$model$unsupervised[1:3], algo,
#'                               resLearn1, nClass = 2, nCore = 1)
#' 
#' 
#' 
#' @seealso Graphical and utility functions in \code{RMixtCompUtilities}. Other clustering packages: \code{Rmixmod}, \code{blockcluster}
#' @author Quentin Grimonprez
#' @export
mixtCompLearn <- function(data, model = NULL, algo = createAlgo(), nClass, criterion = c("BIC", "ICL"), hierarchicalMode = c("auto", "yes", "no"), nRun = 1, nCore = min(max(1, ceiling(detectCores()/2)), nRun), verbose = TRUE)
{
  t1 <- proc.time()
  hierarchicalMode = match.arg(hierarchicalMode) 
  criterion = match.arg(criterion)
  
  ## parameters pretreatment 
  
  # basic : data in R format (missing coded as NA). The user does not give a model, models are imputed among "Gaussian" (numeric data), "Multinomial" (character or factor) and "Poisson" (integer). 
  # expert : data in MixtComp format. The user give a model argument.
  if(is.null(model))
  {
    mode <- "basic"
    model <- imputModel(data)
  }else{
    mode <- "expert"
    model <- RMixtCompUtilities:::formatModel(model)  
  }
  
  performHier <- performHierarchical(hierarchicalMode, mode, model)
  
  ## run MixtComp
  if(performHier)
    resLearn <- hierarchicalLearn(data, model, algo, nClass, criterion, minClassSize = 5, nRun = nRun, nCore = nCore, verbose)
  else
    resLearn <- classicLearn(data, model, algo, nClass, criterion, nRun, nCore, verbose, mode)
  t2 <- proc.time()
  
  if(verbose)
  {
    cat(paste0("Total runtime: ", round((t2-t1)[3], 3), "s\n"))
    if(is.null(resLearn$warnLog))
      cat(paste0("Best model according to ", criterion, ": ", resLearn$algo$nClass," clusters.\n"))
    else
      cat(paste0("Error, no model returned, check $warnLog.\n"))
  }
    
  
  return(resLearn)
}


#' @rdname mixtCompLearn
#' @export
mixtCompPredict <- function(data, model = NULL, algo = resLearn$algo, resLearn, nClass = NULL, nRun = 1, nCore = min(max(1, ceiling(detectCores()/2)), nRun), verbose = FALSE)
{
  ## parameters pretreatment
  if(is.null(model))
    model = getModel(resLearn, with.z_class = FALSE)
  
  model = RMixtCompUtilities:::formatModel(model)
  
  
  if(resLearn$algo$basicMode)
  {
    dataList <- formatDataBasicMode(data, model, resLearn$algo$dictionary)$data
  }else{
    dataList <- RMixtCompUtilities:::formatData(data)
  }
  
  
  algo$nInd = length(dataList[[1]])
  algo$nClass = checkNClass(nClass, resLearn)
  algo$mode = "predict"
  
  algo = RMixtCompUtilities:::completeAlgo(algo)
  
  ## run predict
  if("MixtCompLearn" %in% class(resLearn))
    resPredict <- rmcMultiRun(algo, dataList, model, resLearn$res[[which(resLearn$nClass == algo$nClass)]], nRun, nCore, verbose)
  else
    resPredict <- rmcMultiRun(algo, dataList, model, resLearn, nRun, nCore, verbose)
  
  if(!is.null(resPredict$warnLog))
    warning(paste0("MixtComp failed with the following error: ", resPredict$warnLog))
  else{
    resPredict$algo$basicMode = resLearn$algo$basicMode
    
    if(resPredict$algo$basicMode)
      resPredict = formatOutputBasicMode(resPredict, resLearn$algo$dictionary)
  }
  
  class(resPredict) = "MixtComp"
  
  return(resPredict)
}

# @author Quentin Grimonprez
classicLearn <- function(data, model, algo, nClass, criterion, nRun, nCore, verbose, mode)
{
  
  if(mode == "basic")
  {
    out <- formatDataBasicMode(data, model)
    dataList <- out$data
    dictionary <- out$dictionary
    
    if(verbose)
    {
      cat("You did not provide a model parameter.\n")
      cat("Data are assumed to follow R standard and not MixtComp standard.\n")
      cat("Models will be imputed as follows: \"Gaussian\" for numeric variable, \"Multinomial\" for character or factor variable and \"Poisson\" for integer variable.\n")
      cat("If a variable is named \"z_class\" and its type is character, factor or integer, it will be used as \"LatentClass\".\n")
      print(head(sapply(model, function(x) x$type)))
    }
    
  }else{
    dataList <- RMixtCompUtilities:::formatData(data)
    model <- completeModel(model, dataList)
  }
  
  algo$nInd = length(dataList[[1]])
  algo$mode = "learn"
  
  algo = RMixtCompUtilities:::completeAlgo(algo)
  
  nClass = unique(nClass)
  
  indCrit <- ifelse(criterion == "BIC", 1, 2)
  
  if(verbose)
  {
    cat(paste0("====== Run MixtComp in ", algo$mode, " mode with ", nRun, " run(s) per number of classes and ", nCore, " core(s)\n"))
    cat(paste0("Data: ", algo$nInd, " individuals and ", length(model), " variables.\n"))
  }
  
  resLearn <- list()
  for(i in seq_along(nClass))
  {
    if(verbose)
      cat(paste0("-- K = ", nClass[i], "\n"))
    
    algo$nClass = nClass[i]
    
    t1 <- proc.time()
    resLearn[[i]] <- rmcMultiRun(algo, dataList, model, list(), nRun, nCore, verbose)
    t2 <- proc.time()
    
    class(resLearn[[i]]) = "MixtComp"
    if(!is.null(resLearn[[i]]$warnLog))
      warning(paste0("For k=", nClass[i], ", MixtComp failed with the following error: ", resLearn[[i]]$warnLog))
    else{
      if(verbose)
        cat(paste0("Run completed in ", round((t2-t1)[3], 3), "s\n"))
      
      resLearn[[i]]$algo$basicMode = (mode == "basic")
      resLearn[[i]]$algo$hierarchicalMode = FALSE
      
      if(resLearn[[i]]$algo$basicMode)
        resLearn[[i]] = formatOutputBasicMode(resLearn[[i]], dictionary)
    }
  }
  
  ## Choose the best number of classes according to crit
  allCrit <- sapply(resLearn, function(x) {c(getBIC(x), getICL(x))})
  colnames(allCrit) = nClass
  rownames(allCrit) = c("BIC", "ICL")
  indBestClustering <- which.max(allCrit[indCrit, ])
  
  if(length(indBestClustering) != 0)
  {
    res <- c(resLearn[[indBestClustering]], list(nRun = nRun, criterion = criterion, crit = allCrit, nClass = nClass, res = resLearn))
  }else{
    res <- list(warnLog = "Unable to select a model. Check $res[[i]]$warnLog for details", criterion = criterion, crit = allCrit, nClass = nClass, res = resLearn)
    warning(paste0("MixtComp failed for all the given number of classes."))
  }
  class(res) = c("MixtCompLearn", "MixtComp")
  
  return(res)
}

# @author Quentin Grimonprez
hierarchicalLearn <- function(data, model, algo, nClass, criterion, minClassSize = 5, nRun = 1, nCore = min(max(1, ceiling(detectCores()/2)), nRun), verbose = TRUE)
{
  indCrit <- ifelse(criterion == "BIC", 1, 2)
  
  nClass <- max(nClass)
  
  dataList <- RMixtCompUtilities:::formatData(data)
  model <- completeModel(model, dataList)
  
  algo$nInd = length(dataList[[1]])
  algo$mode = "learn"
  
  algo = RMixtCompUtilities:::completeAlgo(algo)
  
  resLearn <- hierarchicalMixtCompLearn(data, model, algo, nClass, criterion, minClassSize, nRun, nCore, verbose)
  
  ## Choose the best number of classes according to crit
  allCrit <- sapply(resLearn$res[-1], function(x) {c(getBIC(x), getICL(x))})
  colnames(allCrit) = c(resLearn$nClass)
  rownames(allCrit) = c("BIC", "ICL")
  indBestClustering <- which.max(allCrit[indCrit, ])
  
  if(length(indBestClustering) != 0)
  {
    res <- c(resLearn$res[-1][[indBestClustering]], list(nRun = nRun, criterion = criterion, crit = allCrit, nClass = resLearn$nClass, res = resLearn$res[-1]))
    res$algo$basicMode = FALSE
    res$algo$hierarchicalMode = TRUE
  }else{
    res <- list(warnLog = "Unable to select a model. Check $res[[i]]$warnLog for details", criterion = criterion, crit = allCrit, nClass = resLearn$nClass, res = resLearn$res[-1])
    warning(paste0("MixtComp failed for all the given number of classes."))
  }
  class(res) = c("MixtCompLearn", "MixtComp")
  
  return(res)
}

# must an hierarchical run be done ?
# hierarchicalMode mixtCompLearn's argument
# mode "basic" or "expert"
# model mixtCompLearn's argument
# @author Quentin Grimonprez
performHierarchical <- function(hierarchicalMode, mode, model)
{
  if((mode == "basic") || (hierarchicalMode == "no"))
    return(FALSE)
  
  if(hierarchicalMode == "yes")
    return(TRUE)
  
  # hierarchicalMode = "auto"
  containFunctional <- any(sapply(model, function(x){x$type %in% c("Func_CS", "Func_SharedAlpha_CS")}))
  if(containFunctional)
    return(TRUE)
  
  return(FALSE)
}
