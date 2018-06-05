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

clusvisMixtcomp <- function(resmixtcomp, sample.size=5000, maxit=10**3, nbrandomInit=12, nbcpu=1){
  #logtik.estim <- rlogtikmixtcomp(resmixtcomp)
  out <- clusvis(computelogtikmixtcomp(resmixtcomp), prop=resmixtcomp$variable$param$z_class$pi$stat[,1], logtik.obs=computelogtikmixtcomp(resmixtcomp), maxit, nbrandomInit, nbcpu)
  return(out)
}

###################################################################################
##' This function is a wrapper around the plotDensityClusVisu function of the ClusVisu package. It generates
##' a component interpretation graph using the visualization information returned by the clusvisMixtcomp function.
##'
##'
##' @param visInfo list. The parameters estimated by the clusvisMixtcomp function.
##'
plotComponentClusVis <- function(visInfo) {
  plotDensityClusVisu(visInfo, add.obs=F, positionlegend="bottomright", xlim=range(visInfo$centers[,1])+c(-4,6), ylim=range(visInfo$centers[,2])+c(-6,4))
}

###################################################################################
##' This function is a wrapper around the plotDensityClusVisu function of the ClusVisu package. It generates
##' a scatter-plot of the observation memberships using the visualization information returned by the clusvisMixtcomp function.
##'
##'
##' @param visInfo list. The parameters estimated by the clusvisMixtcomp function.
##'
plotObservationsClusVis <- function(visInfo) {
  plotDensityClusVisu(visInfo, add.obs=T, positionlegend="bottomright", xlim=range(visInfo$y[,1])+c(0,2), ylim=range(visInfo$y[,2])+c(-2,0))
}