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
