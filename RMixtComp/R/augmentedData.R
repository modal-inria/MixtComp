augmentedData <- function(stringData) # stringData is a vector of strings
{
  nbrStr <- "-*[0-9.]+"
  
  # ?, [-inf:+inf], -inf:+inf
  listMissing <- list()
  a <- grep("\\?", stringData)
  b <- grep("-inf *: *\\+inf", stringData)
  rangeList <- c(a, b)
  if (length(rangeList) > 0)
  {
    for (i in 1:length(rangeList))
    {
      listMissing[[length(listMissing) + 1]] <- rangeList[i]
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
      listFiniteValues[[length(listFiniteValues) + 1]] <- list(pos=rangeList[i], listvals=nbrList)
      stringData[rangeList[i]] <- "NA"
    }
  }

  # [] and :
  listIntervals <- list()
  rangeList <- grep("-*[0-9.]+ *: *-*[0-9.]+", stringData)
  if (length(rangeList) > 0)
  {
    for (i in 1:length(rangeList))
    {
      nbrList <- str_match_all(stringData[rangeList[i]], nbrStr)
      nbrList <- as.numeric(nbrList[[1]])
      listIntervals[[length(listIntervals) + 1]] <- list(pos=rangeList[i], listvals=nbrList)
      stringData[rangeList[i]] <- "NA"
    }
  }

  # [-inf:] and -inf:
  listLUIntervals <- list()
  rangeList <- grep("-inf *: *-*[0-9.]+", stringData)
  if (length(rangeList) > 0)
  {
    for (i in 1:length(rangeList))
    {
      nbrList <- str_match_all(stringData[rangeList[i]], nbrStr)
      nbrList <- as.numeric(nbrList[[1]])
      listLUIntervals[[length(listLUIntervals) + 1]] <- list(pos=rangeList[i], listvals=nbrList)
      stringData[rangeList[i]] <- "NA"
    }
  }

  # [:+inf] and :+inf
  listRUIntervals <- list()
  rangeList <- grep("-*[0-9.]+ *: *\\+inf", stringData)
  if (length(rangeList) > 0)
  {
    for (i in 1:length(rangeList))
    {
      nbrList <- str_match_all(stringData[rangeList[i]], nbrStr)
      nbrList <- as.numeric(nbrList[[1]])
      listRUIntervals[[length(listRUIntervals) + 1]] <- list(pos=rangeList[i], listvals=nbrList)
      stringData[rangeList[i]] <- "NA"
    }
  }
  
  data = as.numeric(stringData)
  
  currList <- list(data             = data                    ,
                   dataRange        = c(min(data, na.rm=TRUE) ,
                                        max(data, na.rm=TRUE)),
                   listFiniteValues = listFiniteValues        ,
                   listMissing      = listMissing             ,
                   listIntervals    = listIntervals           ,
                   listRUIntervals  = listRUIntervals         ,
                   listLUIntervals  = listLUIntervals         )
  
  return(currList)
}