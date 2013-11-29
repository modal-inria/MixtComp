getMixtCompCluster <- function(strategy){
  # create a new MixtCompCluster
  new("MixtCompCluster", strategy, new("Results"))
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