# MixtComp version 4.0  - july 2019
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
 

# Helper function for Mixtcomp Input formatting
#
# @param lm        list as container
# @param data      Vector of data
# @param id        Name of the variable
# @param model     Name of the model : "Gaussian_pjk", ..
# @param paramStr  Additional parameters as strings
#
# @author Vincent Kubicki
#
addVariable <- function (lm, data, id, model, paramStr) {
  warnLog = "" # warnLog will contain the various possible error messages
  if (length(lm) == 0) {
    # first variable is automaticaly added
    lm[[1]] <- list(
      data = data,
      model = model,
      id = id,
      paramStr = paramStr
    )
  }
  else if (length(lm[[1]]$data) != length(data)) {
    # all variable must have the same number of individuals
    warnLog <- paste(
      warnLog,
      "Variable ",
      id,
      " has ",
      length(data),
      " individuals while other variables currently have: ",
      length(lm[[1]]$data),
      " individuals. All variables must have the same number of individuals.\n",
      sep = ""
    )
  }
  else {
    # variable is fit to be added !
    lm[[length(lm) + 1]] <- list(
      data = data,
      model = model,
      id = id,
      paramStr = paramStr
    )
  }
  return(list(lm = lm,
              warnLog = warnLog))
}

#' Transform data into Mixtcomp accepted format
#'
#' @param ... List of arguments (usually equal to c(data_path, descriptor_path))
#'
#' @return list of data
#'
#' @author Vincent Kubicki
#'
#' @export
getData <- function(...) {
  argList <- list(...)
  lm <- list()
  warnLog = ""

  for (k in 1:length(argList)) {
    dataFile <- argList[[k]][1]
    descriptorFile <- argList[[k]][2]

    if(!is.list(dataFile))
    {
      data <- read.table(file = dataFile,
                         header = TRUE,
                         sep = ";",
                         quote = "\"",
                         dec = ".",
                         fill = FALSE,
                         comment.char = "",
                         stringsAsFactors = FALSE,
                         na.strings = "")
    }else{
      data <- dataFile[[1]]
    }


    if(!is.list(descriptorFile))
    {
      descriptor <- read.table(file = descriptorFile,
                               header = TRUE,
                               sep = ";",
                               quote = "\"",
                               dec = ".",
                               fill = FALSE,
                               comment.char = "",
                               stringsAsFactors = FALSE)
    }else{
      descriptor <- descriptorFile[[1]]
    }

    nameDesc <- colnames(descriptor)

    for (i in 1:length(nameDesc)) { # loop over columns of the descriptor
      currId <- nameDesc[i]
      currModel <- descriptor[1, i]
      if (dim(descriptor)[1] == 2) {
        param <- descriptor[2, i] # a character string describing the parameter space, that will be parsed directly by the model
      }
      else {
        param <- "" # a zero-sized parameter string will be interpreted by the model as "deduce the parameter space automatically"
      }
      if (! (currId %in% colnames(data))) { # descriptor does not match data
        warnLog <- paste(warnLog,
                         "Variable ", currId, " asked for in descriptor file ",
                         "but absent from the data file.\n",
                         sep = "")
      }
      else {
        # perform length check and generate named list for current variable
        res <- addVariable(lm,
                           data[,currId],
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
