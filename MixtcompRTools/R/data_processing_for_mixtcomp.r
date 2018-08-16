#' Helper function for Mixtcomp Input formatting
#'
#' @param lm        list as container
#' @param data      Vector of data
#' @param id        Name of the variable
#' @param model     Name of the model : "Gaussian_pjk", ..
#' @param paramStr  Additional parameters as strings
#'
#' @return
#'
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
#' @import utils
getData <- function(...) {
  argList <- list(...)
  lm <- list()
  warnLog = ""

  for (k in 1:length(argList)) {
    dataFile <- argList[[k]][1]
    descriptorFile <- argList[[k]][2]
    data <- read.table(
      file = dataFile,
      header = TRUE,
      sep = ";",
      quote = "\"",
      dec = ".",
      fill = FALSE,
      comment.char = "",
      stringsAsFactors = FALSE,
      na.strings = ""
    )
    descriptor <- read.table(
      file = descriptorFile,
      header = FALSE,
      sep = ";",
      quote = "\"",
      dec = ".",
      fill = FALSE,
      comment.char = "",
      stringsAsFactors = FALSE
    )
    for (i in 1:length(descriptor)) {
      # loop over columns of the descriptor
      currId <- descriptor[1, i]
      currModel <- descriptor[2, i]
      if (dim(descriptor)[1] == 3) {
        param <-
          descriptor[3, i] # a character string describing the parameter space, that will be parsed directly by the model
      }
      else {
        param <-
          "" # a zero-sized parameter string will be interpreted by the model as "deduce the parameter space automatically"
      }
      if (!descriptor[1, i] %in% names(data)) {
        # descriptor does not match data
        warnLog <- paste(
          warnLog,
          "Variable ",
          currId,
          " asked for in descriptor file ",
          "but absent from the data file.\n",
          sep = ""
        )
      }
      else {
        # perform length check and generate named list for current variable
        res <- addVariable(lm,
                           data[[currId]],
                           currId, # id
                           currModel, # model
                           param)
        lm <-
          res$lm # this discards the warnLog output by addVariable
        warnLog <- paste(warnLog,
                         res$warnLog,
                         sep = "")
      }
    }
  }

  return(list(lm = lm,
              warnLog = warnLog))
}
