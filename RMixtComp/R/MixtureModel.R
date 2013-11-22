setClass(
  Class="MixtureModel",
  representation=representation(
    data = "matrix",
    model = "character"
  )
)

setMethod(
  f="initialize",
  signature=c("MixtureModel"),
  definition=function(.Object,data,model){ 
    .Object@data<-data
    .Object@model<-model
#    validObject(.this)
    return(.Object)
  }
)