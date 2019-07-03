# MixtComp version 4 - july 2019
# Copyright (C) Inria - Université de Lille - CNRS 

#' Convert a mixtcomp string into 2-columns matrix
#'
#' @param x a string containing a fonctional observation (cf example)
#'
#' @return matrix
#'
#' @examples
#' convertFunctionalToVector("1:5,1.5:12,1.999:2.9")
#'
#' @author Quentin Grimonprez
#' @export
convertFunctionalToVector <- function(x){
  if((x == "") || is.na(x) || is.null(x))
    return(list(time = c(), value = c()))
  
  mat <- sapply(strsplit(strsplit(as.character(x), ",")[[1]], ":"), as.numeric)
  
  return(list(time = mat[1,], value = mat[2,]))
}


#' Create a functional in MixtComp format
#'
#' @param time vector containing the time of the functional
#' @param value vector containing the value of the functional
#'
#' @return The functional data formatted to the mixtcomp standard
#'
#' @examples
#' mat <- matrix(c(1, 2, 3, 9, 1, 1.5, 15, 1000), ncol = 2)
#' createFunctional(mat[,1], mat[,2])
#'
#' @author Quentin Grimonprez
#' @export
createFunctional <- function(time, value)
{
  l <- min(length(time), length(value))
  if((l != length(time)) | (l != length(value)))
    warning("time and value do not have the same length.")
  toKeep <- !is.na(value[1:l]) & !is.na(time[1:l])
  return(paste(paste(time[1:l][toKeep], value[1:l][toKeep], sep = ":"), collapse = ","))
}



#' Rename a categorical value
#'
#' @param data matrix/data.frame/vector containing the data
#' @param oldCateg vector containing categories to change
#' @param newCateg vector containing new categorical values
#'
#' @return Data with new categorical values
#'
#' @examples
#' dat <- c("single", "married", "married", "divorced", "single")
#' refactorCategorical(dat, c("single", "married", "divorced"), 1:3)
#'
#' @author Quentin Grimonprez
#' @export
refactorCategorical <- function(data, oldCateg = unique(data), newCateg = 1:length(oldCateg))
{
  ind <- match(data, oldCateg)
  
  if(any(is.na(ind[!is.na(data)])))
    warning("NA produced.")
  
  return(newCateg[ind])
}