setClass(
  Class="AugmentedData",
  representation=representation(
    data = "matrix",
    listValues = "list",
    listIntervals = "list",
    listRanges = "list"
  ),
  prototype = prototype(
    data = matrix(0),
    listValues = list(),
    listIntervals = list(),
    listRanges = list()
  )
)

setMethod(
  f = "initialize",
  signature = c("AugmentedData"),
  definition = function(.Object, stringData){  
    # []
    .Object@listRanges <- list()
    rangeList <- grep("\\[.*\\]", myData)
    for (i in 1:length(rangeList) )
    {
      m <- gregexpr("[0-9.]+", myData[rangeList[i]])
      # couple containing the index and the values is added to lv
      .Object@listRanges[[length(.Object@listRanges) + 1]] <- c(rangeList[i], regmatches(myData[rangeList[i]], m))
      myData[rangeList[i]] <- NA
    }
    
    # {}
    .Object@listIntervals <- list()
    rangeList <- grep("\\{.*\\}", myData)
    for (i in 1:length(rangeList) )
    {
      m <- gregexpr("[0-9.]+", myData[rangeList[i]])
      # couple containing the index and the values is added to lv
      .Object@listValues[[length(.Object@listValues) + 1]] <- c(rangeList[i], regmatches(myData[rangeList[i]], m))
      myData[rangeList[i]] <- NA
    }

    # ?
    lm <- list()
    rangeList <- grep("\\?", myData)
    for (i in 1:length(rangeList) )
    {
      # couple containing the index and the values is added to lv
      lm[[length(lm) + 1]] <- rangeList[i]
      myData[rangeList[i]] <- NA
    }
    
    .Object@data = as.numeric(myData)
    
    return(.Object)
  }
)

