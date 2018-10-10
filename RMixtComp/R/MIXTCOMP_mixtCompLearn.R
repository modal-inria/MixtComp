
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