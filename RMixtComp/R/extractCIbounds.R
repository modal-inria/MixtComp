extractCIGaussianVble = function(var, data){
  theta = matrix(data$variable$param[[var]]$NumericalParam$stat[,1], ncol=2, byrow=TRUE)
  means = as.array(round(theta[,1], 3))
  lowers = as.array(round(qnorm(0.025, theta[,1], sqrt(theta[,2])), 3))
  return(list(mean = means,lower = lowers))
}

extractCIPoissonVble = function(var, data){
  theta = data$variable$param[[var]]$NumericalParam$stat[,1]
  return(list(mean = theta,lower = qpois(0.025, theta), upper=qpois(0.975, theta)))
}

extractCIMultiVble = function(var, data){
  theta = matrix(data$variable$param[[var]]$NumericalParam$stat[,1], nrow=data$mixture$nbCluster, byrow=TRUE)
  
  for (k in 1:nrow(theta)){
    orderk <- order(theta[k,], decreasing=TRUE)
    keep <- orderk[1:which(cumsum(theta[k, orderk])>0.95)[1]]
    theta[k, -keep] <- 0
  }
  
  theta = round(theta, 2)
  out = cbind(1:ncol(theta), t(theta))
  # drop the levels that do not belong to the CI of all the classes
  if (any(rowSums(out) == out[,1])) out <- out[-which(rowSums(out) == out[,1]),, drop = FALSE]
  return(list(levels=out[,1], probs = t(out[,-1])))
}

## To compute the mean curve per component
functionalmeanVal <- function(Tt, alpha, beta){
  weights <- alpha[,2] * Tt + alpha[,1]
  weights <- weights - max(weights)
  weights <- exp(weights) / sum(exp(weights))
  sum((beta[,1] + beta[,2] * Tt) * weights)
}

## To compute the lower/upper bound of the 95%-level confidence interval of the curve per component
functionalboundVal <- function(Tt, borne, alpha, beta, sigma){
  weights <- alpha[,1] + alpha[,2] * Tt
  weights <- weights - max(weights)
  weights <- exp(weights) / sum(exp(weights))
  means <- beta[,1] + beta[,2] * Tt 
  # Newton-Raphson to get the bound
  u <-  sum(qnorm(borne, means, sqrt(sigma))*(weights))
  Fu <- function(u) (sum(weights * pnorm(u, means, sqrt(sigma)) ) - borne)
  fu <- function(u) 2*sum(weights * dnorm(u, means, sqrt(sigma)) )
  cond <- 1
  while (cond ==1 ){
    u <- u - Fu(u) / fu(u)
    cond <- (abs(Fu(u))> 0.0001)
    if (is.na(cond)){u <- qnorm(borne, means, sqrt(sigma))[which.max(weights)]; cond <- 0;}
  }
  return(u)
}

extractCIFunctionnalVble = function(var, data){
  ### Warnings, the range of the time must be found in the data set!!!!!
  Tseq <- unique(unlist(data$variable$data[[var]]$time))
  param = data$variable$param[[var]]
  G <- data$mixture$nbCluster
  S <- length(param$sd$stat[,1])/G
  alpha <- lapply(1:G, function(g) matrix(param$alpha$stat[,1], ncol=2, byrow=TRUE)[((g-1)*S +1) : (g*S),])
  beta <- lapply(1:G, function(g) matrix(param$beta$stat[,1], ncol=2, byrow=TRUE)[((g-1)*S +1) : (g*S),])
  sigma <- matrix(param$sd$stat[,1], nrow=G, ncol=S, byrow=TRUE)
  if (S>1){
    meancurve <- sapply(1:G, function(k) sapply(Tseq, functionalmeanVal, alpha=alpha[[k]], beta=beta[[k]]))
    infcurve <- sapply(1:G, function(k) sapply(Tseq, functionalboundVal, borne=0.025, alpha=alpha[[k]], beta=beta[[k]], sigma=sigma[k,]))
    supcurve <- sapply(1:G, function(k) sapply(Tseq, functionalboundVal, borne=0.975, alpha=alpha[[k]], beta=beta[[k]], sigma=sigma[k,]))
  }else{
    meancurve <- sapply(1:G, function(k) (beta[[k]][1] + beta[[k]][2] * Tseq))
    infcurve <- sapply(1:G, function(k) qnorm(0.025, meancurve[,k], sqrt(sigma[k,1])))
    supcurve <- sapply(1:G, function(k) qnorm(0.975, meancurve[,k], sqrt(sigma[k,1])))
  }
  
  #out <- data.frame(Time=Tseq, meancurve, infcurve, supcurve)
  #colnames(out) <- c("Time",
  #                   paste("mean.class", 1:G, sep="."), 
  #                   paste("inf.class", 1:G, sep="."), 
  #                   paste("sup.class", 1:G, sep="."))
  out = list(time=Tseq, mean=t(meancurve), inf=t(infcurve), sup=t(supcurve))
  return(out)
}

extractCIboundsOneVble = function(var, data){
  out <- NULL
  eval(parse(text=paste("type <- data$variable$type$", var,sep="")))
  if (type == "Gaussian_sjk") out <- extractCIGaussianVble(var, data)
  if (type == "Poisson_k") out <-  extractCIPoissonVble(var, data)
  if (type == "Categorical_pjk") out <-  extractCIMultiVble(var, data)
  if (type == "Functional") out <-  extractCIFunctionnalVble(var, data)
  return(out)
}
