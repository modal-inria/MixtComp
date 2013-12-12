setClass(
  Class="AugmentedData",
  representation=representation(
    data = "matrix",
    listMissing = "list",
    listFiniteValues = "list",
    listIntervals = "list"
  ),
  prototype = prototype(
    data = matrix(0),
    listMissing = list(),
    listFiniteValues = list(),
    listIntervals = list()
  )
)

setMethod(
  f = "initialize",
  signature = c("AugmentedData"),
  definition = function(.Object, stringData){
    nbrStr <- "[0-9.]+"
    nRows <- nrow(stringData)
    # [] and # :
    .Object@listIntervals <- list()
    
    # []
    rangeList <- grep("\\[.*\\]", myData)
    for (i in 1:length(rangeList) )
    {
      nbrList <- str_match_all(myData[rangeList[i]], nbrStr)
      nbrList <- as.numeric(nbrList[[1]])
      .Object@listIntervals[[length(.Object@listIntervals) + 1]] <- list(c((rangeList[i] - 1) %%  nRows,
                                                                           (rangeList[i] - 1) %/% nRows),
                                                                         nbrList)
      myData[rangeList[i]] <- NA
    }
    
    # :
    rangeList <- grep(".*:.*", myData)
    for (i in 1:length(rangeList) )
    {
      nbrList <- str_match_all(myData[rangeList[i]], nbrStr)
      nbrList <- as.numeric(nbrList[[1]])
      .Object@listIntervals[[length(.Object@listIntervals) + 1]] <- list(c((rangeList[i] - 1) %%  nRows,
                                                                           (rangeList[i] - 1) %/% nRows),
                                                                         nbrList)
      myData[rangeList[i]] <- NA
    }
    
    # {}
    .Object@listFiniteValues <- list()
    rangeList <- grep("\\{.*\\}", myData)
    for (i in 1:length(rangeList) )
    {
      nbrList <- str_match_all(myData[rangeList[i]], nbrStr)
      nbrList <- as.numeric(nbrList[[1]])
      .Object@listFiniteValues[[length(.Object@listFiniteValues) + 1]] <- list(c((rangeList[i] - 1) %%  nRows,
                                                                                 (rangeList[i] - 1) %/% nRows),
                                                                               nbrList)
      myData[rangeList[i]] <- NA
    }

    # ?
    .Object@listMissing <- list()
    rangeList <- grep("\\?", myData)
    for (i in 1:length(rangeList) )
    {
      .Object@listMissing[[length(.Object@listMissing) + 1]] <- c((rangeList[i] - 1) %%  nRows,
                                                                  (rangeList[i] - 1) %/% nRows)
      myData[rangeList[i]] <- NA
    }
    
    show(myData)
    .Object@data = matrix(as.numeric(myData), nrow = nrow(myData))
    
    return(.Object)
  }
)

