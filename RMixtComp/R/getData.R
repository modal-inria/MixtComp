getData <- function(...){
  argList <- list(...)
  lm <- list()
  warnLog = ""
  
  for (k in 1:length(argList))
  {
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
    if (length(data) > maxVariable)
    {
      warnLog <- paste(warnLog,
                       "There are ", length(data), " variables. However the maximum number of variables for this demonstration is ",
                       maxVariable, " variables.\n",
                       "Please try again with less variables.\n",
                       sep = "")
    }
    for (i in 1:length(descriptor)) # loop over columns of the descriptor
    {
      currId <- descriptor[1, i]
      currModel <- descriptor[2, i]
      if (! descriptor[1, i] %in% names(data)) # descriptor does not match data
      {
        warnLog <- paste(warnLog,
                         "Variable ", currId, " asked for in descriptor file",
                         " but absent from the data file.\n",
                         sep = "")
      }
      else
      {
        # perform length check and generate named list for current variable
        res <- addVariable(lm,
                           data[[currId]],
                           currId, # id
                           currModel) # model
        lm <- res$lm
        warnLog <- paste(warnLog,
                         res$warnLog,
                         sep = "")
      }
    }
  }

  return(list(lm = lm,
              warnLog = warnLog))
}