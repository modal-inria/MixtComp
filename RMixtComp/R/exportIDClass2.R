Compute.proba.vbles.class <- function(results){
  d <- length(results$variable$type) - 1
  G <- results$mixture$nbCluster
  proportions <- results$variable$param$z_class$pi$stat[,1]  
  proba.vbles <- lapply(1:d, function(j){
    name <- names(results$variable$type)[j+1]
    type <- results$variable$type[j+1]
    eval(parse(text = paste("tmpdata <- results$variable$data$", name, "$completed", sep="")))
    if (type=="Categorical_pjk"){
      eval(parse(text = paste("alpha <- matrix(results$variable$param$", name, "$NumericalParam$stat[,1], nrow=G, byrow=TRUE)", sep="")))
      out <- sapply(1:G, function(k) log(proportions[k]) + log(alpha[k,tmpdata]))
      rm(alpha)
    }else if (type=="Gaussian_sjk"){
      eval(parse(text = paste("param <- matrix(results$variable$param$", name, "$NumericalParam$stat[,1], nrow=G, ncol=2, byrow=TRUE)", sep="")))
      out <- sapply(1:G, function(k) log(proportions[k]) + dnorm(tmpdata, param[k,1], param[k,2], log=TRUE))
      rm(param)
    }else if (type=="Ordinal"){
      eval(parse(text = paste("param <- matrix(results$variable$param$", name, "$muPi$stat[,1], nrow=G, ncol=2, byrow=TRUE)", sep="")))
      out <- sapply(1:G, function(k) log(proportions[k])* rep(1, length(tmpdata)))
      rm(param)
    }else if (type=="Poisson_k"){
      eval(parse(text = paste("param <- as.numeric(results$variable$param$", name, "$NumericalParam$stat[,1])", sep="")))
      out <- sapply(1:G, function(k) log(proportions[k]) + dpois(tmpdata, param[k], log=TRUE))
      rm(param)
    }else if (type=="Rank"){
      out <- sapply(1:G, function(k) log(proportions[k])* rep(1, length(tmpdata)))
    }
    out <- exp(sweep(out, 1,  apply(out, 1, max), "-"))
    return(out / rowSums(out))
  })
  return(proba.vbles)
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
  
  proba.post <- Compute.proba.vbles.class(output)
  tik <- output$variable$data$z_class$stat
  zhat <- output$variable$data$z_class$completed
  
  pvdiscrim.vbles <- data.frame(names=names.vbles, 
                                discrim=round(1 - sapply(1:length(proba.post), function(k) -sum(log(proba.post[[k]]**proba.post[[k]]))) / (n * log(G)), 4),
                                shortnames= namesshort,
                                type=paste(names.vbles, '<br>', as.character(output$variable$type)[-1], sep=""))
  ordervbles <- order(pvdiscrim.vbles$discrim, decreasing = T)
  pvdiscrim.vbles <- pvdiscrim.vbles[order(pvdiscrim.vbles$discrim, decreasing = T),]
  pvdiscrim.vbles$colors <- col_numeric("Blues", domain = c(0,100))(ceiling(pvdiscrim.vbles$discrim*100))
  pvdiscrim.vbles <- as.list(pvdiscrim.vbles)
  pvdiscrim.vbles$titre <- "Discriminative level of the variables"
  ## visualisation des classes
  pvdiscrim.classes <- data.frame(names=paste("class", 1:G, sep="."), discrim=round(1 - (-colSums(log(tik**tik)) / (nrow(tik) * exp(-1))), 4))
  orderclass <- order(pvdiscrim.classes$discrim, decreasing = T)
  pvdiscrim.classes <- pvdiscrim.classes[orderclass,]
  pvdiscrim.classes$colors <- col_numeric("Blues", domain = c(0,100))(ceiling(pvdiscrim.classes$discrim*100))
  pvdiscrim.classes <- as.list(pvdiscrim.classes)
  pvdiscrim.classes$titre <- "Discriminative level of the classes"
  ### Stage 2 : carte 2D
  ## visualisation des variables
  delta <- list(x=pvdiscrim.vbles$shortnames,
                y=pvdiscrim.vbles$shortnames,
                z=round(1 - sqrt(sapply(ordervbles, function(j1) sapply(ordervbles, function(j2) mean(rowSums((proba.post[[j1]] - proba.post[[j2]])**2))))),4),
                titre="Clustering-based similarities between variables")
  delta$textmat <- delta$z
  for (j in 1:d)  delta$textmat[,j] <- paste("Similarity beween<br>", delta$y[j], " and ", delta$x, ": <br>", delta$z[,j], sep="")
  delta$colors <- (col_numeric("Blues", domain = c(0,100))(range(delta$z*100)))
  ## visualisation des classes
  sigma <- list(x=paste("class", 1:G, sep="."), 
                y=paste("class", 1:G, sep="."), 
                z=1-sqrt(sapply(orderclass, function(k) colMeans(sweep(tik[,orderclass], 1, tik[,k], "-")**2))),
                titre="Clustering-based similarities between classes")
  sigma$textmat <- sigma$z
  for (k in 1:G)  sigma$textmat[,k] <- paste("Similarity beween<br>", sigma$y[k], " and ", sigma$x, ": <br>", sigma$z[,k], sep="")
  sigma$colors <- (col_numeric("Blues", domain = c(0,100))(range(sigma$z*100)))
  ### Stage 3 : visusalisation des probabilités de classement
  ## histograme des tik
  ## heatmap des tik
  tiksorted <- NULL
  for (k in 1:G) tiksorted <- rbind(tiksorted, ExtractedTik(k, tik, zhat))
  return(list(pvdiscrim.vbles=pvdiscrim.vbles,
              pvdiscrim.classes=pvdiscrim.classes,
              delta=delta,
              sigma=sigma))
}

exportIDClass2 = function(data) {
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
  
  sigma = paste0('"sigma": ', toJSON(datavisu$sigma), '')
  

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
              footerStr,
              sep = '\n\n')
  return(out);
}
