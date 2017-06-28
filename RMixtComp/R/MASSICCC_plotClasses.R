# This function returns the log-probabilites of classification for each observation
# It takes one input argument (named output) returned by the function Rmixcomp
# It returns a matrix of the logarithm of the probabilities of classification
# Rows correspond to observations and column correspond to component
# Here, few types of data are allowed: continuous or categorical
# The other types of data must be implemented
computelogtikmixcomp <- function(output){
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


# This function generates a sample from the model defined by the object output returned
# by the R package Rmixtcomp
rlogtikmixcomp <- function(output, sample.size){
  computelogtikmixcomp(output)
}


###################################################################################
##' This function estimates the parameters used for visualization of model-based clustering performs with R package ClusVis 
##' To achieve the parameter infernece, it automatically samples probabilities of classification from the model parameters
##'
##'
##' @param resmixcomp list. It is an object return by MixComp.
##' @param sample.size numeric. Number of probabilities of classification sampled for parameter inference.
##' @param maxit numeric. It limits the number of iterations for the Quasi-Newton algorithm (default 1000).
##' @param nbrandomInit numeric. It defines the number of random initialization of the Quasi-Newton algorithm.
##' @param nbcpu numeric. It specifies the number of CPU (only for linux, R package parallel must by loaded).
##'

clusvisMixcomp <- function(resmixcomp, sample.size=5000, maxit=10**3, nbrandomInit=12, nbcpu=3){
  logtik.estim <- rlogtikmixcomp(output)
  out <- clusvis(logtik.estim, prop=resmixcomp$variable$param$z_class$pi$stat[,1], logtik.obs=computelogtikmixcomp(resmixcomp), maxit, nbrandomInit, nbcpu)
  return(out)
}