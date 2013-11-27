listModels <- c( "Gamma_ajk_bjk"
               , "Gamma_ajk_bj"
               , "Gaussian_sjk"
               , "Gaussian_s")

addIngredient <- function (lm, data, model){
  if (!is.na(match(model,listModels)))
  {
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