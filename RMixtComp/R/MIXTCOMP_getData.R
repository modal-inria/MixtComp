
#' @title Import data from a csv file
#' 
#' @description Import the data file and the descriptor file in the format required by \link{mixtCompCluster} and \link{mixtCompPredict} functions.
#'
#' @param ... a vector containing the names of the two files to import (see Details).
#' 
#' @return a list containing :
#' \describe{
#'   \item{lm}{imported data for \link{mixtCompCluster} and \link{mixtCompPredict}}
#'   \item{warnlog}{warning message}
#' }
#' 
#' 
#' @details 
#' The functions \link{mixtCompCluster} and \link{mixtCompPredict} require two files : a data file and a descriptor file.
#' 
#' 
#' The data file is a csv file (; separator), each column corresponds to a variable. The first row contains the name of each variable.
#' Others rows correspond to the different samples. See the section associated with each model for more details about data format.
#' 
#' 
#' The descriptor file is a csv file (; separator) containing 3 rows. The first row contains the names of the variables.
#' This first row must be the same as the first row of the data file. The second row contains the model associated with each variables.
#' Six models are available in RMixtComp : Gaussian_sjk, Categorical_pjk, Poisson_k, Functional, Rank, Ordinal.
#' The third row is optional and contains parameters associated with each model. 
#'
#' @section Gaussian data:
#' Gaussian data are real values with the dot as decimal separator.
#' Missing data are indicated by a \emph{?}. Partial data can be provided through intervals denoted by 
#' \emph{[a:b]} where \emph{a} (resp. \emph{b}) is a real or \emph{-inf} (resp. \emph{+inf}).
#' 
#' 
#' @section Categorical Data:
#' Categorical data must be consecutive integer with 1 as minimal value. Missing data are indicated by a \emph{?}.
#' For partial data, a list of possible values can be provided by \emph{{a_1,\dots,a_j}}, 
#' where \emph{a_i} denotes a categorical value.
#' 
#' @section Poisson Data:
#' Poisson data must be positive integer. Missing data are indicated by a \emph{?}.
#' 
#' @section Ordinal Data:
#' Ordinal data must be consecutive integer with 0 as minimal value. Missing data are indicated by a \emph{?}.
#' For partial data, a list of possible values can be provided by \emph{{a_1,\dots,a_j}}, 
#' where \emph{a_i} denotes a categorical value.
#' 
#' @section Rank data:
#' TODO
#' 
#' @section Functional data:
#' In the case of a functional model, \emph{nSub: i, nCoeff: j} must be indicated in the third row of the descriptor file.
#' \emph{i} is the number of subregressions in a functional data and \emph{j} the number of coefficients
#'  of each regression (2 = linear, 3 = quadratic, ...).
#'
#' 
#' 
#' 
#'
#' @examples 
#' \dontrun{
#' dat <- getData(c("data.csv", "descriptor.csv")) 
#' }
#'
#' @seealso \link{mixtCompCluster} \link{mixtCompPredict}
#' 
#' @author Vincent Kubicki
getData <- function(...) {
  argList <- list(...)
  lm <- list()
  warnLog = ""
  
  for (k in 1:length(argList)) {
    dataFile <- argList[[k]][1]
    descriptorFile <- argList[[k]][2]
    data <- read.table(file = dataFile,
                       header = TRUE,
                       sep = ";",
                       quote = "\"",
                       dec = ".",
                       fill = FALSE,
                       comment.char = "",
                       stringsAsFactors = FALSE,
                       na.strings = "")
    descriptor <- read.table(file = descriptorFile,
                             header = FALSE,
                             sep = ";",
                             quote = "\"",
                             dec = ".",
                             fill = FALSE,
                             comment.char = "",
                             stringsAsFactors = FALSE)
    for (i in 1:length(descriptor)) { # loop over columns of the descriptor
      currId <- descriptor[1, i]
      currModel <- descriptor[2, i]
      if (dim(descriptor)[1] == 3) {
        param <- descriptor[3, i] # a character string describing the parameter space, that will be parsed directly by the model
      }
      else {
        param <- "" # a zero-sized parameter string will be interpreted by the model as "deduce the parameter space automatically"
      }
      if (! descriptor[1, i] %in% names(data)) { # descriptor does not match data
        warnLog <- paste(warnLog,
                         "Variable ", currId, " asked for in descriptor file ",
                         "but absent from the data file.\n",
                         sep = "")
      }
      else {
        # perform length check and generate named list for current variable
        res <- addVariable(lm,
                           data[[currId]],
                           currId, # id
                           currModel, # model
                           param)
        lm <- res$lm # this discards the warnLog output by addVariable
        warnLog <- paste(warnLog,
                         res$warnLog,
                         sep = "")
      }
    }
  }

  return(list(lm = lm,
              warnLog = warnLog))
}