addVariable <- function (lm,
                         data,
                         id,
                         model)
{
  warnLog = "" # warnLog will contain the various possible error messages
  if (length(lm) == 0) # first variable is automaticaly added
  {
    lm[[1]] <- list(data = data,
                    model = model,
                    id = id)
  }
  else if (length(lm[[1]]$data) != length(data)) # all variable must have the same number of individuals
  {
    warnLog <- paste(warnLog,
                     "Variable ", id, " has ", length(data), " individuals while other variables currently have: ",
                     length(lm[[1]]$data), " individuals. All variables must have the same number of individuals.\n",
                     sep = "")
  }
  else if (length(data) > maxIndividual) # check if the variable has a minimal length
  {
    warnLog <- paste(warnLog,
                     "Variable ", id, " has ", length(data), " individuals while other variables currently have ",
                     length(lm[[1]]$data), " individuals. All variables must have the same number of individuals.\n",
                     sep = "")
  }
  else # variable is fit to be added !
  {
    lm[[length(lm)+1]] <- list(data = data,
                               model = model,
                               id = id)
  }

  return(list(lm = lm,
              warnLog = warnLog))
}