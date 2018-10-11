createAlgoJson <- function(nClass, nInd, mcStrategy, mode, ...)
{
  unrequiredFields <- list(...)
  toDelete <- names(unrequiredFields) %in% c("nClass", "nInd", "confidenceLevel", "mode", "nbBurnInIter",
                                                "nbIter", "nbGibbsBurnInIter", "nbGibbsIter", "nInitPerClass", "nSemTry")
  unrequiredFields = unrequiredFields[!toDelete]
  
  algo = c(list(nClass = nClass, nInd = nInd), mcStrategy,
           list(mode = mode), unrequiredFields)
  
  return(toJSON(algo, auto_unbox = TRUE, pretty = TRUE))
}

# @param data a data.frame where each column corresponds to a variable 
createDataJson <- function(data)
{
  data[,] = lapply(data[,], as.character)
  toJSON(data, dataframe = "columns", auto_unbox = FALSE, pretty = TRUE)
}


convertDescriptorToList <- function(descriptor)
{
  lapply(descriptor, function(x)
  {
    element <- as.list(as.character(x))
    
    if(length(element) == 1) # if paramStr is not given
      element = c(element, "")
    
    names(element) = c("type", "paramStr")
    
    return(element)
  })
}

# @param descriptor a data.frame where each column corresponds to a variable. Each column contains the model and eventually supplementary parameters
createDescriptorJson <- function(descriptor)
{
  descriptor <- convertDescriptorToList(descriptor)
  toJSON(descriptor, auto_unbox = TRUE, pretty = TRUE)
}