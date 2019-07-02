
# format the model list for rmc function:
# - add paramStr when missing
# - ensure the list format of each element
# @author Quentin Grimonprez
formatModel <- function(model)
{
  model = lapply(model, function(x){
    if(!is.list(x))
      x = list(type = x)
    if(!("paramStr" %in% names(x)))
      x$paramStr = ""
    
    return(x)
  })
  
  return(model)
}


# format data.frame or matrix in list of character
# keep list in list format
# @author Quentin Grimonprez
formatData <- function(data)
{
  if(is.data.frame(data) || is.matrix(data))
  {
    out <- lapply(1:ncol(data), function(i) as.character(data[,i]))
    names(out) = colnames(data)
  }else{
    out <- lapply(data, as.character)
  }
  
  return(out)
}


# @author Quentin Grimonprez
completeAlgo <- function(algo)
{
  algoDefault <- createAlgo()
  
  missingNames <- setdiff(names(algoDefault), names(algo))
  
  return(c(algo, algoDefault[missingNames]))
}
