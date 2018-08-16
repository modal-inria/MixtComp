
#' Convert a mixtcomp string into 2-columns matrix
#'
#' @param x a string containing a fonctional observation (cf example)
#'
#' @return matrix
#'
#' @examples
#' \dontrun{
#' convertFunctional("1:5,1.5:12,1.999:2.9")}
convertFunctional <- function(x){
  return(do.call(rbind, lapply(strsplit(strsplit(as.character(x), ",")[[1]], ":"), as.numeric)))
}

#' créé le format fonctionnel de mixtcomp
#'
#' @param temps vector of numeric
#' @param valeur vector of numeric
#'
#' @return The functional data formatted to the mixtcomp standard
#'
#' @examples
#' \dontrun{
#' Mat = matrix(c(1,2,3,9,1,1.5,15,1000),ncol=2)
#' convertFunctional(Mat[,1],Mat[,2])}
createFunctional <- function(temps, valeur)
{
  return(paste(paste(temps, valeur, sep = ":"), collapse = ","))
}


#' PLot functional data
#'
#' @param x Functional data in mixtcomp string format
#' @param ... additional plot parameter
#'
#' @return void
#'
#' @examples
#' \dontrun{
#' plotFunctional("1:5,3:2,6:9.78,9:4,12:25")
#' }
plotFunctional <- function(x, ...)
{
  newX <- convertFunctional(x)

  return(graphics::plot(newX[,1], newX[,2], type = "s", ...))
}


#' lines functional data
#'
#' @param x Functional data in mixtcomp string format
#' @param ... additional plot parameter
#'
#' @return void
#' @examples
#' \dontrun{
#' #' plotFunctional("1:5,3:2,6:9.78,9:4,12:25")
#' linesFunctional("1:7,3:4,6:11.78,9:5,12:12")
#' }
linesFunctional <- function(x, ...)
{
  newX <- convertFunctional(x)

  return(graphics::lines(newX[,1], newX[,2], type = "s", ...))
}

