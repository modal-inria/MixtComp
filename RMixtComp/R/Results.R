setClass(
  Class="Results",
  representation=representation(
    nbCluster = "numeric",
    model = "character",
    lnlikelihood = "numeric",
    proportions = "numeric",
    partition = "integer",
    proba = "matrix"
  ),
  prototype=prototype(
    nbCluster = numeric(0),
    model = character(0),
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
    cat("* nbCluster   = ", object@nbCluster,"\n")      
    cat("* model name  = ", object@model, "\n")
    cat("* likelihood  = ", object@lnlikelihood, "\n")
    cat("* proportions = ", object@proportions, "\n")
    cat("* partition   = ", object@partition, "\n")
    cat("* proba       = ", object@proba, "\n")
    }
)