## To compute the mean curve per component
meanVal <- function(Tt, alpha, beta){
  weights <- alpha[,2] * Tt + alpha[,1]
  weights <- weights - max(weights)
  weights <- exp(weights) / sum(exp(weights))
  sum((beta[,1] + beta[,2] * Tt) * weights)
}

## To compute the lower bound of the 95%-level confidence interval of the curve per component
infVal <- function(Tt, alpha, beta, sigma){
  weights <- alpha[,1] + alpha[,2] * Tt
  weights <- weights - max(weights)
  weights <- exp(weights) / sum(exp(weights))
  means <- beta[,1] + beta[,2] * Tt 
  borne <- 0.025
  # Newton-Raphson to get the bound
  u <-  qnorm(borne, means, sqrt(sigma))[which.max(weights)]
  Fu <- function(u) (sum(weights * pnorm(u, means, sqrt(sigma)) ) - borne)
  fu <- function(u) 2*sum(weights * dnorm(u, means, sqrt(sigma)) ) 
  while (abs(Fu(u))> 0.00001)  u <- u - Fu(u) / fu(u) 
  return(u)
}

## To have a data.frame with the time, the bounds of the 95%-level CI and the mean curbe per component 
computeDataCurve <- function(output, Tseq){
  G <- output$mixture$nbCluster
  tmpalpha <- matrix(output$variable$param$V0$alpha$stat[,1], ncol=2, byrow=TRUE)
  tmpbeta <- matrix(output$variable$param$V0$beta$stat[,1], ncol=2, byrow=TRUE)
  S <- nrow(tmpalpha) / G
  alpha <- lapply(1:G, function(g) tmpalpha[((g-1)*S +1) : (g*S),])
  beta <- lapply(1:G, function(g) tmpbeta[((g-1)*S +1) : (g*S),])
  sigma <- matrix(output$variable$param$V0$sd$stat[,1], nrow=G, ncol=S, byrow=TRUE)
  if (S>1){
    meancurve <- sapply(1:G, function(k) sapply(Tseq, meanVal, alpha=alpha[[k]], beta=beta[[k]]))
    infcurve <- sapply(1:G, function(k) sapply(Tseq, infVal, alpha=alpha[[k]], beta=beta[[k]], sigma=sigma[k,]))
  }else{
    meancurve <- sapply(1:G, function(k) (beta[[k]][1] + beta[[k]][2] * Tseq))
    infcurve <- sapply(1:G, function(k) qnorm(0.025, meancurve[,k], sqrt(sigma[k,1])))
  }
  supcurve <- 2 * meancurve - infcurve
  data <- data.frame(Time=Tseq, meancurve, infcurve, supcurve)
  colnames(data) <- c("Time", paste("mean.class", 1:G, sep="."), paste("inf.class", 1:G, sep="."), paste("sup.class", 1:G, sep="."))
  return(data)
}

## Plot, Time defines the space where the curves are computed  
plotCurves <- function(output, Time){
  G <- output$mixture$nbCluster
  data <- computeDataCurve(output, Time)
  formattedW <- NULL
  for(k in 1:G){
    formattedW <- c(formattedW,  list(
      list(y = data[,1+k+G], 
           type = 'scatter', mode = 'lines',
           line = list(color = 'transparent'),showlegend = FALSE,
           name = paste("95% lcl class",k,sep=".")),
      list(y = data[,1+k+2*G], 
           type = 'scatter', mode = 'lines',
           fill = 'tonexty', fillcolor='rgba(0,100,80,0.4)', line = list(color = 'transparent'),
           showlegend = FALSE, name = paste("95% ucl class",k,sep=".")),
      list(y = data[,1+k], 
           type = 'scatter', mode = 'lines',
           line = list(color=k, width = 4),showlegend = FALSE, 
           name = paste("mean class",k,sep="."))
    ))
  }
  # Reduce the list of plotly compliant objs, starting with the plot_ly() value and adding the `add_trace` at the following iterations
  p <- Reduce(function(acc, curr)  do.call(add_trace, c(list(p=acc),curr)),
              formattedW,
              init=plot_ly(x = data$Time)%>%
                layout(title = "Mean curves and 95%-level confidence intervals per class",
                       paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
                       xaxis = list(title = "Time",
                                    gridcolor = 'rgb(255,255,255)',
                                    showgrid = TRUE,
                                    showline = FALSE,
                                    showticklabels = TRUE,
                                    tickcolor = 'rgb(127,127,127)',
                                    ticks = 'outside',
                                    zeroline = FALSE),
                       yaxis = list(title = "Value",
                                    gridcolor = 'rgb(255,255,255)',
                                    showgrid = TRUE,
                                    showline = FALSE,
                                    showticklabels = TRUE,
                                    tickcolor = 'rgb(127,127,127)',
                                    ticks = 'outside',
                                    zeroline = FALSE))
  )
  p
}