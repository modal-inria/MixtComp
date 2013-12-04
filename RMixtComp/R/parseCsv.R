parseCsv <- function(filename){
  myData <- as.matrix(read.csv(filename, sep=";", header=FALSE, stringsAsFactors=FALSE))
  
  rangeList <- grep("\\[.*\\]", myData)
  for (i in 1:length(rangeList) )
  {
    m <- gregexpr("[0-9.]+", myData[rangeList[i]])
    show(regmatches(myData[rangeList[i]], m))
    myData[rangeList[i]] <- NA
  }
  
  rangeList <- grep("\\{.*\\}", myData)
  for (i in 1:length(rangeList) )
  {
    m <- gregexpr("[0-9.]+", myData[rangeList[i]])
    show(regmatches(myData[rangeList[i]], m))
    myData[rangeList[i]] <- NA
  }
  
  return(as.integer(myData))
}
