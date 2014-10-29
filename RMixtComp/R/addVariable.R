addVariable <- function (lm, data, model, id){
  if (!is.na(match(model, listModels)))
  {
    if (length(lm) == 0)
    {
      show("lm = 0")
      lm[[1]] <- list(data = data,
                      model = model,
                      id = id)
    }
    else
    {
      if (length(lm[[1]]$data) == length(data))
      {
        show("lm > 0")
        lm[[length(lm)+1]] <- list(data = data,
                                   model = model,
                                   id = id)
      }
      else
      {
        message( "New variable has "
               , length(data)
               , " samples while the data currently in the mixture has "
               , length(lm[[1]]$data) # comparison with the size of the data in the first model, first column
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