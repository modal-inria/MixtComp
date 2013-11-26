addIngredient <- function (lm, data, model){
  data = as.matrix(data)
  if (length(lm) == 0)
  {
    lm[1] <- new("MixtureModel", data, model, typeof(data))
  }
  else
  {
    if (nrow(lm[[1]]@data) == nrow(data))
    {
      lm[length(lm)+1] <- new("MixtureModel", data, model, typeof(data))
    }
    else
    {
      message("New ingredient does not have the same number of elements as other ingredient in current mixture. It has not been added.")
    }
  }
  return(lm)
}