# Mean and 95%-level confidence intervals per class for a Gaussian Mixture
plotContinuousData <- function(output, var){
  #### This part computes the element corresponding to variable "var" of
  #### the slot cibounds of the JSON file 
  data <- extractCIGaussianVble(var, output)
  ### End of part corresponding the JSON file
  p <- plot_ly(x=data$mean,
               y=c(1:length(data$mean)),
               type="scatter",
               mode = 'markers',
               showlegend = FALSE,
               hoverinfo = "text",
               text = paste("Class.", 1:length(data$mean), "<br>",
                            "Mean: ", round(data$mean,2), "<br>",
                            "CI-95%: [", round(data$lower,2),
                            ",", round(2*data$mean - data$lower,2),"]",sep="")
  )%>%layout(title = "Mean and 95%-level confidence intervals per class",
           paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
           xaxis = list(title = var,
                        gridcolor = 'rgb(255,255,255)',
                        showgrid = TRUE,
                        showline = FALSE,
                        showticklabels = TRUE,
                        tickcolor = 'rgb(127,127,127)',
                        ticks = 'outside',
                        zeroline = FALSE),
           yaxis = list(title = "Classes",
                        gridcolor = 'rgb(255,255,255)',
                        showgrid = F,
                        showline = FALSE,
                        showticklabels = T,
                        tickvals=1:length(data$mean),
                        tickcolor = 'rgb(127,127,127)',ticks="",
                        zeroline = FALSE))
  for(i in 1:length(data$mean)){
    p <- add_trace(p,
                   x = c(data$lower[i], 2*data$mean[i] - data$lower[i]),  # x0, x1
                   y = c(i, i),  # y0, y1
                   mode = "lines",
                   line = list(color='rgba(0,100,80,0.4)',width = 20),
                   showlegend = FALSE,
                   hoverinfo = "text",
                   text = paste("Class.", i, "<br>",
                                "Mean: ", round(data$mean[i],2), "<br>",
                                "CI-95%: [", round(data$lower[i],2),
                                ",", round(2*data$mean[i] - data$lower[i],2),"]",sep="")
    )
  }
  print(p)
}

# Mean and 95%-level confidence intervals per class for a Poisson Mixture
plotIntegerData <- function(output, var){
  #### This part computes the element corresponding to variable "var" of
  #### the slot cibounds of the JSON file 
  data <- extractCIPoissonVble(var, output)
  ### End of part corresponding the JSON file
  p <- plot_ly(x=data$mean,
               y=1:length(data$mean),
               type="scatter",
               mode = 'markers',
               showlegend = FALSE,
               hoverinfo = "text",
               text = paste("Class.", 1:length(data$mean), "<br>",
                            "Mean: ", round(data$mean,2), "<br>",
                            "CI-95%: [", round(data$lower,2),
                            ",", round(data$upper,2),"]",sep="")
  )%>%
    layout(title = "Mean and 95%-level confidence intervals per class",
           paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
           xaxis = list(title = var,
                        gridcolor = 'rgb(255,255,255)',
                        showgrid = TRUE,
                        showline = FALSE,
                        showticklabels = TRUE,
                        tickcolor = 'rgb(127,127,127)',
                        ticks = 'outside',
                        zeroline = FALSE),
           yaxis = list(title = "Classes",
                        gridcolor = 'rgb(255,255,255)',
                        showgrid = F,
                        showline = FALSE,
                        showticklabels = T,
                        tickvals=1:length(data$mean),
                        tickcolor = 'rgb(127,127,127)',ticks="",
                        zeroline = FALSE))
  p
  for(i in 1:length(data$mean)){
    p <- add_trace(p,
                   x = c(data$lower[i], data$upper[i]),  # x0, x1
                   y = c(i, i),  # y0, y1
                   mode = "lines",
                   line = list(color='rgba(0,100,80,0.4)',width = 20),
                   showlegend = FALSE,
                   hoverinfo = "text",
                   text = paste("Class.", i, "<br>",
                                "Mean: ", round(data$mean[i],2), "<br>",
                                "CI-95%: [", round(data$lower[i],2),
                                ",", round(data$upper[i],2),"]",sep="")
    )
  }
  print(p)
}

# Mode and 95%-level confidence intervals per class for a Multinomial Mixture
plotCategoricalData <- function(output, var){
  data <- extractCIMultiVble(var, output)
  formattedW <- lapply(1:output$mixture$nbCluster, 
                       function(k) list(y=data$probs[k,],
                                        type='bar',
                                        hoverinfo = "text",
                                        text=paste("class", k, sep="."),
                                        showlegend=FALSE,
                                        marker = list(line = list(color = 'black', width = 1.5)))
  )
  
  # Reduce the list of plotly compliant objs, starting with the plot_ly() value and adding the `add_trace` at the following iterations
  p <- Reduce(function(acc, curr)  do.call(add_trace, c(list(p=acc),curr)),
              formattedW,
              init=plot_ly(x = data$levels)%>%
                layout(title = "Mode and 95%-level confidence intervals per class",
                       paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
                       xaxis = list(title = paste("Levels of",var),
                                    gridcolor = 'rgb(255,255,255)',
                                    showgrid = TRUE,
                                    showline = FALSE,
                                    showticklabels = TRUE,
                                    tickcolor = 'rgb(127,127,127)',
                                    ticks = 'outside',
                                    tickvals=data$levels,
                                    zeroline = FALSE),
                       yaxis = list(title = "Probability",
                                    gridcolor = 'rgb(255,255,255)',
                                    showgrid = TRUE,
                                    showline = FALSE,
                                    showticklabels = TRUE,
                                    tickcolor = 'rgb(127,127,127)',
                                    ticks = 'outside',
                                    zeroline = FALSE))
  )
  print(p)
}

# Mean and 95%-level confidence intervals per class for a Functional Mixture
plotFunctionalData <- function(output, var, add.obs=FALSE, ylim=NULL, xlim=NULL){
  # Computation of the Confidence Intervals (CI)
  data <- extractCIFunctionnalVble(var, output)
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
  for(k in 1:G){
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
  print(p)
}