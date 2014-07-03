mixtCompSemStrategy <- function( nbTrialInInit = 2, nbBurnInIter = 20, nbIter = 100, nbGibbsIter = 100 ){
  # create a new instance of SEM strategy
  return(new("Strategy", "randomInit", nbTrialInInit, nbBurnInIter, nbIter, nbGibbsIter))
}

setClass(
  Class="Strategy",
  representation=representation(
    initMethod = "character",
    nbTrialInInit = "numeric",
    nbBurnInIter = "numeric",
    nbIter = "numeric",
    nbGibbsIter = "numeric"
  ),
  prototype=prototype(
    initMethod = "randomInit",
    nbTrialInInit = 2,
    nbBurnInIter = 20,
    nbIter = 100,
    nbGibbsIter = 100
  ),
  # validity function
  validity=function(object){
    # for 'initMethod'
    if(!is.na(match(object@initMethod,listModels))){
      stop("initMethod unknown.")
    }
    # for 'nbTrialInInit'
    if (!is.wholenumber(object@nbTrialInInit)){
      stop("nbTrialInInit must be an integer.")
    }
    if (object@nbTrialInInit < 1){
      stop("nbTrialInInit must be positive.")
    }
    # for 'nbBurnInIter'
    if (!is.wholenumber(object@nbBurnInIter)){
      stop("nbBurnInIter must be an integer.")
    }
    if (object@nbBurnInIter < 1){
      stop("nbBurnInIter must be positive.")
    }
    # for 'nbIter'
    if (!is.wholenumber(object@nbIter)){
      stop("nbIter must be an integer.")
    }
    if (object@nbIter < 1){
      stop("nbIter must be positive.")
    }
    # for 'nbGibbsInIter'
    if (!is.wholenumber(object@nbGibbsIter)){
      stop("nbBurnInIter must be an integer.")
    }
    if (object@nbGibbsIter < 1){
      stop("nbBurnInIter must be positive.")
    }
    return(TRUE)
  }
)

setMethod(
  f="initialize",
  signature=c("Strategy"),
  definition=function(.Object, initMethod, nbTrialInInit, nbBurnInIter, nbIter, nbGibbsIter){
    .Object@initMethod<-initMethod
    .Object@nbTrialInInit<-nbTrialInInit
    .Object@nbBurnInIter<-nbBurnInIter
    .Object@nbIter<-nbIter
    .Object@nbGibbsIter<-nbGibbsIter
    validObject(.Object)
    return(.Object)
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
    cat("* number of Gibbs iterations        = ", object@nbGibbsIter, "\n")
    cat("****************************************\n")
  }
)