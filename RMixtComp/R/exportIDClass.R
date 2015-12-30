writeIDClassHTML = function(data,
                            fileNameHTML) {
  cat(headerStr,
      varStr,
      exportIDClass(data),
      footerStr,
      file = fileNameHTML,
      sep = "\n",
      fill = FALSE,
      labels = NULL,
      append = FALSE)
}

writeIDClassJSON = function(data,
                            fileNameJSON) {
  cat(exportIDClass(data),
      file = fileNameJSON,
      sep = "\n",
      fill = FALSE,
      labels = NULL,
      append = FALSE)
}

varStr = 'var IDData ='

pStr = function(x){
  return(format(round(x, 1), nsmall = 1))
}

exportIDClass = function(data) {
  headerStr = paste('{')
  
  classStr = paste0('"nbClass": ', extractNbClass(data), ',')
  
  piStr = paste0('"pi": [', extractPi(data), '],')
  
  nbVarStr = paste0('"nbVar": ', extractNBVar(data), ',')
  
  varNameStr = paste0('"varName": [', extractVarName(data), '],')
  
  varTypeStr = paste0('"varType": [', extractVarType(data), '],')
  
  eStr = paste0('"e": [', extractID(data), '],')
  
  pStr = paste0('"p": [', extractParam(data), ']')
  
  footerStr = '}'
  
  out = paste(headerStr,
              classStr,
              piStr,
              nbVarStr,
              varNameStr,
              varTypeStr,
              eStr,
              pStr,
              footerStr,
              sep = '\n\n')
  return(out);
}

extractNbClass = function(data){
  return(data$mixture$nbCluster)
}

extractPi = function(data){
  out = paste0(data$variable$param$z_class$pi$stat[1,1])
  for (k in 2:data$mixture$nbCluster) {
    out = paste0(out, ', ', data$variable$param$z_class$pi$stat[k,1])
  }
  return(out)
}

extractNBVar = function(data){
  return(length(data$variable$param) - 1)
}

extractVarName = function(data){
  nameList = names(data$variable$type)
  out = paste0('"', nameList[2], '"')
  for (j in 3:length(nameList)) {
    out = paste0(out, ', "', nameList[j],'"')
  }
  return(out)
}

extractVarType = function(data){
  typeList = data$variable$type
  out = paste0('"', typeList[2], '"')
  for (j in 3:length(typeList)) {
    out = paste0(out, ', "', typeList[j],'"')
  }
  return(out)
}

extractID = function(data){
  IDClass = data$mixture$IDClass
  nbClass = nrow(IDClass)
  vecRow = vector(mode = 'character', length = nbClass)
  for (k in 1:nbClass){
    rowStr = paste(pStr(IDClass[k,]), collapse = ', ')
    vecRow[k] = paste0('[', rowStr, ']')
  }
  out = paste(vecRow, collapse = ',\n')
  return(out)
}

extractParam = function(data){
  param = paramTable(data)
  nbClass = nrow(param)
  vecRow = vector(mode = 'character', length = nbClass)
  for (k in 1:nbClass){
    rowStr = paste(param[k,], collapse = ', ')
    vecRow[k] = paste0('[', rowStr, ']')
  }
  out = paste(vecRow, collapse = ',\n')
  return(out)
}

paramTable = function(data){
  nbClass = data$mixture$nbCluster
  param = data$variable$param
  typeList = data$variable$type
  nbVar = length(param)
  varName = names(data$variable$type)
  
  out = matrix(data = "",
               nrow = nbClass,
               ncol = nbVar - 1)
  
  j = 1
  for (jP in 1:nbVar) { # the first variable corresponds to the LatentClass, which is ignored in the IDClass table
    currVar = varName[[jP]]
    
    if (typeList[[currVar]] == 'LatentClass') {
      next
    }
    else if (typeList[currVar] == 'Categorical_pjk') {
      out[,j] = paramCategorical(nbClass, param[[currVar]])
    }
    else if (typeList[currVar] == 'Gaussian_sjk') {
      out[,j] = paramGaussian(nbClass, param[[currVar]])
    }
    else if (typeList[currVar] == 'Poisson_k') {
      out[,j] = paramPoisson(nbClass, param[[currVar]])
    }
    else if (typeList[currVar] == 'Ordinal') {
      out[,j] = paramOrdinal(nbClass, param[[currVar]])
    }
    else if (typeList[currVar] == 'Rank') {
      out[,j] = paramRank(nbClass, param[[currVar]])
    }
    else {
      out[,j] = paramUnknown(nbClass, param[[currVar]])
    }
    j = j + 1
  }
  
  return(out)
}

paramCategorical = function(nbClass, var){
  val = var$NumericalParam$stat[,1]
  out = vector(mode = 'character', length = nbClass)
  nbModality = length(val) / nbClass
  for (k in 1:nbClass){
    firstInd = ((k - 1) * nbModality + 1)
    lastInd =    k      * nbModality
    propStr = paste(val[firstInd:lastInd], collapse = ', ')
    out[k] = paste0('{"alpha" : "[', propStr, ']"}')
  }
  return(out)
}

paramGaussian = function(nbClass, var){
  val = var$NumericalParam$stat[,1]
  out = vector(mode = 'character', length = nbClass)
  for (k in 1:nbClass){
    firstInd = (k - 1) * 2 + 1
    out[k] = paste0('{"mu" : "', val[firstInd], '", "sigma" : "', val[firstInd + 1], '"}')
  }
  return(out)
}

paramPoisson = function(nbClass, var){
  val = var$NumericalParam$stat[,1]
  out = vector(mode = 'character', length = nbClass)
  for (k in 1:nbClass){
    out[k] = paste0('{"lambda" : "', val[k], '"}')
  }
  return(out)
}

paramOrdinal = function(nbClass, var){
  val = var$muPi$stat[,1]
  
  out = vector(mode = 'character', length = nbClass)
  for (k in 1:nbClass) {
    firstInd = (k - 1) * 2 + 1
    out[k] = paste0('{"mu" : "', as.integer(val[firstInd]), '", "pi" : "', val[firstInd + 1], '"}')
  }
  return(out)
}

paramRank = function(nbClass, var){
  valMu = var$mu$stat
  valPi = var$pi$stat[,1]
  
  out = vector(mode = 'character', length = nbClass)
  for (k in 1:nbClass) {
    collapsedMu = paste0(valMu[[k]][[1]][[1]], collapse = " ")
    out[k] = paste0('{"mu" : "', collapsedMu,
                    '", "pi" : "', valPi[k], '"}')
  }
  return(out)
}

paramUnknown <- function(nbClass, var){
  out = vector(mode = 'character', length = nbClass)
  for (k in 1:nbClass){
    out[k] = '"Parameter output not implemented yet"'
  }
  return(out)
}