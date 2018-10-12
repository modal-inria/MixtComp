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
  data = formatData(data)
  toJSON(data, auto_unbox = FALSE, pretty = TRUE)
}



# @param descriptor a data.frame where each column corresponds to a variable. Each column contains the model and eventually supplementary parameters
createDescriptorJson <- function(descriptor)
{
  descriptor <- formatDesc(descriptor)
  toJSON(descriptor, auto_unbox = TRUE, pretty = TRUE)
}