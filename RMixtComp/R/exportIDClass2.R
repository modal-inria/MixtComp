# Compute.proba.vbles.class <- function(results){
#   d <- length(results$variable$type) - 1
#   G <- results$mixture$nbCluster
#   proportions <- results$variable$param$z_class$pi$stat[,1]
#   proba.vbles <- lapply(1:d, function(j){
#     name <- names(results$variable$type)[j+1]
#     type <- results$variable$type[j+1]
#     eval(parse(text = paste("tmpdata <- results$variable$data$", name, "$completed", sep="")))
#     if (type=="Categorical_pjk"){
#       eval(parse(text = paste("alpha <- matrix(results$variable$param$", name, "$NumericalParam$stat[,1], nrow=G, byrow=TRUE)", sep="")))
#       out <- sapply(1:G, function(k) log(proportions[k]) + log(alpha[k,tmpdata]))
#       rm(alpha)
#     }else if (type=="Gaussian_sjk"){
#       eval(parse(text = paste("param <- matrix(results$variable$param$", name, "$NumericalParam$stat[,1], nrow=G, ncol=2, byrow=TRUE)", sep="")))
#       out <- sapply(1:G, function(k) log(proportions[k]) + dnorm(tmpdata, param[k,1], param[k,2], log=TRUE))
#       rm(param)
#     }else if (type=="Ordinal"){
#       eval(parse(text = paste("param <- matrix(results$variable$param$", name, "$muPi$stat[,1], nrow=G, ncol=2, byrow=TRUE)", sep="")))
#       out <- sapply(1:G, function(k) log(proportions[k]) + log(dord(tmpdata, param[k,1], param[k,2])))
#       rm(param)
#     }else if (type=="Poisson_k"){
#       eval(parse(text = paste("param <- as.numeric(results$variable$param$", name, "$NumericalParam$stat[,1])", sep="")))
#       out <- sapply(1:G, function(k) log(proportions[k]) + dpois(tmpdata, param[k], log=TRUE))
#       rm(param)
#     }else if (type=="Rank"){
#       eval(parse(text = paste("pi <- as.numeric(results$variable$param$", name, "$pi$stat[,1])", sep="")))
#       mu <- output$variable$param$rank1$mu$stat[[k]][[1]][[1]]
#       out <- sapply(1:G, function(k) log(proportions[k]) + drank(tmpdata, mu[k,], pi[k]))
#     }
#     out <- exp(sweep(out, 1,  apply(out, 1, max), "-"))
#     return(out / rowSums(out))
#   })
#   return(proba.vbles)
# }



