functionalmeanValDegSup <- function(Tt, alpha, beta){
  weights <- alpha[,2] * Tt + alpha[,1]
  weights <- weights - max(weights)
  weights <- exp(weights) / sum(exp(weights))  # To avoid numerical problems
  weigths <- round(weights,4)
  weights <- weigths/sum(weights)
  sum(beta %*% (Tt**(0:(ncol(beta) -1))) * weights)
}

objectivefunctional <- function(x, pi, mu, s, seuil) (sum(pi*pnorm(x, mu, s)) - seuil)**2

functionalboundValVbleDegSup <- function(Tt, borne, alpha, beta, sigma){
  weights <- alpha[,1] + alpha[,2] * Tt
  weights <- weights - max(weights)
  weights <- exp(weights) / sum(exp(weights))
  # To avoid numerical problems
  weigths <- round(weights,4)
  weights <- weigths/sum(weights)
  means <- beta %*% (Tt**(0:(ncol(beta) -1))) 
  return(
    optimize(objectivefunctional,
             interval = range(qnorm(borne-0.001, means, sigma)[which(weights!=0)], qnorm(borne+0.001, means, sigma)[which(weights!=0)]),
             pi=weights,
             mu=means,
             s=sigma,
             seuil=borne)$minimum
  )
}

extractCIFunctionnalVbleDegSup <- function(var, data){
  Tseq <- sort(unique(unlist(data$variable$data[[var]]$time)), decreasing = F)
  param = data$variable$param[[var]]
  G <- data$mixture$nbCluster
  S <- length(param$sd$stat[,1])/G
  Q <- nrow(param$beta$stat)/(G*S)
  alpha <- lapply(1:G, function(g) matrix(param$alpha$stat[,1], ncol=2, byrow=TRUE)[((g-1)*S +1) : (g*S),, drop=FALSE])
  beta <- lapply(1:G, function(g) matrix(param$beta$stat[,1], ncol=Q, byrow=TRUE)[((g-1)*S +1) : (g*S),, drop=FALSE])
  sigma <- matrix(param$sd$stat[,1], nrow=G, ncol=S, byrow=TRUE)
  meancurve <- sapply(1:G, function(k) sapply(Tseq, functionalmeanValDegSup, alpha=alpha[[k]], beta=beta[[k]]))
  if (S>1){
    infcurve <- sapply(1:G, function(k) sapply(Tseq, functionalboundValVbleDegSup, borne=0.025, alpha=alpha[[k]], beta=beta[[k]], sigma=sigma[k,, drop=FALSE]))
    supcurve <- sapply(1:G, function(k) sapply(Tseq, functionalboundValVbleDegSup, borne=0.975, alpha=alpha[[k]], beta=beta[[k]], sigma=sigma[k,, drop=FALSE]))
  }else{
    infcurve <- sapply(1:G, function(k) qnorm(0.025, meancurve[,k, drop=FALSE], sigma[k,1]))
    supcurve <- sapply(1:G, function(k) qnorm(0.975, meancurve[,k, drop=FALSE], sigma[k,1]))
  }
  out = list(time=Tseq, mean=t(meancurve), inf=t(infcurve), sup=t(supcurve))
  return(out)
}

# Mean and 95%-level confidence intervals per class for a Functional Mixture
plotFunctionalDataDegSup <- function(output, var, class.set=1:output$mixture$nbCluster,add.obs=FALSE, ylim=NULL, xlim=NULL){
  # Computation of the Confidence Intervals (CI)
  data <- extractCIFunctionnalVbleDegSup(var, output)
  G <- output$mixture$nbCluster
  
  # Computation of the bounds for x-axis and y-axis
  if (is.null(xlim)) xlim <- range(sapply(1:length(output$variable$data[[var]]$time),
                                          function(j) range(output$variable$data[[var]]$time[[j]]) ))
  if (is.null(ylim)){
    if (add.obs){
      ylim <- range(c(min(data$inf), max(data$sup), sapply(1:length(output$variable$data[[var]]$data),
                                                           function(j) range(output$variable$data[[var]]$data[[j]]) )))  
    }else{
      ylim <- c(min(data$inf), max(data$sup))      
    }
  }
  formattedW <- NULL
  # observations are added for plot
  if (add.obs){
    for(i in 1:length(output$variable$data[[var]]$time)){
      formattedW <- c(formattedW,  list(
        list(x = output$variable$data[[var]]$time[[i]],
             y = output$variable$data[[var]]$data[[i]], 
             type = 'scatter', mode = 'lines',showlegend = FALSE,line = list(color='rgba(0,100,80,0.4)', width = 1) ))
      )
    }
  }
  # mean curves and CI are added
  for(k in class.set){
    formattedW <- c(formattedW,  list(
      list(y = data$inf[k,], 
           type = 'scatter', mode = 'lines',
           line = list(color = 'transparent'),showlegend = FALSE,
           name = paste("95% lcl class",k,sep=".")),
      list(y = data$sup[k,], 
           type = 'scatter', mode = 'lines',
           fill = 'tonexty', fillcolor='rgba(0,100,80,0.4)', line = list(color = 'transparent'),
           showlegend = FALSE, name = paste("95% ucl class",k,sep=".")),
      list(y = data$mean[k,], 
           type = 'scatter', mode = 'lines',
           line = list(color=k, width = 4),showlegend = FALSE, 
           name = paste("mean class",k,sep="."))
    ))
  }
  # Reduce the list of plotly compliant objs, starting with the plot_ly() value and adding the `add_trace` at the following iterations
  p <- Reduce(function(acc, curr)  do.call(add_trace, c(list(p=acc),curr)),
              formattedW,
              init=plot_ly(x = data$time)%>%
                layout(title = "Mean curves and 95%-level confidence intervals per class",
                       paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
                       xaxis = list(title = "Time",
                                    gridcolor = 'rgb(255,255,255)',
                                    showgrid = TRUE,
                                    showline = FALSE,
                                    showticklabels = TRUE,
                                    tickcolor = 'rgb(127,127,127)',
                                    ticks = 'outside',
                                    range=xlim,
                                    zeroline = FALSE),
                       yaxis = list(title = "Value",
                                    gridcolor = 'rgb(255,255,255)',
                                    showgrid = TRUE,
                                    showline = FALSE,
                                    showticklabels = TRUE,
                                    tickcolor = 'rgb(127,127,127)',
                                    ticks = 'outside',
                                    range=ylim,
                                    zeroline = FALSE))
  )
  p
}