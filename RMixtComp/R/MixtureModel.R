setClass(
  Class="MixtureModel",
  representation=representation(
    data = "matrix",
    model = "character",
    type = "character"
  )
)

setMethod(
  f="initialize",
  signature=c("MixtureModel"),
  definition=function(.Object,data,model,type){ 
    .Object@data<-data
    .Object@model<-model
    .Object@type<-type
    return(.Object)
  }
)