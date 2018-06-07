###################################################################################
#' @title Run clusvis for MixtComp output
#' 
#' @description This function estimates the parameters used for visualization of model-based clustering performs with R package ClusVis 
#' To achieve the parameter infernece, it automatically samples probabilities of classification from the model parameters
#'
#'
#' @param resmixtcomp list. It is an object return by MixComp.
#' @param sample.size numeric. Number of probabilities of classification sampled for parameter inference.
#' @param maxit numeric. It limits the number of iterations for the Quasi-Newton algorithm (default 1000).
#' @param nbrandomInit numeric. It defines the number of random initialization of the Quasi-Newton algorithm.
#' @param nbcpu numeric. It specifies the number of CPU (only for linux, R package parallel must by loaded).
#' @param estimateLogTik if TRUE, estimate the logtik. If FALSE, use the logtik returned by MixtComp
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
#' mcStrategy <- list(nbBurnInIter = 100,
#'                    nbIter = 100,
#'                    nbGibbsBurnInIter = 50,
#'                    nbGibbsIter = 50,
#'                    ratioInitialization = 0.1)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#'
#' resvisu <- clusvisMixtComp(res)
#' }
#'
#' @export
clusvisMixtComp <- function(resmixtcomp, sample.size = 5000, maxit = 10**3, nbrandomInit = 12, nbcpu = 1, estimateLogTik = FALSE){
  
  if(estimateLogTik)
  {
    logtik.estim <- t(replicate(sample.size, rlogtikmixcomp(resmixtcomp)))
    out <- clusvis(logtik.estim, prop=resmixtcomp$variable$param$z_class$pi$stat[,1], logtik.obs=logmixcomp(resmixtcomp), maxit, nbrandomInit, nbcpu)
  }else{
    out <- clusvis(computelogtikmixtcomp(resmixtcomp), prop=resmixtcomp$variable$param$z_class$pi$stat[,1], logtik.obs=computelogtikmixtcomp(resmixtcomp), maxit, nbrandomInit, nbcpu)
  }
  
  return(out)
}

###################################################################################
#' @name plotComponentClusVis
#' @aliases  plotObservationsClusVis
#'
#' @title ClusVis plots
#'
#' @description plotComponentClusVis is a wrapper around the plotDensityClusVisu function of the ClusVisu package. It generates
#' a component interpretation graph using the visualization information returned by the clusvisMixtcomp function.
#' plotObservationsClusVis is a wrapper around the plotDensityClusVisu function of the ClusVisu package. It generates
#' a scatter-plot of the observation memberships using the visualization information returned by the clusvisMixtcomp function.
#'
#' @usage plotComponentClusVis(visInfo)
#' plotObservationsClusVis(visInfo)
#'
#' @param visInfo list. The parameters estimated by the clusvisMixtcomp function.
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
#' mcStrategy <- list(nbBurnInIter = 100,
#'                    nbIter = 100,
#'                    nbGibbsBurnInIter = 50,
#'                    nbGibbsIter = 50,
#'                    ratioInitialization = 0.1)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#'
#' resvisu <- clusvisMixtComp(res)
#' 
#' plotComponentClusVis(resvisu)
#' plotObservationsClusVis(resvisu) 
#' 
#' }
#'
#' @export
plotComponentClusVis <- function(visInfo) {
  plotDensityClusVisu(visInfo, add.obs=F, positionlegend="bottomright", xlim=range(visInfo$centers[,1])+c(-4,6), ylim=range(visInfo$centers[,2])+c(-6,4))
}

###################################################################################
#' @export
plotObservationsClusVis <- function(visInfo) {
  plotDensityClusVisu(visInfo, add.obs=T, positionlegend="bottomright", xlim=range(visInfo$y[,1])+c(0,2), ylim=range(visInfo$y[,2])+c(-2,0))
}




# This function returns the log-probabilites of classification for each observation
# It takes one input argument (named output) returned by the function Rmixcomp
# It returns a matrix of the logarithm of the probabilities of classification
# Rows correspond to observations and column correspond to component
# Here, few types of data are allowed: continuous or categorical
# The other types of data must be implemented
computelogtikmixtcomp <- function(output){
  sweep(output$mixture$lnProbaGivenClass, 
        1,
        apply(output$mixture$lnProbaGivenClass,
              1,
              function(vec) (max(vec) + log(sum(exp(vec - max(vec)))))),
        "-")
  
}


# This function generates a sample from the model defined by the object output returned
# by the R package Rmixtcomp
rlogtikmixtcomp <- function(output, sample.size){
  computelogtikmixtcomp(output)
}




# This function returns the log-probabilites of classification for each observation
# It takes one input argument (named output) returned by the function Rmixcomp
# It returns a matrix of the logarithm of the probabilities of classification
# Rows correspond to observations and column correspond to component
# Here, few types of data are allowed: continuous or categorical
# The other types of data must be implemented
logmixcomp <- function(output){
  param <- output$variable$param
  x <- output$variable$data
  logtik <- matrix(0, length(x$z_class$completed), output$mixture$nbCluster)
  for (k in 1:output$mixture$nbCluster)  logtik[,k] <- log(param$z_class$pi$stat[k,1])
  
  for (j in 2:length(x)){
    if (output$variable$type[[j]] == "Gaussian_sjk"){
      for (k in 1:ncol(logtik)){
        tmp <- matrix(param[[j]]$NumericalParam$stat[,1], 2, output$mixture$nbCluster)
        logtik[,k] <- logtik[,k] + dnorm(x[[j]]$completed, tmp[1,k], tmp[2,k], log=TRUE)
      }
    }
    if (output$variable$type[[j]] == "Categorical_pjk"){
      for (k in 1:ncol(logtik)){
        tmp <- log(matrix(param[[j]]$NumericalParam$stat[,1], ncol=output$mixture$nbCluster))
        logtik[,k] <- logtik[,k] + tmp[x[[j]]$completed, k]
      }
    }
    
  }
  logtik <- sweep(logtik, 1, apply(logtik, 1, max), "-")
  logtik <- sweep(logtik, 1, log(rowSums(exp(logtik))), "-")
  logtik
}

# This function returns the log-density per component of a single observation "x" 
# according to its "type", its parameters "p" for the "g" components
# Here, few types of data are allowed: continuous or categorical
# The other types of data must be implemented
dlogsinglemixcomp <- function(x, p, type, g){
  dlog <- rep(0, g)
  if (type=="Gaussian_sjk"){
    p <- matrix(p, g, 2)
    dlog <- dnorm(x, p[,1], p[,2], log = TRUE)
  }else if (type == "Categorical_pjk"){
    m <- length(p) / g
    p <- matrix(p, m, g)
    dlog <- log(p[x,])
  }else{
    stop("this type of variable is not implemented")
  }
  dlog
}
# This function samples a single observation from the component "z" 
# for the variable of type "type" and parameters "p".
# Here, few types of data are allowed: continuous or categorical
# The other types of data must be implemented
rsinglemixcomp <- function(p, type, z, g, Tseq=NULL){
  x <- 0
  if (type=="Gaussian_sjk"){
    x <- rnorm(1, p[1+(z-1)*2], p[2+(z-1)*2])
  }else if (type == "Categorical_pjk"){
    m <- length(p) / g
    x <- sample(1:m, 1, prob=p[m*(z-1) + (1:m)])
  }else {
    stop("this type of variable is not implemented")
    
    
  }
  x
}

# This function generates a sample from the model defined by the object output returned
# by the R package Rmixtcomp
rmixcomp <- function(output){
  z <- sample(1:output$mixture$nbCluster, 1, prob=output$variable$param$z_class$pi$stat[,1])
  x <- sapply(1:(length(output$variable$param)-1),
              function(j, output, z) rsinglemixcomp(output$variable$param[[j+1]]$NumericalParam$stat[,1], output$variable$type[[j+1]], z, output$mixture$nbCluster),
              output=output,
              z=z)
  names(x) <- names(output$variable$type)[-1]
  x
}

# This function generates a sample of the logarithm of probabilities of classification
# defined by the model and its parameters given by the object output returned
# by the R package Rmixtcomp
rlogtikmixcomp <- function(output){
  x=rmixcomp(output)
  dlog <- rowSums(sapply(1:(length(output$variable$param)-1),
                         function(j, output, x) dlogsinglemixcomp(x[j], output$variable$param[[j+1]]$NumericalParam$stat[,1], output$variable$type[[j+1]], output$mixture$nbCluster),
                         output=output,
                         x=x)) + log(output$variable$param$z_class$pi$stat[,1])
  dlog <- dlog - max(dlog)
  dlog <- dlog - log(sum(exp(dlog)))
  return(dlog)
}

