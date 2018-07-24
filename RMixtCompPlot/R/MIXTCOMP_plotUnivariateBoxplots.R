#' Boxplot per class
#' 
#' @param output object returned by function \emph{mixtCompCluster}
#' @param var name of the variable
#' @param grl if TRUE plot the general distribution of the data
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
#' 
#' library(RMixtComp)
#' 
#' # path to files
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descUnsupervised.csv", package = "RMixtComp")
#' 
#' resGetData <- getData(c(pathToData, pathToDescriptor))
#' 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- createMcStrategy()
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # plot
#' plotDataBoxplot(res, "Gaussian1")
#' 
#' } 
#' 
#' @author Matthieu MARBAC
#'
#' @export
plotDataBoxplot <- function(output, var, grl = TRUE, ...)
{
  if(!(var%in%names(output$variable$type)))
    stop("This variable does not exist in the mixture model.")
  
  type <- output$variable$type[[var]]
  
  type <- ifelse(type %in% c("Gaussian_sjk", "Weibull", "Poisson_k", "NegativeBinomial"), "Numerical", type)
  
  switch(type,
         "Numerical" = plotBoxplotperClass(extractBoundsBoxplotNumericalVble(var, output, class = 1:output$mixture$nbCluster, grl = grl)),
         "Categorical_pjk" = plotCategoricalData(extractBoundsBoxplotCategoricalVble(var, output), var, class = 1:output$mixture$nbCluster, grl),
         "Functional" = plotFunctionalData(output, var, ...),
         "FunctionalSharedAlpha" = plotFunctionalData(output, var, ...),
         warning(paste0("Not (yet) available for model ", type)))
}

# here bounds is a matrix
plotBoxplotperClass <- function(bounds){
  # Définition des paramètres du graphiques
  plot(NA,
       xlim=range(as.numeric(bounds)),
       ylim=c(0.2, nrow(bounds) + .8 ),
       xlab="value",
       ylab="class",
       yaxt = "n",
       lab=c(10, nrow(bounds), 5))
  ytick <- 1:nrow(bounds)
  axis(side = 2, at = ytick, labels = rownames(bounds))
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
