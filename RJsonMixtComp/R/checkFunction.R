# JsonMixtComp does not work with Functional and Rank data
checkModel <- function(model)
{
  if("Functional"%in%model)
    stop("Functional data are not  supported by RJsonMixtComp.")
  if("Rank"%in%model)
    stop("Rank data are not  supported by RJsonMixtComp.")
  
  return(TRUE)
}


# Path of different files must not contain : or ~
# ... path to the different files
checkPath <- function(...)
{
  argList <- list(...)
  
  if(any(grepl(":", argList)))
    stop("File path must not contain the character ':'.")
  if(any(grepl("~", argList)))
    stop("File path must not contain the character '~'.")
 
  return(TRUE) 
}