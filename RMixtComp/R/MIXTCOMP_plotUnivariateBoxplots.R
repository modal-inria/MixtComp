#' @name plotDataBoxplot
#' @aliases plotFunctionalData plotCategoricalData plotBoxplotperClass
#' 
#' @title Boxplot per class
#' 
#' @usage plotDataBoxplot(output, var, ...)
#' plotContinuousData(output, var)
#' plotIntegerData(output, var)
#' plotCategoricalData(output, var)
#' plotFunctionalData(output, var, add.obs = FALSE, ylim = NULL, xlim = NULL)
#' 
#' @param output object returned by function \link{mixtCompCluster}
#' @param var name of the variable
#' @param ... other parameters (see \emph{Details})
#' 
#' @details For functional data, three other parameters are available:
#' \describe{
#'  \item{add.obs}{if TRUE, observations are added to the plot. Default = FALSE.}
#'  \item{ylim}{ylim of the plot.}
#'  \item{xlim}{xlim of the plot.}
#' }
#' 
#' 
#' @examples 
#' \dontrun{
#' # path to files
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descriptor.csv", package = "RMixtComp")
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
#' # plot
#' plotDataBoxplot(res, "gaussian1")
#' 
#' } 
#' 
#' @author Matthieu MARBAC
plotDataBoxplot <- function(output, var, ...)
{
  if(!(var%in%names(output$variable$type)))
    stop("This variable does not exist in the mixture model.")
  
  type <- output$variable$type[[var]]
  
  switch(type,
         "Gaussian_sjk" = plotBoxplotperClass(extractBoundsBoxplotNumericalVble(var, output)),
         "Categorical_pjk" = plotCategoricalData(extractBoundsBoxplotCategoricalVble(var, output)),
         "Poisson_k" = plotBoxplotperClass(extractBoundsBoxplotNumericalVble(var, output)),
         "Functional" = plotFunctionalData(output, var, ...),
         cat("Not yet implemented"))
}

# here bounds is a matrix
plotBoxplotperClass <- function(bounds){
  # Définition des paramètres du graphiques
  plot(NA,
       xlim=range(as.numeric(bounds)),
       ylim=c(0.2, nrow(bounds) + .8 ),
       xlab="value",
       ylab="class",
       lab=c(10, nrow(bounds), 5))
  # Boucle sur les classes
  for (k in 1:nrow(bounds)){
    # Lines horizontales
    lines(c(bounds[k,1], bounds[k,2]), c(k, k) )
    lines(c(bounds[k,2], bounds[k,4]), (-.3)* c(1, 1) + k )
    lines(c(bounds[k,2], bounds[k,4]), (.3)* c(1, 1) + k )
    lines(c(bounds[k,4], bounds[k,5]), c(k, k) )
    # Lines verticales
    lines(rep(bounds[k,1], 2), c(-.15,.15) + k)
    lines(rep(bounds[k,2], 2), c(-.3,.3) + k)
    lines(rep(bounds[k,3], 2), c(-.3,.3) + k, lwd=2)
    lines(rep(bounds[k,4], 2), c(-.3,.3) + k)
    lines(rep(bounds[k,5], 2), c(-.15,.15) + k)
  }
}
