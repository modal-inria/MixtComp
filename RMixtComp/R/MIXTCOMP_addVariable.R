addVariable <- function (lm,
                         data,
                         id,
                         model,
                         paramStr) {
  warnLog = "" # warnLog will contain the various possible error messages
  if (length(lm) == 0) { # first variable is automaticaly added
    lm[[1]] <- list(data = data,
                    model = model,
                    id = id,
                    paramStr = paramStr)
  }
  else if (length(lm[[1]]$data) != length(data)) { # all variable must have the same number of individuals
    warnLog <- paste(warnLog,
                     "Variable ", id, " has ", length(data), " individuals while other variables currently have: ",
                     length(lm[[1]]$data), " individuals. All variables must have the same number of individuals.\n",
                     sep = "")
  }
  else { # variable is fit to be added !
    lm[[length(lm)+1]] <- list(data = data,
                               model = model,
                               id = id,
                               paramStr = paramStr)
  }

  return(list(lm = lm,
              warnLog = warnLog))
}