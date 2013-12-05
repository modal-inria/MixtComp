getData <- function(dataFile, descriptorFile){
  lm <- list()
  data <- read.table(file = dataFile,
                         header = TRUE,
                         sep = ";",
                         quote = "\"",
                         dec = ".",
                         fill = FALSE,
                         comment.char = "",
                         stringsAsFactors = FALSE)
  descriptors <- read.table(file = descriptorFile,
                            header = FALSE,
                            sep = ";",
                            quote = "\"",
                            dec = ".",
                            fill = FALSE,
                            comment.char = "",
                            stringsAsFactors = FALSE)
    for (i in 1:length(data)){
      lm <- addIngredient(lm, data[i], descriptors[1,i])
    }
  return(lm)
}

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