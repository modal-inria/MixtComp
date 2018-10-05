# @param x result from MixtComp read with fromJSON
convertOutput <- function(x)
{
  x = lapply(x, convertJsonElement)
  if(any(sapply(x, is.list)))
  {
    x[sapply(x, is.list)] = lapply(x[sapply(x, is.list)], convertOutput)
  }
  
  return(x)
}


convertJsonElement <- function(x)
{
  if("ctype" %in% names(x))
  {
    x <- switch(x$ctype,
                "Matrix" = {
                  if((x$nrow == 0) | (x$ncol = 0))
                    return(matrix(nrow = x$nrow, ncol = x$ncol, dimnames = list(x$rowNames, x$colNames)))
                  
                  rownames(x$data) = x$rowNames
                  colnames(x$data) = x$colNames
                  
                  return(x$data)
                }, 
                "Vector" = {
                  if(x$nrow == 0)
                    return(c())
                  if(length(x$rowNames) != 0) 
                    names(x$data) = x$rowNames
                  
                  return(x$data)
                })
  }
  return(x)
}
