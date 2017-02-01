# Mean and 95%-level confidence intervals per class for a Gaussian Mixture
plotContinuousData <- function(output, var){
  #### This part computes the element corresponding to variable "var" of
  #### the slot cibounds of the JSON file 
  data <- extractCIGaussianVble(var, output)
  ### End of part corresponding the JSON file
  p <- plot_ly(x=data$mean,
               y=c(1:nrow(data)),
               type="scatter",
               mode = 'markers',
               showlegend = FALSE,
               hoverinfo = "text",
               text = paste("Class.", 1:ncol(data), "<br>",
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
                        tickvals=1:nrow(data),
                        tickcolor = 'rgb(127,127,127)',ticks="",
                        zeroline = FALSE))
  p
  for(i in 1:nrow(data)){
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

# Mean and 95%-level confidence intervals per class for a Gaussian Mixture
plotIntegerData <- function(output, var){
  #### This part computes the element corresponding to variable "var" of
  #### the slot cibounds of the JSON file 
  data <- extractCIPoissonVble(var, output)
  ### End of part corresponding the JSON file
  p <- plot_ly(x=data$mean,
               y=1:nrow(out),
               type="scatter",
               mode = 'markers',
               showlegend = FALSE,
               hoverinfo = "text",
               text = paste("Class.", 1:nrow(out), "<br>",
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
                        tickvals=1:nrow(out),
                        tickcolor = 'rgb(127,127,127)',ticks="",
                        zeroline = FALSE))
  p
  for(i in 1:nrow(out)){
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


plotCategoricalData <- function(output, var){
  data <- extractCIMultiVble(var, output)
  formattedW <- lapply(1:output$mixture$nbCluster, 
                       function(k) list(y=data[,k+1],
                                        type='bar',
                                        hoverinfo = "text",
                                        text=paste("class",k,sep="."),
                                        showlegend=FALSE,
                                        marker = list(line = list(color = 'black', width = 1.5)))
  )
  
  # Reduce the list of plotly compliant objs, starting with the plot_ly() value and adding the `add_trace` at the following iterations
  p <- Reduce(function(acc, curr)  do.call(add_trace, c(list(p=acc),curr)),
              formattedW,
              init=plot_ly(x = data$level)%>%
                layout(title = "Mean curves and 95%-level confidence intervals per class",
                       paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
                       xaxis = list(title = paste("Level of",var),
                                    gridcolor = 'rgb(255,255,255)',
                                    showgrid = TRUE,
                                    showline = FALSE,
                                    showticklabels = TRUE,
                                    tickcolor = 'rgb(127,127,127)',
                                    ticks = 'outside',
                                    tickvals=data$level,
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


# Mean and 95%-level confidence intervals per class for a Gaussian Mixture
plotIntegerData <- function(output, var){
  #### This part computes the element corresponding to variable "var" of
  #### the slot cibounds of the JSON file 
  data <- extractCIPoissonVble(var, output)
  ### End of part corresponding the JSON file
  p <- plot_ly(x=data$mean,
               y=1:nrow(out),
               type="scatter",
               mode = 'markers',
               showlegend = FALSE,
               hoverinfo = "text",
               text = paste("Class.", 1:nrow(out), "<br>",
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
                        tickvals=1:nrow(out),
                        tickcolor = 'rgb(127,127,127)',ticks="",
                        zeroline = FALSE))
  p
  for(i in 1:nrow(out)){
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


plotCategoricalData <- function(output, var){
  data <- extractCIMultiVble(var, output)
  formattedW <- lapply(1:output$mixture$nbCluster, 
                       function(k) list(y=data[,k+1],
                                        type='bar',
                                        hoverinfo = "text",
                                        text=paste("class",k,sep="."),
                                        showlegend=FALSE,
                                        marker = list(line = list(color = 'black', width = 1.5)))
  )
  
  # Reduce the list of plotly compliant objs, starting with the plot_ly() value and adding the `add_trace` at the following iterations
  p <- Reduce(function(acc, curr)  do.call(add_trace, c(list(p=acc),curr)),
              formattedW,
              init=plot_ly(x = data$level)%>%
                layout(title = "Mean curves and 95%-level confidence intervals per class",
                       paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
                       xaxis = list(title = paste("Level of",var),
                                    gridcolor = 'rgb(255,255,255)',
                                    showgrid = TRUE,
                                    showline = FALSE,
                                    showticklabels = TRUE,
                                    tickcolor = 'rgb(127,127,127)',
                                    ticks = 'outside',
                                    tickvals=data$level,
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



plotFunctionallData <- function(output, var){
  data <- extractCIFunctionnalVble(var, output)
  G <- output$mixture$nbCluster
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
  print(p)
}

