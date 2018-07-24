#'
#' Transform JsonMixtComp output
#'
#' @param out output of JsonMixtComp
#' @param confidenceLevel quantile for confidence interval of estimated parameters.
#' @param mode "learn" or "predict"
#' 
#' @return list that looks like le output of RMixtCompPredict and RMixtCompCluster
#' 
#' @export
convertJsonRobject <- function(out, confidenceLevel = 0.95, mode = c("learn", "predict"))
{
  mode = match.arg(mode)
  
  out$mixture$IDClass = convertIDClass(out$mixture$IDClass)
  
  type <- out$variable$type 
  nbClass <- out$mixture$nbCluster
  
  for(nomVar in names(type))
  { 
    if(type[nomVar] %in% c("Functional", "FunctionalSharedAlpha"))
    {
      nCoeff <- as.numeric(strsplit(out$variable$param[[nomVar]]$alpha$paramStr,"nCoeff: ")[[1]][2])
      nSub <- as.numeric(strsplit(strsplit(out$variable$param[[nomVar]]$alpha$paramStr,", nCoeff: ")[[1]][1],"nSub: ")[[1]][2])
    }else{
      nCoeff = nSub <- 1
    }

    out$variable$param[[nomVar]] = convertParam(out$variable$param[[nomVar]], type[[nomVar]], nbClass, nCoeff, nSub, mode)

    out$variable$data[[nomVar]]$stat = convertDataStat(out$variable$data[[nomVar]]$stat, type[[nomVar]], confidenceLevel)
  }
  
  # réordonner les variables : z_class en 1er + ordre alphabétique
  ord <- order(names(out$variable$type))
  indZ_class <- which(names(out$variable$type) == "z_class")
  newOrd <- c(indZ_class, setdiff(ord, indZ_class))
  
  out$variable$type = out$variable$type[newOrd]
  out$variable$param = out$variable$param[newOrd]
  out$variable$data = out$variable$data[newOrd]
  
  if(out$mixture$warnLog == "")
    out$mixture$warnLog = NULL
  
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
convertParam <- function(param, type, nbClass = 1, nCoeff = 1, nSub = 1, mode = c("learn", "predict"))
{
  mode = match.arg(mode)
  
  nomObj <- switch(type, 
                   "Ordinal" = "muPi",
                   "LatentClass" = "pi",
                   "Functional" = c("alpha", "beta", "sd"),
                   "FunctionalSharedAlpha" = c("alpha", "beta", "sd"),
                   "NumericalParam")
  
  for(i in seq_along(nomObj))
  {
    if(mode == "learn")
    {
      param[[nomObj[i]]]$log = do.call(rbind, param[[nomObj[i]]]$log)
      colnames(param[[nomObj[i]]]$log) = rep("", ncol(param[[nomObj[i]]]$log))
      param[[nomObj[i]]]$stat = do.call(cbind, param[[nomObj[i]]]$stat)
    }
    
    if(mode == "predict")
    {  
      param[[nomObj[i]]]$stat = matrix(param[[nomObj[i]]]$stat$value, ncol = 1)
      colnames(param[[nomObj[i]]]$stat) = "value"
    }
    
    nomRow <- switch(type,
                     "Ordinal" = unlist(lapply(paste0("k: ", 1:nbClass), function(x){paste0(x, c(", mu:", ", pi"))})),
                     "Gaussian_sjk" = unlist(lapply(paste0("k: ", 1:nbClass), function(x){paste0(x, c(", mean:", ", sd"))})),
                     "Categorical_pjk" = nomRowParamCateg(param),
                     "Poisson_k" = paste0("k: ", 1:nbClass, ", lambda"),
                     "NegativeBinomial" = unlist(lapply(paste0("k: ", 1:nbClass), function(x){paste0(x, c(", n:", ", p"))})),
                     "Weibull" = unlist(lapply(paste0("k: ", 1:nbClass), function(x){paste0(x, c(", k:", ", lambda"))})),
                     "LatentClass" = paste0("k: ", 1:nbClass),
                     "Functional"= nomRowParamFunctional(nomObj[i], nbClass, nSub, nCoeff),
                     "FunctionalSharedAlpha"= nomRowParamFunctional(nomObj[i], nbClass, nSub, nCoeff))
    
    rownames(param[[nomObj[i]]]$stat) = nomRow
  }
  
  
  
  
  return(param)
}


# param outMixtComp$variable$param$nomVar
nomRowParamCateg <- function(param)
{
  nModality <- as.numeric(gsub("nModality: ", "", param$NumericalParam$paramStr))
  
  return(unlist(lapply(paste0("k: ", 1:(nrow(param$NumericalParam$stat)/nModality)), function(x){paste0(x, paste0(", modality: ", c(1:nModality)))})))
}


nomRowParamFunctional <- function(nomObj, nbClass, nSub, nCoeff)
{
  switch(nomObj, 
         "alpha"= apply(expand.grid(0:1,0:(nSub-1), 0:(nbClass-1))[,3:1], 1, function(x){paste0("k: ",x[1],", s: ",x[2],", alpha",x[3] )}),
         "beta" = apply(expand.grid(0:(nCoeff-1), 0:(nSub-1), 0:(nbClass-1))[,3:1], 1, function(x){paste0("k: ",x[1],", s: ",x[2],", c: ",x[3] )}),
         "sd"   = apply(expand.grid(0:(nSub-1), 0:(nbClass-1))[,2:1], 1, function(x){paste0("k: ",x[1],", s: ",x[2])}))
}