getMixtCompCluster <- function(nbTrialInInit, nbBurnInIter, nbIter, nbGibbsBurnInIter, nbGibbsIter){
  # create a new Sem Strategy
  mcStrategy <- new("Strategy", "randomClassInit", nbTrialInInit, nbBurnInIter, nbIter, nbGibbsBurnInIter, nbGibbsIter)
  # create a new MixtCompCluster
  return(new("MixtCompCluster", mcStrategy, new("Results")))
}

setClass(
  Class="MixtCompCluster",
  representation=representation(
    strategy = "Strategy",
    results = "Results"
  ),
  prototype=prototype(
  )
)

setMethod(
  f="initialize",
  signature=c("MixtCompCluster"),
  definition=function(.Object, strategy, results){
    .Object@strategy<-strategy
    .Object@results<-results
    return(.Object)
  }
)

setMethod(
  f="show",
  signature=c("MixtCompCluster"),
  function(object){ 
    show(object@strategy)
    show(object@results)
  }
)