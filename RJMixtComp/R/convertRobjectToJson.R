# @author Quentin Grimonprez
createAlgoJson <- function(nClass, nInd, mcStrategy, mode)
{
  toKeep <- names(mcStrategy) %in% names(createAlgo())
  unrequiredFields = mcStrategy[!toKeep]
  
  algo = c(list(nClass = nClass, nInd = nInd), completeAlgo(mcStrategy[toKeep]),
           list(mode = mode), unrequiredFields)
  
  return(toJSON(algo, auto_unbox = TRUE, pretty = TRUE))
}

# @param data a data.frame where each column corresponds to a variable 
# @author Quentin Grimonprez
createDataJson <- function(data)
{
  data = formatData(data)
  toJSON(data, auto_unbox = FALSE, pretty = TRUE)
}



# @param descriptor a data.frame where each column corresponds to a variable. Each column contains the model and eventually supplementary parameters
# @author Quentin Grimonprez
createModelJson <- function(model)
{
  model <- formatModel(model)
  toJSON(model, auto_unbox = TRUE, pretty = TRUE)
}