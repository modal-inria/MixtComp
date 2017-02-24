logToMulti <- function(logIn) {
  max <- max(logIn);
  multiOut <- logIn - max
  multiOut <- exp(multiOut)
  sum <- sum(multiOut)
  multiOut <- multiOut / sum;

  return(multiOut)
}