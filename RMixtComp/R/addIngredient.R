addIngredient <- function (lm, data, model){
  lm[length(lm)+1] <- new("MixtureModel", as.matrix(data), model, typeof(data))
  return(lm)
}