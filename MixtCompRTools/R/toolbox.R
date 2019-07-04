# MixtComp version 4.0  - july 2019
# Copyright (C) Inria - Université de Lille - CNRS

#' Convert a mixtcomp string into 2-columns matrix
#'
#' @param x a string containing a fonctional observation (cf example)
#'
#' @return matrix
#'
#' @examples
#' convertFunctional("1:5,1.5:12,1.999:2.9")
#'
#' @author Quentin Grimonprez
#'
#' @export
convertFunctional <- function(x){
  return(do.call(rbind, lapply(strsplit(strsplit(as.character(x), ",")[[1]], ":"), as.numeric)))
}

#' Create a functional in MixtComp format
#'
#' @param time vector containing the time of the functional
#' @param value vector containing the value of the functional
#'
#' @return The functional data formatted to the mixtcomp standard
#'
#' @examples
#' mat <- matrix(c(1, 2, 3, 9, 1, 1.5, 15, 1000), ncol=2)
#' createFunctional(mat[,1], mat[,2])
#'
#' @author Quentin Grimonprez
#'
#' @export
createFunctional <- function(time, value)
{
  return(paste(paste(time, value, sep = ":"), collapse = ","))
}


#' Plot functional data
#'
#' @param x Functional data in mixtcomp string format
#' @param ... additional plot parameter
#'
#'
#' @examples
#' plotFunctional("1:5,3:2,6:9.78,9:4,12:25")
#'
#' @author Quentin Grimonprez
#'
#' @export
plotFunctional <- function(x, ...)
{
  newX <- convertFunctional(x)

  return(plot(newX[,1], newX[,2], type = "s", ...))
}


#' lines functional data
#'
#' @param x Functional data in mixtcomp string format
#' @param ... additional plot parameter
#'
#' @examples
#' plotFunctional("1:5,3:2,6:9.78,9:4,12:25")
#' linesFunctional("1:7,3:4,6:11.78,9:5,12:12")
#'
#'
#' @author Quentin Grimonprez
#'
#' @export
linesFunctional <- function(x, ...)
{
  newX <- convertFunctional(x)

  return(lines(newX[,1], newX[,2], type = "s", ...))
}


# refactor categorical data
refactorCategoricalData <- function(data, ancienneCategorie, nouvelleCategorie)
{
  dataTemp <- nouvelleCategorie[match(data, ancienneCategorie)]

  return(dataTemp)
}

# rand index between two partitions
rand.index <- function (group1, group2)
{
  x <- abs(sapply(group1, function(x) x - group1))
  x[x > 1] <- 1
  y <- abs(sapply(group2, function(x) x - group2))
  y[y > 1] <- 1
  sg <- sum(abs(x - y))/2
  bc <- choose(dim(x)[1], 2)
  ri <- 1 - sg/bc
  return(ri)
}
