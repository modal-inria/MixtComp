addModel <- function (lm, data, model){
  if (!is.na(match(model, listModels)))
  {
    augData <- augmentedData(data)
    if (length(lm) == 0)
    {
      show("lm = 0")
      lm[[1]] <- new("MixtureModel", augData, model, "double")
    }
    else
    {
      if (nrow(lm[[1]]@augData@data) == nrow(data))
      {
        show("lm > 0")
        lm[[length(lm)+1]] <- new("MixtureModel", augData, model, "double")
      }
      else
      {
        message( "New ingredient has "
               , length(data)
               , " samples while the data currently in the mixture has "
               , length(lm[[1]]["data"]) # comparison with the size of the data in the first model, first column
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