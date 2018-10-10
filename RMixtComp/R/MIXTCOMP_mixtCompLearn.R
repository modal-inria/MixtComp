
mixtCompLearn <- function(data, desc, nClass, algo, crit = c("BIC", "ICL"))
{
  crit = match.arg(crit)
  indCrit <- ifelse(crit == "BIC", 1, 2)
  
  dataList <- formatData(data)
  desc <- formatDesc(desc)  
  
  algo$nInd = length(dataList[[1]])
  algo$mode = "learn"
  
  resLearn <- list()
  for(i in seq_along(nClass))
  {
    algo$nClass = nClass[i]
    
    resLearn[[i]] <- rmc(algo, dataList, desc, list())
    class(resLearn[[i]]) = "MixtCompCluster"
  }
  
  allCrit <- sapply(resLearn, function(x) {c(getBIC(x), getICL(x))})
  colnames(allCrit) = c(nClass)
  rownames(allCrit) = c("BIC", "ICL")
  indBestClustering <- which.max(allCrit[indCrit, ])
  
  if(length(indBestClustering) != 0)
  {
    res <- c(resLearn[[indBestClustering]], list(criterion = crit, crit = allCrit, nClass = nClass, res = resLearn))
  }else{
    res <- list(warnLog = "Unable to select a model. Check $res[[i]]$warnLog for details", criterion = crit, crit = allCrit, nClass = nClass, res = resLearn)
  }
  class(res) = c("MixtCompLearn", "MixtCompCluster")
  
  return(res)
}


# format the descriptor list for rmc function:
# - add paramStr when missing
# - ensure the list format of each element
formatDesc <- function(desc)
{
  desc = lapply(desc, function(x){
    if(!is.list(x))
      x = list(type = x)
    if(!("paramStr" %in% names(x)))
      x$paramStr = ""
    
    return(x)
  })
  
  return(desc)
}


# format data.frame or matrix in list of character
# keep list in list format
formatData <- function(data)
{
  if(is.data.frame(data) || is.matrix(data))
  {
    out <- lapply(1:ncol(data), function(i) as.character(data[,i]))
    names(out) = colnames(data)
  }else{
    out <- data
  }

  return(out)
}


# check the number of class given by the user in mixtCompPredict
checkNClass <- function(nClass, resLearn)
{
  if(is.null(nClass))
    nClass = resLearn$algo$nClass
  
  if(length(nClass) > 1)
  {
    nClass = nClass[1]
    warning("Several nClass given. Only the first is used.")
  }
  
  if("MixtCompLearn" %in% class(resLearn))
  {
    if(!(nClass %in% resLearn$nClass))
    {
      nClass = resLearn$algo$nClass
      warning("The provided value of nClass does not match any results in the resLearn object. nClass is set to the value with the best criterion value.")
    }
  }else{
    if(nClass != resLearn$algo$nClass)
    {
      nClass = resLearn$algo$nClass
      warning("The provided value of nClass does not match the value in the resLearn object. nClass is set to the value of the resLearn object.")
    }
  }
  
  return(nClass)
}
