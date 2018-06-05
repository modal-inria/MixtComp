################################################################################
### Confidence levels

## Get
extractCIboundsOneVble = function(var, data, class, grl){
  out <- NULL
  type <- data$variable$type[[var]]
  if (type == "Gaussian_sjk") out <- extractCIGaussianVble(var, data, class, grl)
  if (type == "Weibull") out <- extractCIWeibullVble(var, data, class, grl)
  if (type == "Poisson_k") out <-  extractCIPoissonVble(var, data, class, grl)
  if (type == "Categorical_pjk") out <-  extractCIMultiVble(var, data, class, grl)
  if (type == "Functional") out <-  extractCIFunctionnalVble(var, data, class, grl)
  return(out)
}

## CI bounds for numerical variables (Gaussian or Poisson)
extractCIGaussianVble = function(var, data, class, grl){
  theta = matrix(data$variable$param[[var]]$NumericalParam$stat[,1], ncol=2, byrow=TRUE)
  means = as.array(round(theta[,1], 3))
  lowers = as.array(round(qnorm(0.025, theta[,1], theta[,2]), 3))
  uppers = as.array(round(qnorm(0.975, theta[,1], theta[,2]), 3))
  means <- means[class]
  lowers <- lowers[class]
  uppers <- uppers[class]
  if (grl){
    means <- c(means, mean(data$variable$data[[var]]$completed))
    bounds <- ceiling(c(0.025, 0.975) * length(data$variable$data[[var]]$completed))
    tmp <- sort(data$variable$data[[var]]$completed)[bounds]
    lowers <- c(lowers, tmp[1])
    uppers <- c(uppers, tmp[2])
  }
  return(list(mean = means, lower = lowers, uppers=uppers))
}

extractCIPoissonVble = function(var, data, class, grl){
  theta <- as.array(data$variable$param[[var]]$NumericalParam$stat[class,1])
  if (grl)  theta <- cbind(theta, mean(data$variable$data[[var]]$completed))
  return(list(mean = theta, lower = qpois(0.025, theta), uppers=qpois(0.975, theta)))
}

extractCIWeibullVble = function(var, data, class, grl){
  theta = matrix(data$variable$param[[var]]$NumericalParam$stat[,1], ncol=2, byrow=TRUE)
  means = as.array(round(theta[,1], 3))
  lowers = as.array(round(qweibull(0.025, theta[,1], theta[,2]), 3))
  uppers = as.array(round(qweibull(0.975, theta[,1], theta[,2]), 3))
  means <- means[class]
  lowers <- lowers[class]
  uppers <- uppers[class]
  if (grl){
    means <- c(means, mean(data$variable$data[[var]]$completed))
    bounds <- ceiling(c(0.025, 0.975) * length(data$variable$data[[var]]$completed))
    tmp <- sort(data$variable$data[[var]]$completed)[bounds]
    lowers <- c(lowers, tmp[1])
    uppers <- c(uppers, tmp[2])
  }
  return(list(mean = means, lower = lowers, uppers=uppers))
}

## Categorical variables
extractCIMultiVble = function(var, data, class, grl){
  theta = matrix(data$variable$param[[var]]$NumericalParam$stat[,1], nrow=data$mixture$nbCluster, byrow=TRUE)
  theta <- theta[class, ,drop=FALSE]
  if (grl){
    tmp <- table(data$variable$data[[var]]$completed)
    theta <- rbind(theta, tmp/sum(tmp))
  }
  for (k in 1:nrow(theta)){
    orderk <- order(theta[k,], decreasing=TRUE)
    keep <- orderk[1:which(cumsum(theta[k, orderk])>0.95)[1]]
    theta[k, -keep] <- 0
  }
  
  theta = round(theta, 2)
  out = cbind(1:ncol(theta), t(theta))
  # drop the levels that do not belong to the CI of all the classes
  if (any(rowSums(out) == out[,1])) out <- out[-which(rowSums(out) == out[,1]),, drop = FALSE]
  return(list(levels=out[,1], probs = t(out[,-1, drop = FALSE])))
}

## Functional variables
# To compute the mean curve per component
functionalmeanVal <- function(Tt, alpha, beta){
  weights <- alpha[,2] * Tt + alpha[,1]
  weights <- weights - max(weights)
  weights <- exp(weights) / sum(exp(weights))  # To avoid numerical problems
  weigths <- round(weights,4)
  weights <- weigths/sum(weights)
  
  b <- 0
  for(i in 1:ncol(beta))
    b = b + beta[,i] * Tt^(i-1)

  sum(b * weights)
}

# Tool function for assessing the quantile for the functional model
objectivefunctional <- function(x, pi, mu, s, seuil) (sum(pi*pnorm(x, mu, s)) - seuil)**2

# To compute the lower/upper bound of the 95%-level confidence interval of the curve per component
functionalboundVal <- function(Tt, borne, alpha, beta, sigma){
  weights <- alpha[,1] + alpha[,2] * Tt
  weights <- weights - max(weights)
  weights <- exp(weights) / sum(exp(weights))
  # To avoid numerical problems
  weigths <- round(weights,4)
  weights <- weigths/sum(weights)
  means  <- 0
  for(i in 1:ncol(beta))
    means  = means  + beta[,i] * Tt^(i-1)

  
  return(
    optimize(objectivefunctional,
             interval = range(qnorm(borne-0.001, means, sigma)[which(weights!=0)], qnorm(borne+0.001, means, sigma)[which(weights!=0)]),
             pi=weights,
             mu=means,
             s=sigma,
             seuil=borne)$minimum
  )
}

extractCIFunctionnalVble = function(var, data){
  Tseq <- sort(unique(unlist(data$variable$data[[var]]$time)), decreasing = FALSE)
  param = data$variable$param[[var]]
  G <- data$mixture$nbCluster
  nSub <- length(param$sd$stat[,1])/G
  nCoeff <- length(param$beta$stat[,1])/G/nSub
  
  alpha <- lapply(1:G, function(g) matrix(param$alpha$stat[,1], ncol=2, byrow=TRUE)[((g-1)*nSub +1) : (g*nSub),, drop=FALSE])
  beta <- lapply(1:G, function(g) matrix(param$beta$stat[,1], ncol=nCoeff, byrow=TRUE)[((g-1)*nSub +1) : (g*nSub),, drop=FALSE])
  sigma <- matrix(param$sd$stat[,1], nrow = G, ncol = nSub, byrow=TRUE)
  if (nSub>1){
    meancurve <- sapply(1:G, function(k) sapply(Tseq, functionalmeanVal, alpha=alpha[[k]], beta=beta[[k]]))
    infcurve <- sapply(1:G, function(k) sapply(Tseq, functionalboundVal, borne=0.025, alpha=alpha[[k]], beta=beta[[k]], sigma=sigma[k,, drop=FALSE]))
    supcurve <- sapply(1:G, function(k) sapply(Tseq, functionalboundVal, borne=0.975, alpha=alpha[[k]], beta=beta[[k]], sigma=sigma[k,, drop=FALSE]))
  }else{

    
    meancurve <- sapply(1:G, function(k) {
      b <- 0
      for(i in 1:nCoeff) 
        b = b + beta[[k]][i] * Tseq^(i-1)
      return(b)
      })
    
    infcurve <- sapply(1:G, function(k) qnorm(0.025, meancurve[,k, drop=FALSE], sigma[k,1]))
    supcurve <- sapply(1:G, function(k) qnorm(0.975, meancurve[,k, drop=FALSE], sigma[k,1]))
  }
  out = list(time=Tseq, mean=t(meancurve), inf=t(infcurve), sup=t(supcurve))
  return(out)
}


############################################################################################
#### Boxplots

## Get
extractBoxplotInfoOneVble <- function(var, data, class=1:data$mixture$nbCluster, grl=FALSE){
  out <- NULL
  type <- data$variable$type[[var]]
  if ((type == "Gaussian_sjk") || (type == "Poisson_k")) out <- extractBoundsBoxplotNumericalVble(var, data, class, grl)
  if (type == "Categorical_pjk") out <-  extractBoundsBoxplotCategoricalVble(var, data, class, grl)
  if (type == "Functional") out <-  extractCIFunctionnalVble(var, data)
  return(out)
}

## Numerical variables 
extractBoundsBoxplotNumericalVble <- function(var, data, class=1:data$mixture$nbCluster, grl=FALSE) {
  obs <- data$variable$data[[var]]$completed
  tik <- data$variable$data$z_class$stat
  orderedIndices <- order(obs)
  cumsums <- apply(tik[orderedIndices,, drop=F], 2, cumsum)
  cumsums <- t(t(cumsums) / cumsums[nrow(cumsums), ])
  thresholds <- sapply(c(.05, .25, .5, .75, .95), 
                       function(threshold, cumsums) obs[orderedIndices[apply(abs(cumsums - threshold), 2, which.min)]], 
                       cumsums=cumsums)
  thresholds <- matrix(thresholds, nrow=data$mixture$nbCluster)
  thresholds <- thresholds[class, , drop=FALSE]
  rownames(thresholds) <- paste("component", class)
  colnames(thresholds) <- paste("quantil.", c(.05, .25, .5, .75, .95))
  if (grl){
    obs <- sort(data$variable$data[[var]]$completed, decreasing = FALSE)
    tmp <- round(c(.05, .25, .5, .75, .95) * length(obs))
    if (any(tmp==0)) tmp[which(tmp==0)] <- 1
    if (any(tmp>length(obs))) tmp[which(tmp>length(obs))] <- length(obs)
    thresholds <- rbind(thresholds, obs[tmp])
    rownames(thresholds)[nrow(thresholds)] <- "general"
  }
  
  return(thresholds)
}

## Categorical variables 
extractBoundsBoxplotCategoricalVble <- function(var, data, class=1:data$mixture$nbCluster, grl=FALSE) {
  obs <- data$variable$data[[var]]$completed
  tik <- data$variable$data$z_class$stat
  levels <- sort(unique(obs))
  probs <- t(sapply(1:data$mixture$nbCluster, 
                    function(k, tik, obs)
                      sapply(levels,
                             function(w, obs, level) sum(w * (obs == level)),
                             w=tik[,k],
                             obs=obs
                      ) / sum(tik[,k]),
                    tik=tik ,
                    obs=obs
  ))
  probs <- probs[class, , drop=FALSE]
  rownames(probs) <- paste("component", class)
  colnames(probs) <- levels
  if (grl){
    obs <- sort(table(data$variable$data[[var]]$completed), decreasing = TRUE)
    obs <- obs/sum(obs)
    probs <- rbind(probs, obs)
    rownames(probs)[nrow(probs)] <- "general"
  }
  return(list(levels=levels, probs=probs))
}

