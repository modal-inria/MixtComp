setClass(
  Class="Strategy",
  representation=representation(
    initMethod = "character",
    nbTrialInInit = "numeric",
    nbBurnInIter = "numeric",
    nbIter = "numeric"
  ),
  prototype=prototype(
    initMethod = "randomInit",
    nbTrialInInit = 2.,
    nbBurnInIter = 20.,
    nbIter = 100.
  )
)

setMethod(
  f="initialize",
  signature=c("Strategy"),
  definition=function(.Object, initMethod, nbTrialInInit, nbBurnInIter, nbIter){
    .Object@initMethod<-initMethod
    .Object@nbTrialInInit<-nbTrialInInit
    .Object@nbBurnInIter<-nbBurnInIter
    .Object@nbIter<-nbIter
    return(.Object)
  }
)

setMethod(
  f="print",
  signature=c("Strategy"),
  function(x,...){
    cat("****************************************\n")
    cat("*** MixtComp Strategy:\n")
    cat("* initialization method             = ", x@initMethod, "\n")
    cat("* number of tries in initialization = ", x@nbTrialInInit, "\n")
    cat("* number of iterations in burn-in   = ", x@nbBurnInIter, "\n")
    cat("* number of iterations              = ", x@nbIter, "\n")
    cat("****************************************\n")
  }
)

setMethod(
  f="show",
  signature=c("Strategy"),
  function(object){
    cat("****************************************\n")
    cat("*** MixtComp Strategy:\n")
    cat("* initialization method             = ", object@initMethod, "\n")
    cat("* number of tries in initialization = ", object@nbTrialInInit, "\n")
    cat("* number of iterations in burn-in   = ", object@nbBurnInIter, "\n")
    cat("* number of iterations              = ", object@nbIter, "\n")
    cat("****************************************\n")
  }
)