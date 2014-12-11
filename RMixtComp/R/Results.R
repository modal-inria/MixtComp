mixtCompResults <- function(){
  # create a new instance of Results
  new("Results")
}

setClass(
  Class="Results",
  representation=representation(
    nbCluster = "numeric",
    lnCompletedLikelihood = "numeric",
    lnObservedLikelihood = "numeric",
    proportions = "numeric",
    partition = "integer",
    proba = "matrix",
    runOK = "logical",
    warnLog = "character"
  ),
  prototype=prototype(
    nbCluster = numeric(0),
    lnCompletedLikelihood = numeric(0),
    lnObservedLikelihood = numeric(0),
    proportions = numeric(0),
    partition = integer(0),
    proba = matrix(nrow=0,ncol=0),
    runOK = F,
    warnLog = character(0)
  )
)

setMethod(
  f="show",
  signature=c("Results"),
  function(object)
  {
    cat("****************************************\n")
    cat("*** MixtComp Results:\n")
    cat("* number of clusters    = ", object@nbCluster,"\n")
    cat("* completed likelihood  = ", object@lnCompletedLikelihood, "\n")
    cat("* observed likelihood   = ", object@lnObservedLikelihood, "\n")
    cat("* proportions           = ", object@proportions, "\n")
    cat("* partition             = ", object@partition, "\n")
    cat("* proba                 = ", object@proba, "\n")
    cat("****************************************\n")
  }
)