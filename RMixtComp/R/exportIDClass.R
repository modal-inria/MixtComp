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
  
  pStr = paste0('"p": [', extractParam(data), '],')
  
  pvdiscrimclasses = paste0('"pvdiscrimclasses": ', extractPvDiscrimClass(data), ',')
  
  pvdiscrimvbles = paste0('"pvdiscrimvbles": ', extractPvDiscrimVbles(data), ',')
  
  delta = paste0('"delta": ', extractDistDelta(data), ',')
  
  sigma = paste0('"sigma": ', extractDistSigma(data), ',')
  
  ordertik = paste0('"tiksorted": ', extractOrderTik(data), ',')
  
  cibounds = paste0('"cibounds": ', extractCIbounds(data), '')
  
  footerStr = '}'
  
  out = paste(headerStr,
              classStr,
              piStr,
              nbVarStr,
              varNameStr,
              varTypeStr,
              pStr,
              pvdiscrimclasses,
              pvdiscrimvbles,
              delta,
              sigma,
              ordertik,
              cibounds,
              footerStr,
              sep = '\n\n')
  return(out);
}


# Extract the confidence interval and the mean for each variable
# function extractCIboundsOneVbles is defined in file extractCIbounds.R
extractCIbounds = function(data){
  return(toJSON(lapply(names(output$variable$type)[-1],
                       function(v) extractCIboundsOneVble(v, data))))
}

# Sort the tik of observations belonging to class k with decreasing order
extractOrderTik = function(data){
  return(
    toJSON(
      unlist(
        sapply(1:data$mixture$nbCluster, 
               function(k) order(data$variable$data$z_class$stat[,k] * (data$variable$data$z_class$completed == k ),
                                 decreasing = T)[1:(table(data$variable$data$z_class$completed)[k])]
        ))))
}

extractDistSigma = function(data){
  return(toJSON(round(1 - sqrt(sapply(1:data$mixture$nbCluster,
                                  function(k) colMeans(sweep(data$variable$data$z_class$stat,
                                                             1,
                                                             data$variable$data$z_class$stat[,k],
                                                             "-")**2))
  ), 4)))
}

extractDistDelta = function(data){
  return(toJSON(round(1 - data$mixture$delta, 4)))
}

extractPvDiscrimClass = function(data){
  tik <- data$variable$data$z_class$stat
  return(toJSON(round(1 - (-colMeans(log(tik**tik)) / exp(-1)), 4)))
}


extractPvDiscrimVbles = function(data){
  return(toJSON(unname(round(1 - colSums(data$mixture$IDClass), 4))))
}

extractNbClass = function(data){
  return(data$mixture$nbCluster)
}

extractPi = function(data){
  out = paste0(data$variable$param$z_class$pi$stat[1,1])  # output of the first class
  if (data$mixture$nbCluster > 1) { # output only if there are more than one class
    for (k in 2:data$mixture$nbCluster) {
      out = paste0(out, ', ', data$variable$param$z_class$pi$stat[k,1])
    }
  }
  return(out)
}

extractNBVar = function(data){
  return(length(data$variable$param) - 1)
}

extractVarName = function(data){
  nameList = names(data$variable$type)
  out = paste0('"', nameList[2], '"')
  if (length(nameList) > 2) {
    for (j in 3:length(nameList)) {
      out = paste0(out, ', "', nameList[j],'"')
    }
  }
  return(out)
}

extractVarType = function(data){
  typeList = data$variable$type
  out = paste0('"', typeList[2], '"')
  if (length(typeList) > 2) {
    for (j in 3:length(typeList)) {
      out = paste0(out, ', "', typeList[j],'"')
    }
  }
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
  nbVar = length(data$variable$type)
  varName = names(data$variable$type)
  
  out = matrix(data = "",
               nrow = nbClass,
               ncol = nbVar - 1)
  
  j = 1
  for (jP in 1:nbVar) {
    currVar = varName[[jP]]
    
    if (typeList[[currVar]] == 'LatentClass') {
      next # the first variable corresponds to the LatentClass, which is ignored in the IDClass table
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
    else if (typeList[currVar] == 'Functional') {
      out[,j] = paramFunctional(nbClass, param[[currVar]])
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

paramFunctional = function(nbClass, var){
  valAlpha = var$alpha$stat[,1]
  valBeta = var$beta$stat[,1]
  valSd = var$sd$stat[,1]
  
  out = vector(mode = 'character', length = nbClass)
  for (k in 1:nbClass) {
    out[k] = paste0('{"alpha" : "', valAlpha[k],
                    '", "beta" : "', valBeta[k], 
                    '", "sigma" :"', valSd[k], '"}')
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