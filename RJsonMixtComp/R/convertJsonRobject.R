# fonction pour mettre la sortie Json au même format que RMixtComp
convertJsonRobject <- function(out, confidenceLevel = 0.95, mode = c("learn", "predict"))
{
  mode = match.arg(mode)
  
  out$mixture$IDClass = convertIDClass(out$mixture$IDClass)
  
  type <- out$variable$type 
  nbClass <- out$mixture$nbCluster
  
  for(nomVar in names(type))
  {
    if(mode == "learn")
    {
      out$variable$param[[nomVar]] = convertParamLearn(out$variable$param[[nomVar]], type[[nomVar]])
    }else{
      out$variable$param[[nomVar]] = convertParamPredict(out$variable$param[[nomVar]], type[[nomVar]])
    }

    out$variable$data[[nomVar]]$stat = convertDataStat(out$variable$data[[nomVar]]$stat, type[[nomVar]], confidenceLevel)
  }
  
  
  return(out)
}



# La sortie IDClass de JsonMixtComp est transposée et séparée en 2 élément
convertIDClass <- function(IDClass)
{
  newIDClass <- t(IDClass$idc)
  colnames(newIDClass) = IDClass$colName
  
  return(newIDClass)
}



# dataStat outMixtComp$variable$data$nomVar$stat
convertDataStat <- function(dataStat, type, confidenceLevel)
{
  if(!is.null(dataStat))
  {
    if(type != "Categorical_pjk")
    {
      colnames(dataStat) = switch(type,
                                  "LatentClass" = paste("k:", 1:ncol(dataStat)),
                                  c("indice", "median", paste0("q ",(1-confidenceLevel)/2*100,"%"), paste0("q ",(1-(1-confidenceLevel)/2)*100,"%")))
    }
  }
  
  return(dataStat)
}




# param outMixtComp$variable$param$nomVar
convertParamLearn <- function(param, type)
{
  nomObj <- switch(type, 
                   "Ordinal" = "muPi",
                   "LatentClass" = "pi",
                   "NumericalParam")
  
  param[[nomObj]]$log = do.call(rbind, param[[nomObj]]$log)
  colnames(param[[nomObj]]$log) = rep("", ncol(param[[nomObj]]$log))
  
  param[[nomObj]]$stat = do.call(cbind, param[[nomObj]]$stat)
  rownames(param[[nomObj]]$stat) = rownames(param[[nomObj]]$log)
  
  return(param)
}


# param outMixtComp$variable$param$nomVar
convertParamPredict <- function(param, type)
{
  nomObj <- switch(type, 
                   "Ordinal" = "muPi",
                   "LatentClass" = "pi",
                   "NumericalParam")

  param[[nomObj]]$stat = matrix(param[[nomObj]]$stat$value, ncol = 1)
  colnames(param[[nomObj]]$stat) = "value"
  
  
  nomRow <- switch(type,
                   "Ordinal" = unlist(lapply(paste0("k: ", 1:(nrow(param[[nomObj]]$stat)/2)), function(x){paste0(x, c(", mu:", ", pi"))})),
                   "Gaussian_sjk" = unlist(lapply(paste0("k: ", 1:(nrow(param[[nomObj]]$stat)/2)), function(x){paste0(x, c(", mean:", ", sd"))})),
                   "Categorical_pjk" = nomRowParamCateg(param),
                   "Poisson_k" = paste0("k: ", 1:nrow(param[[nomObj]]$stat), ", lambda"),
                   "LatentClass" = paste0("k: ", 1:nrow(param[[nomObj]]$stat)))
  
  rownames(param[[nomObj]]$stat) = nomRow
  
  return(param)
}

# param outMixtComp$variable$param$nomVar
nomRowParamCateg <- function(param)
{
  nModality <- as.numeric(gsub("nModality: ", "", param$NumericalParam$paramStr))

  return(unlist(lapply(paste0("k: ", 1:(nrow(param$NumericalParam$stat)/nModality)), function(x){paste0(x, paste0(", modality: ", c(1:nModality)))})))
}
