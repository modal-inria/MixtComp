# fonction pour mettre la sortie Json au même format que RMixtComp
convertJsonRobject <- function(out, confidenceLevel = 0.95)
{
  
  out$mixture$IDClass = convertIDClass(out$mixture$IDClass)
  
  type <- out$variable$type 
  nbClass <- out$mixture$nbCluster
  
  for(nomVar in names(type))
  {
    out$variable$param[[nomVar]] = convertParam(out$variable$param[[nomVar]], type[[nomVar]])
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
convertParam <- function(param, type)
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

