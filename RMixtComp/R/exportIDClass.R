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
  cat(exportIDClass2(data),
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

# Ancienne version (à supprimer si Vincent ok)
# exportIDClass = function(data) {
#   headerStr = paste('{')
#   
#   classStr = paste0('"nbClass": ', extractNbClass(data), ',')
#   
#   piStr = paste0('"pi": [', extractPi(data), '],')
#   
#   nbVarStr = paste0('"nbVar": ', extractNBVar(data), ',')
#   
#   varNameStr = paste0('"varName": [', extractVarName(data), '],')
#   
#   varTypeStr = paste0('"varType": [', extractVarType(data), '],')
#   
#   eStr = paste0('"e": [', extractID(data), '],')
#   
#   pStr = paste0('"p": [', extractParam(data), ']')
#   
#   footerStr = '}'
#   
#   out = paste(headerStr,
#               classStr,
#               piStr,
#               nbVarStr,
#               varNameStr,
#               varTypeStr,
#               eStr,
#               pStr,
#               footerStr,
#               sep = '\n\n')
#   return(out);
# }



exportIDClass = function(data) {
  headerStr = paste('{')
  
  classStr = paste0('"nbClass": ', extractNbClass(data), ',')
  
  piStr = paste0('"pi": [', extractPi(data), '],')
  
  nbVarStr = paste0('"nbVar": ', extractNBVar(data), ',')
  
  varNameStr = paste0('"varName": [', extractVarName(data), '],')
  
  varTypeStr = paste0('"varType": [', extractVarType(data), '],')
  
  eStr = paste0('"e": [', extractID(data), '],')
  
  pStr = paste0('"p": [', extractParam(data), '],')
  
  datavisu <- VisuTools(data)
  
  pvdiscrimclasses = paste0('"pvdiscrimclasses": ', toJSON(datavisu$pvdiscrim.classes), ',')
  
  pvdiscrimvbles = paste0('"pvdiscrimvbles": ', toJSON(datavisu$pvdiscrim.vbles), ',')
  
  delta = paste0('"delta": ', toJSON(datavisu$delta), ',')
  
  sigma = paste0('"sigma": ', toJSON(datavisu$sigma), ',')
  
  tiksorted = paste0('"tiksorted": ', toJSON(datavisu$tiksorted), '')
  
  
  footerStr = '}'
  
  out = paste(headerStr,
              classStr,
              piStr,
              nbVarStr,
              varNameStr,
              varTypeStr,
              eStr,
              pStr,
              pvdiscrimclasses,
              pvdiscrimvbles,
              delta,
              sigma,
              tiksorted,
              footerStr,
              sep = '\n\n')
  return(out);
}

ExtractedTik <- function(k, tik, z){
  tmp <- tik[(z==k),]
  rownames(tmp) <- which(z==k)
  tmp <- tmp[order(tmp[,k], decreasing = T),]
  tmp
}

VisuTools <- function(output){
  n <- output$mixture$nbInd
  d <- length(output$variable$type) - 1
  names.vbles <- names(output$variable$type)[-1]
  namesshort <- abbreviate(names.vbles, 6)
  names(namesshort) <- NULL
  G <- output$mixture$nbCluster
  tik <- output$variable$data$z_class$stat
  zhat <- output$variable$data$z_class$completed
  # Pour le barplot des variables
  pvdiscrim.vbles <- data.frame(names=names.vbles, 
                                discrim=round(1 - colSums(output$mixture$IDClass), 4),
                                shortnames= namesshort,
                                type=paste(names.vbles, '<br>', as.character(output$variable$type)[-1], sep=""))
  ordervbles <- order(pvdiscrim.vbles$discrim, decreasing = T)
  pvdiscrim.vbles <- pvdiscrim.vbles[order(pvdiscrim.vbles$discrim, decreasing = T),]
  pvdiscrim.vbles$colors <- col_numeric("Blues", domain = c(0,100))(ceiling(pvdiscrim.vbles$discrim*100))
  pvdiscrim.vbles <- as.list(pvdiscrim.vbles)
  pvdiscrim.vbles$titre <- "Discriminative level of the variables"
  ## Pour le barplot des classes
  pvdiscrim.classes <- data.frame(names=paste("class", 1:G, sep="."), discrim=round(1 - (-colSums(log(tik**tik)) / (nrow(tik) * exp(-1))), 4))
  orderclass <- order(pvdiscrim.classes$discrim, decreasing = T)
  pvdiscrim.classes <- pvdiscrim.classes[orderclass,]
  pvdiscrim.classes$colors <- col_numeric("Blues", domain = c(0,100))(ceiling(pvdiscrim.classes$discrim*100))
  pvdiscrim.classes <- as.list(pvdiscrim.classes)
  pvdiscrim.classes$titre <- "Discriminative level of the classes"
  ## Pour la heatmap des distances entre variables
  output$mixture$delta <- output$mixture$delta[ordervbles,]
  output$mixture$delta <- output$mixture$delta[,ordervbles]
  delta <- list(x=pvdiscrim.vbles$shortnames,
                y=pvdiscrim.vbles$shortnames,
                z=round(1 - output$mixture$delta, 4),
                titre="Clustering-based similarities between variables")
  delta$textmat <- delta$z
  for (j in 1:d)  delta$textmat[,j] <- paste("Similarity beween<br>", delta$y[j], " and ", delta$x, ": <br>", delta$z[,j], sep="")
  delta$colors <- (col_numeric("Blues", domain = c(0,100))(range(delta$z*100)))
  ## Pour la heatmap des distances entre classes
  sigma <- list(x=paste("class", 1:G, sep="."), 
                y=paste("class", 1:G, sep="."), 
                z=1-sqrt(sapply(orderclass, function(k) colMeans(sweep(tik[,orderclass], 1, tik[,k], "-")**2))),
                titre="Clustering-based similarities between classes")
  sigma$textmat <- sigma$z
  for (k in 1:G)  sigma$textmat[,k] <- paste("Similarity beween<br>", sigma$y[k], " and ", sigma$x, ": <br>", sigma$z[,k], sep="")
  sigma$colors <- (col_numeric("Blues", domain = c(0,100))(range(sigma$z*100)))
  ### Pour la heatmap des tik
  tiksorted <- NULL
  for (k in 1:G) tiksorted <- rbind(tiksorted, ExtractedTik(k, tik, zhat))
  tiksorted <- list(x=paste("class", 1:G, sep="."),
                    y=rownames(tiksorted),
                    z=tiksorted,
                    titre="Probabilities of classification")
  tiksorted$colors <- (col_numeric("Blues", domain = c(0,100))(range(tiksorted$z*100)))
  tiksorted$textmat <- tiksorted$z
  for (j in 1:ncol(tiksorted$textmat)) tiksorted$textmat[,j] <- 
    paste("Probability that <br> observation", 
          rownames(tiksorted$z), 
          " <br>belongs to class",
          j, 
          ": <br>",
          round(tiksorted$z[,j],4))
  
  return(list(pvdiscrim.vbles=pvdiscrim.vbles,
              pvdiscrim.classes=pvdiscrim.classes,
              delta=delta,
              sigma=sigma,
              tiksorted=tiksorted))
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

extractID = function(data){
  IDClass = data$mixture$IDClass
  nbClass = nrow(IDClass)
  vecRow = vector(mode = 'character', length = nbClass)
  for (k in 1:nbClass){
    rowStr = paste(IDClass[k,], collapse = ', ')
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