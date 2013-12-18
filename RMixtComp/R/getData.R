getData <- function(...){
  argList <- list(...)
  lm <- list()
  
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
                       stringsAsFactors = FALSE)
    descriptors <- read.table(file = descriptorFile,
                              header = FALSE,
                              sep = ";",
                              quote = "\"",
                              dec = ".",
                              fill = FALSE,
                              comment.char = "",
                              stringsAsFactors = FALSE)
    for (i in 1:length(data))
    {
      lm <- addModel(lm, data[[i]], descriptors[1, i]) # data is sent as a dataframe
    }
  }

  return(lm)
}