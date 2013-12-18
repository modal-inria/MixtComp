setClass(
  Class ="MixtureModel",
  representation = representation(
  augData = "list",
  model = "character",
  id = "character",
  type = "character"
  )
)

setMethod(
  f = "initialize",
  signature = c("MixtureModel"),
  definition = function(.Object, augData, model, id, type){ 
    .Object@augData <- augData
    .Object@model   <- model
    .Object@id      <- id
    .Object@type    <- type
    return(.Object)
  }
)