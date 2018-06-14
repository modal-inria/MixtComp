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
createMcStrategy <- function(nbBurnInIter = 100, nbIter = 100, nbGibbsBurnInIter = 50, nbGibbsIter = 50, nInitPerClass = 10, nSemTry = 20)
{
  list(nbBurnInIter = nbBurnInIter,
       nbIter = nbIter,
       nbGibbsBurnInIter = nbGibbsBurnInIter,
       nbGibbsIter = nbGibbsIter,
       nInitPerClass = nInitPerClass,
       nSemTry = nSemTry)
}


