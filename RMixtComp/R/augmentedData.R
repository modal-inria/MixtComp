augmentedData <- function(stringData) # stringData is a vector of strings
{
  nbrStr <- "[0-9.]+"

  # [] and # :
  listIntervals <- list()
  
  # []
  rangeList <- grep("\\[.*\\]", stringData)
  if (length(rangeList) > 0)
  {
    for (i in 1:length(rangeList))
    {
      nbrList <- str_match_all(stringData[rangeList[i]], nbrStr)
      nbrList <- as.numeric(nbrList[[1]])
      listIntervals[[length(listIntervals) + 1]] <- list(rangeList[i], nbrList)
      stringData[rangeList[i]] <- "NA"
    }
  }
  
  # :
  rangeList <- grep(".*:.*", stringData)
  if (length(rangeList) > 0)
  {
    for (i in 1:length(rangeList))
    {
      nbrList <- str_match_all(stringData[rangeList[i]], nbrStr)
      nbrList <- as.numeric(nbrList[[1]])
      listIntervals[[length(listIntervals) + 1]] <- list(rangeList[i], nbrList)
      stringData[rangeList[i]] <- "NA"
    }
  }
  
  # {}
  listFiniteValues <- list()
  rangeList <- grep("\\{.*\\}", stringData)
  if (length(rangeList) > 0)
  {
    for (i in 1:length(rangeList))
    {
      nbrList <- str_match_all(stringData[rangeList[i]], nbrStr)
      nbrList <- as.numeric(nbrList[[1]])
      listFiniteValues[[length(listFiniteValues) + 1]] <- list(rangeList[i], nbrList)
      stringData[rangeList[i]] <- "NA"
    }
  }
  
  # ?
  listMissing <- list()
  rangeList <- grep("\\?", stringData)
  if (length(rangeList) > 0)
  {
    for (i in 1:length(rangeList))
    {
      listMissing[[length(listMissing) + 1]] <- rangeList[i]
      stringData[rangeList[i]] <- "NA"
    }
  }
  
  currList <- list(data             = as.numeric(stringData) ,
                   listIntervals    = listIntervals          ,
                   listFiniteValues = listFiniteValues       ,
                   listMissing      = listMissing            )
  
  return(currList)
}