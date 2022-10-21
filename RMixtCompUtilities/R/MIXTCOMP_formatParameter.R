# MixtComp version 4 - july 2019
# Copyright (C) Inria - Université de Lille - CNRS

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>


#' @title Format the model parameter
#'
#' @description Format the model list for rmc/rmcMultiRun functions:
#' - add paramStr when missing
#' - ensure the list format of each element
#'
#' @param model description of model used per variable
#'
#' @return model as a list where each element is the model applied to a variable (list with elements type and paramStr)
#'
#' @author Quentin Grimonprez
#'
#' @export
formatModel <- function(model) {
  if (("matrix" %in% class(model)) || ("data.frame" %in% class(model))) {
    model <- apply(model, 2, function(x) {
      out <- list(type = as.character(x[1]))
      if (length(x) >= 2) {
        out$paramStr <- as.character(x[2])
      }

      out
    })
  } else {
    model <- lapply(model, function(x) {
      if (!is.list(x)) {
        x <- list(type = as.character(x))
      }
      if (!("paramStr" %in% names(x))) {
        x$paramStr <- ""
      }

      return(x)
    })
  }

  return(model)
}


#' @title Format the data parameter required by rmc
#'
#' @description format data.frame or matrix in list of character
#'
#' @param data data parameter as data.frame, matrix or list
#'
#' @return data as a list of characters
#'
#' @author Quentin Grimonprez
#'
#' @export
formatData <- function(data) {
  if (is.data.frame(data) || is.matrix(data)) {
    out <- lapply(seq_len(ncol(data)), function(i) as.character(data[, i]))
    names(out) <- colnames(data)
  } else {
    out <- lapply(data, as.character)
  }

  return(out)
}


#' @title Add the missing element to algo parameter
#'
#' @description Add the missing element to algo parameter with default values
#'
#' @param algo a list with the different algo parameters for rmc function
#'
#' @return algo parameter with all required elements (see \link{createAlgo} function)
#'
#' @author Quentin Grimonprez
#'
#' @export
completeAlgo <- function(algo) {
  algoDefault <- createAlgo()

  missingNames <- setdiff(names(algoDefault), names(algo))

  return(c(algo, algoDefault[missingNames]))
}
