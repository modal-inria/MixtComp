setClass(
  Class ="MixtureModel",
  representation = representation(
  augData = "list",
  model = "character",
  type = "character"
  )
)

setMethod(
  f = "initialize",
  signature = c("MixtureModel"),
  definition = function(.Object, augData, model, type){ 
    .Object@augData <- augData
    .Object@model <- model
    .Object@type <- type
    return(.Object)
  }
)