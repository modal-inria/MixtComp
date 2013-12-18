addVariable <- function (lm, data, model, id){
  if (!is.na(match(model, listModels)))
  {
    augData <- augmentedData(data)
    if (length(lm) == 0)
    {
      show("lm = 0")
      lm[[1]] <- new("MixtureModel",
                     augData,
                     model,
                     id,
                     "double")
    }
    else
    {
      if (length(lm[[1]]@augData$data) == length(augData$data))
      {
        show("lm > 0")
        lm[[length(lm)+1]] <- new("MixtureModel",
                                  augData,
                                  model,
                                  id,
                                  "double")
      }
      else
      {
        message( "New variable has "
               , length(augData$data)
               , " samples while the data currently in the mixture has "
               , length(lm[[1]]["data"]) # comparison with the size of the data in the first model, first column
               , " samples. New variable has not been added.")
      }
    }
  }
  else
  {
    message("Model ", model, " is not yet supported. New variable has not been added.")
  }
  return(lm)
}