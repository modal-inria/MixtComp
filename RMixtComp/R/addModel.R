addModel <- function (lm, data, model){
  if (!is.na(match(model, listModels)))
  {
    augData <- new("AugmentedData", data)
    if (length(lm) == 0)
    {
      lm[[1]] <- new("MixtureModel", augData, model, "double")
    }
    else
    {
      if (nrow(lm[[1]]@augData@data) == nrow(data))
      {
        lm[[length(lm)+1]] <- new("MixtureModel", augData, model, "double")
      }
      else
      {
        message( "New ingredient has "
               , nrow(data)
               , " samples while the data currently in the mixture has "
               , nrow(lm[[1]]@data)
               , " samples. New ingredient has not been added.")
      }
    }
  }
  else
  {
    message("Model ", model, " is not yet supported. New ingredient has not been added.")
  }
  return(lm)
}