mixtCompResults <- function(){
  # create a new instance of Results
  new("Results")
}

setClass(
  Class="Results",
  representation=representation(
    nbCluster = "numeric",
    lnlikelihood = "numeric",
    proportions = "numeric",
    partition = "integer",
    proba = "matrix"
  ),
  prototype=prototype(
    nbCluster = numeric(0),
    lnlikelihood = numeric(0),
    proportions = numeric(0),
    partition = integer(0),
    proba = matrix(nrow=0,ncol=0)
  )
)

setMethod(
  f="show",
  signature=c("Results"),
  function(object){
    cat("****************************************\n")
    cat("*** MixtComp Results:\n")
    cat("* nbCluster   = ", object@nbCluster,"\n")
    cat("* likelihood  = ", object@lnlikelihood, "\n")
    cat("* proportions = ", object@proportions, "\n")
    cat("* partition   = ", object@partition, "\n")
    cat("* proba       = ", object@proba, "\n")
    cat("****************************************\n")
    }
)