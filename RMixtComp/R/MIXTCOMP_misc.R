#' @title create mcStrategy object 
#' 
#' @description create a mcStrategy object 
#'
#' @param nbBurnInIter Number of iterations of the burn-in part of the SEM algorithm.
#' @param nbIter Number of iterations of the SEM algorithm.
#' @param nbGibbsBurnInIter Number of iterations of the burn-in part of the Gibbs algorithm.
#' @param nbGibbsIter Number of iterations of the Gibbs algorithm.
#' @param nInitPerClass Number of individuals used to initialize each cluster (default = 10).
#' @param nSemTry Number of try of the algorithm for avoiding an error.
#' 
#' @return a list with the parameters values
#' 
#' @examples 
#' # default values
#' mcStrategy <- createMcStrategy()
#'
#' # default values
#' mcStrategy <- createMcStrategy(nbIter = 200)
#' 
#' @export
createMcStrategy <- function(nbBurnInIter = 100, nbIter = 100, nbGibbsBurnInIter = 50, nbGibbsIter = 50, nInitPerClass = 10, nSemTry = 20)
{
  list(nbBurnInIter = nbBurnInIter,
       nbIter = nbIter,
       nbGibbsBurnInIter = nbGibbsBurnInIter,
       nbGibbsIter = nbGibbsIter,
       nInitPerClass = nInitPerClass,
       nSemTry = nSemTry)
}


#' @name computeDiscrimPowerVar
#' 
#' @title Discriminative power
#'
#' @description Compute the discriminative power of each variable or classe
#'
#'
#' @param outMixtComp output object of \link{mixtCompCluster} or \link{mixtCompPredict} functions.
#'
#' @return the discriminative power
#' 
#' @details 
#' The discriminative power of variable j is defined by 1 - Cj
#' Cj=  - sum_{k=1}^K sum_{i=1}^n P(Z_i=k|x_{ij}) ln(P(Z_i=k|x_{ij})) / (n*lnK)
#' 
#' A high value (close to one) means that the variable is highly discriminating. A low value (close to zero) means that the variable is poorly discriminating.
#' 
#' 
#' The discriminative power of class k is defined by 1 - Dk
#' Dk=  - sum_{i=1}^n P(Z_i=k|x_i) ln(P(Z_i=k|x_i)) / (n*exp(-1))
#' 
#'
#' @examples 
#' \dontrun{
#' # path to files
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RMixtComp")
#' 
#' resGetData <- getData(c(pathToData, pathToDescriptor))
#' 
#' # define the algorithm's parameters
#' mcStrategy <- createMcStrategy()
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' computeDiscrimPowerVar(res)
#' computeDiscrimPowerClass(res)
#' }
#' 
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
#' @param outMixtComp output object of \link{mixtCompCluster} or \link{mixtCompPredict} functions.
#'
#' @return a similarity matrix
#' 
#' @details 
#' The similarities between variables j and h is defined by 1 - Delta(j,h)
#' Delta(j,h)^2 = (1/n) * sum_{i=1}^n sum_{k=1}^K (P(Z_i=k|x_{ij}) - P(Z_i=k|x_{ih}))^2
#' 
#' The similarities between classes k and g is defined by 1 - Sigma(k,g)
#' Sigma(k,g)^2 = (1/n) * sum_{i=1}^n (P(Z_i=k|x_i) - P(Z_i=g|x_i))^2
#'
#' @examples 
#' \dontrun{
#' # path to files
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RMixtComp")
#' 
#' resGetData <- getData(c(pathToData, pathToDescriptor))
#' 
#' # define the algorithm's parameters
#' mcStrategy <- createMcStrategy()
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' computeSimilarityVar(res)
#' computeSimilarityClass(res)
#' }
#' 
#' @export
computeSimilarityVar <- function(outMixtComp)
{
  varNames <- names(outMixtComp$variable$type)[names(outMixtComp$variable$type)!="z_class"]
  
  simil <- 1-outMixtComp$mixture$delta
  rownames(simil) = colnames(simil) = varNames

  return(simil)
}

#' @rdname computeSimilarityVar
#' @export
computeSimilarityClass <- function(outMixtComp)
{
  simil <- 1-sqrt(sapply(1:outMixtComp$mixture$nbCluster,
                function(k) colMeans(sweep(outMixtComp$variable$data$z_class$stat, 1, outMixtComp$variable$data$z_class$stat[,k],"-")**2)))
  colnames(simil) = rownames(simil)
  
  return(simil)
}
