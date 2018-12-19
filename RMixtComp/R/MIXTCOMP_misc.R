#' @title Create algo object 
#' 
#' @description create an algo object required by \link{mixtCompLearn} and \link{mixtCompPredict}.
#'
#' @param nbBurnInIter Number of iterations of the burn-in part of the SEM algorithm.
#' @param nbIter Number of iterations of the SEM algorithm.
#' @param nbGibbsBurnInIter Number of iterations of the burn-in part of the Gibbs algorithm.
#' @param nbGibbsIter Number of iterations of the Gibbs algorithm.
#' @param nInitPerClass Number of individuals used to initialize each cluster (default = 10).
#' @param nSemTry Number of try of the algorithm for avoiding an error.
#' @param confidenceLevel confidence level for confidence bounds for parameter estimation
#' @param ratioStableCriterion stability partition required to stop earlier the SEM 
#' @param nStableCriterion number of iterations of partition stability to stop earlier the SEM
#' 
#' @return a list with the parameters values
#' 
#' @examples 
#' # default values
#' algo <- createAlgo()
#'
#' # change some values
#' algo <- createAlgo(nbIter = 200)
#' 
#' @export
createAlgo <- function(nbBurnInIter = 50, nbIter = 50, nbGibbsBurnInIter = 50, nbGibbsIter = 50, nInitPerClass = 10, nSemTry = 20, confidenceLevel = 0.95, ratioStableCriterion = 0.95, nStableCriterion = 10)
{
  list(nbBurnInIter = nbBurnInIter,
       nbIter = nbIter,
       nbGibbsBurnInIter = nbGibbsBurnInIter,
       nbGibbsIter = nbGibbsIter,
       nInitPerClass = nInitPerClass,
       nSemTry = nSemTry,
       confidenceLevel = confidenceLevel,
       ratioStableCriterion = ratioStableCriterion,
       nStableCriterion = nStableCriterion)
}


#' @name computeDiscrimPowerVar
#' 
#' @title Discriminative power
#'
#' @description Compute the discriminative power of each variable or classe
#'
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \link{mixtCompLearn} or \link{mixtCompPredict} functions.
#'
#' @return the discriminative power
#' 
#' @details 
#' The discriminative power of variable j is defined by 1 - C(j)
#' \deqn{C(j)=  - \sum_{k=1}^K sum_{i=1}^n P(Z_i=k|x_{ij}) \log(P(Z_i=k|x_{ij})) / (n*\log(K))}
#' 
#' A high value (close to one) means that the variable is highly discriminating. A low value (close to zero) means that the variable is poorly discriminating.
#' 
#' 
#' The discriminative power of class k is defined by 1 - D(k)
#' \deqn{D(k) =  -\sum_{i=1}^n P(Z_i=k|x_i) \log(P(Z_i=k|x_i)) / (n*\exp(-1))}
#' 
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
#' discVar <- computeDiscrimPowerVar(resLearn)
#' discClass <- computeDiscrimPowerClass(resLearn)
#' 
#' # graphic representation of discVar
#' plotDiscrimVar(resLearn)
#' # graphic representation of discClass
#' plotDiscrimClass(resLearn)
#' }
#' 
#' @seealso \code{\link{plotDiscrimClass}} \code{\link{plotDiscrimVar}}
#' @export
computeDiscrimPowerVar <- function(outMixtComp)
{
  return(1-colSums(outMixtComp$mixture$IDClass))
}

#' @rdname computeDiscrimPowerVar
#' @export
computeDiscrimPowerClass <- function(outMixtComp)
{
  return(1 -(-colMeans(log(outMixtComp$variable$data$z_class$stat**outMixtComp$variable$data$z_class$stat)) / exp(-1)))
}


#' @name computeSimilarityVar
#' 
#' @title Similarity
#'
#' @description Compute the similarity between variables (or classes)
#'
#'
#' @param outMixtComp object of class \emph{MixtCompLearn} or \emph{MixtComp} obtained using \link{mixtCompLearn} or \link{mixtCompPredict} functions.
#'
#' @return a similarity matrix
#' 
#' @details 
#' The similarities between variables j and h is defined by Delta(j,h)
#' \deqn{Delta(j,h)^2 = 1 - \sqrt{(1/n) * \sum_{i=1}^n \sum_{k=1}^K (P(Z_i=k|x_{ij}) - P(Z_i=k|x_{ih}))^2}}
#' 
#' The similarities between classes k and g is defined by 1 - Sigma(k,g)
#' \deqn{Sigma(k,g)^2 = (1/n) * \sum_{i=1}^n (P(Z_i=k|x_i) - P(Z_i=g|x_i))^2}
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
#' simVar <- computeSimilarityVar(resLearn)
#' simClass <- computeSimilarityClass(resLearn)
#' }
#' 
#' @seealso \code{\link{heatmapVar}} \code{\link{heatmapClass}}
#' @export
computeSimilarityVar <- function(outMixtComp)
{
  simil <- outMixtComp$mixture$delta

  return(simil)
}

#' @rdname computeSimilarityVar
#' @export
computeSimilarityClass <- function(outMixtComp)
{
  simil <- 1-sqrt(sapply(1:outMixtComp$algo$nClass,
                         function(k) colMeans(sweep(outMixtComp$variable$data$z_class$stat, 1, outMixtComp$variable$data$z_class$stat[,k],"-")**2)))
  
  if(is.null(outMixtComp$algo$dictionary$z_class))
    colnames(simil) = rownames(simil) = paste("Class", 1:outMixtComp$algo$nClass)
  else
    colnames(simil) = rownames(simil) = outMixtComp$algo$dictionary$z_class$old
  
  return(simil)
}



# rand index 
rand.index <- function (partition1, partition2) 
{
  x <- abs(sapply(partition1, function(x) {x - partition1}))
  x[x > 1] <- 1
  
  y <- abs(sapply(partition2, function(x) {x - partition2}))
  y[y > 1] <- 1
  
  sg <- sum(abs(x - y))/2
  bc <- choose(dim(x)[1], 2)
  
  randIndex <- 1 - sg/bc
  
  return(randIndex)
}


#' Available models
#'
#' Get information about models implemented in MixtComp
#'
#' @return a data.frame containing models implemented in MixtComp
#' \describe{
#' \item{model}{model name}
#' \item{data.type}{data type}
#' \item{format}{Special format required for individuals}
#' \item{missing.formats}{accepted formats (separated by a ;) for missing values}
#' \item{hyperparameter}{Required hyperparameters in the paramStr elements of model object}
#' \item{comments}{comments about the model}
#' \item{reference}{link to article}
#' }
#'
#' @seealso \code{\link{mixtCompLearn}}
#' 
#' @export
availableModels <- function()
{
  data.frame("model" = c("Gaussian", "Weibull", "Multinomial", "Poisson", "NegativeBinomial", "Rank_ISR", "Func_CS", "Func_SharedAlpha_CS"), 
             "data type" = c("Real", "Positive real", "Categorical", "Positive integer", "Positive integer", "Rank", "Functional", "Functional"),
             "format" = c("", "", "Numbers from 1 to number of modalities", "", "", "o1,o2,..,oN", "time1:value1,time2:value2,...,timeN:valueN", "time1:value1,time2:value2,...,timeN:valueN"),
             "missing formats" = c("?; [a:b]; [-inf:b]; [a:inf]", "?; [a:b]; [a:+inf]", "?; {a,..,b}", "?", "?", "?,?,?,?; 4,3,?,?; {2 3}, 4,{2 3}, 1", "", ""),
             "hyperparameter" = c("", "", "", "", "", "", "nSub:a, nCoeff:b", "nSub:a, nCoeff:b"),
             "comments" = c("", "", "", "", "", "o1,o2,..,oN: o1 is an integer corresponding to the object's number ranked in 1st position", 
                            "nSub = number of subregressions. nCoeff = number of coefficients of each polynomial regression. Between individuals, functionals can have different time values and length.", 
                            "Func_CS with parameter alpha shared between classes: subregressions start and finish at the same time between clusters."),
             "reference" = c("", "", "", "", "", "https://hal.inria.fr/hal-00743384", "https://arxiv.org/abs/1312.6967", "https://arxiv.org/abs/1312.6967")
             )
}