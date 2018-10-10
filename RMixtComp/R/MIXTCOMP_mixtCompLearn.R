
# format the descriptor list for rmc function:
# - add paramStr when missing
# - ensure the list format of each element
formatDesc <- function(desc)
{
  desc = lapply(desc, function(x){
    if(!is.list(x))
      x = list(type = x)
    if(!("paramStr" %in% names(x)))
      x$paramStr = ""
    
    return(x)
  })
  
  return(desc)
}

# format data.frame or matrix in list of character
# keep list in list format
formatData <- function(data)
{
  if(is.data.frame(data) || is.matrix(data))
  {
    out <- lapply(1:ncol(data), function(i) as.character(data[,i]))
    names(out) = colnames(data)
  }else{
    out <- data
  }

  return(out)
}


# check the number of class given by the user in mixtCompPredict
checkNClass <- function(nClass, resLearn)
{
  if(is.null(nClass))
    nClass = resLearn$algo$nClass
  
  if(length(nClass) > 1)
  {
    nClass = nClass[1]
    warning("Several nClass given. Only the first is used.")
  }
  
  if("MixtCompLearn" %in% class(resLearn))
  {
    if(!(nClass %in% resLearn$nClass))
    {
      nClass = resLearn$algo$nClass
      warning("The provided value of nClass does not match any results in the resLearn object. nClass is set to the value with the best criterion value.")
    }
  }else{
    if(nClass != resLearn$algo$nClass)
    {
      nClass = resLearn$algo$nClass
      warning("The provided value of nClass does not match the value in the resLearn object. nClass is set to the value of the resLearn object.")
    }
  }
  
  return(nClass)
}
