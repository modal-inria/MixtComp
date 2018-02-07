#' @name plotDataCI
#' @aliases plotFunctionalData plotCategoricalData plotCIIntegerData plotCIContinuousData
#' 
#' @title Mean and 95\%-level confidence intervals per class
#' 
#' @usage plotDataCI(output, var, ...)
#' plotCIContinuousData(output, var)
#' plotCIIntegerData(output, var)
#' plotCategoricalData(output, var)
#' plotFunctionalData(output, var, add.obs = FALSE, ylim = NULL, xlim = NULL)
#' 
#' @param output object returned by function \link{mixtCompCluster}
#' @param var name of the variable
#' @param ... other parameters (see \emph{Details})
#' 
#' @details For functional data, three other parameters are available:
#' \describe{
#'  \item{add.obs}{if TRUE, observations are added to the plot. Default = FALSE.}
#'  \item{ylim}{ylim of the plot.}
#'  \item{xlim}{xlim of the plot.}
#' }
#' 
#' 
#' @examples 
#' \dontrun{
#' # path to files
#' pathToData <- system.file("extdata", "data.csv", package = "RMixtComp")
#' pathToDescriptor <- system.file("extdata", "descriptor.csv", package = "RMixtComp")
#' 
#' resGetData <- getData(c(pathToData, pathToDescriptor))
#' 
#' 
#' # define the algorithm's parameters
#' mcStrategy <- list(nbBurnInIter = 100,
#'                    nbIter = 100,
#'                    nbGibbsBurnInIter = 50,
#'                    nbGibbsIter = 50)
#' 
#' # run RMixtCompt for clustering
#' res <- mixtCompCluster(resGetData$lm, mcStrategy, nbClass = 2, confidenceLevel = 0.95)
#' 
#' # plot
#' plotDataCI(res, "gaussian1")
#' 
#' } 
#' 
#' @author Matthieu MARBAC
plotDataCI <- function(output, var, class=1:output$mixture$nbCluster, grl=FALSE,...)
{
  if(!(var%in%names(output$variable$type)))
    stop("This variable does not exist in the mixture model.")
  
  type <- output$variable$type[[var]]
  
  switch(type,
         "Gaussian_sjk" = plotCINumericData(extractCIGaussianVble(var, output, class, grl), var, class, grl),
         "Categorical_pjk" = plotCategoricalData(extractCIMultiVble(var, output, class, grl), var, class, grl),
         "Poisson_k" = plotCINumericData(extractCIPoissonVble(var, output, class, grl), var, class, grl),
         "Functional" = plotFunctionalData(output, var, ...),
         cat("Not yet implemented"))
}


# Mean and 95% confidence level per class for a numeric variable (Gaussian or Poisson)
plotCINumericData <- function(data, var, class, grl){
  text1 <- paste0("Class.", class, "<br>",
                 "Mean: ", round(data$mean[1:length(class)],2), "<br>",
                 "CI-95%: [", round(data$lower[1:length(class)],2),
                 ",", round(data$uppers[1:length(class)],2),"]")
  if (grl) text1 <- c(text1, paste0("Grl <br>",
                                    "Mean: ", round(data$mean[length(data$mean)],2), "<br>",
                                    "CI-95%: [", round(data$lower[length(data$mean)],2),
                                    ",", round(data$uppers[length(data$mean)],2),"]"))
  p <- plot_ly(x=data$mean, 
               y=c(1:length(data$mean)),
               type="scatter",
               mode = 'markers',
               showlegend = FALSE,
               hoverinfo = "text",
               text = text1
  )%>%layout(title = "Mean and 95%-level confidence intervals per class",
             paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
             xaxis = list(title = var,
                          titlefont = list(
                            family = "Arial, sans-serif",
                            size = 18,
                            color = "lightgrey"
                          ),
                          gridcolor = 'rgb(255,255,255)',
                          showgrid = TRUE,
                          showline = FALSE,
                          showticklabels = TRUE,
                          tickcolor = 'rgb(127,127,127)',
                          ticks = 'outside',
                          zeroline = FALSE,
                          range=c(min(data$lower), max(data$uppers)) + c(-1,1) * abs(max(data$uppers)-min(data$lower))/10),
             yaxis = list(title = var,
                          gridcolor = 'rgb(255,255,255)',
                          showgrid = F,
                          showline = FALSE,
                          showticklabels = F,
                          tickcolor = 'rgb(127,127,127)',ticks="",
                          zeroline = FALSE))
  for(i in 1:length(class)){
    p <- add_trace(p,
                   x = c(data$lower[i], data$uppers[i]),  # x0, x1
                   y = c(i, i),  # y0, y1
                   mode = "lines",
                   line = list(color='rgba(0,100,80,0.4)',width = 20),
                   showlegend = FALSE,
                   hoverinfo = "text",
                   text = paste("Class.", i, "<br>",
                                "Mean: ", round(data$mean[i],2), "<br>",
                                "CI-95%: [", round(data$lower[i],2),
                                ",", round(data$uppers[i],2),"]",sep="")
    )
  }
  if (grl){
    i <- length(data$lower)
    p <- add_trace(p,
                   x = c(data$lower[i], data$uppers[i]),  # x0, x1
                   y = c(i, i),  # y0, y1
                   mode = "lines",
                   line = list(color='rgba(100, 10,80,0.4)',width = 20),
                   showlegend = FALSE,
                   hoverinfo = "text",
                   text = paste("Class.", i, "<br>",
                                "Mean: ", round(data$mean[i],2), "<br>",
                                "CI-95%: [", round(data$lower[i],2),
                                ",", round(data$uppers[i],2),"]",sep="")
    )
  }
  
  print(p)
}

# # Mean and 95%-level confidence intervals per class for a Poisson Mixture
# plotCIIntegerData <- function(data, var, class, grl){
#   p <- plot_ly(x=data$mean,
#                y=1:length(data$mean),
#                type="scatter",
#                mode = 'markers',
#                showlegend = FALSE,
#                hoverinfo = "text",
#                text = paste("Class.", 1:length(data$mean), "<br>",
#                             "Mean: ", round(data$mean,2), "<br>",
#                             "CI-95%: [", round(data$lower,2),
#                             ",", round(data$upper,2),"]",sep="")
#   )%>%
#     layout(title = "Mean and 95%-level confidence intervals per class",
#            paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
#            xaxis = list(title = var,
#                         gridcolor = 'rgb(255,255,255)',
#                         showgrid = TRUE,
#                         showline = FALSE,
#                         showticklabels = TRUE,
#                         tickcolor = 'rgb(127,127,127)',
#                         ticks = 'outside',
#                         zeroline = FALSE),
#            yaxis = list(title = var,
#                         gridcolor = 'rgb(255,255,255)',
#                         showgrid = F,
#                         showline = FALSE,
#                         showticklabels = T,
#                         tickvals=1:length(data$mean),
#                         tickcolor = 'rgb(127,127,127)',ticks="",
#                         zeroline = FALSE))
#   p
#   for(i in 1:length(data$mean)){
#     p <- add_trace(p,
#                    x = c(data$lower[i], data$upper[i]),  # x0, x1
#                    y = c(i, i),  # y0, y1
#                    mode = "lines",
#                    line = list(color='rgba(0,100,80,0.4)',width = 20),
#                    showlegend = FALSE,
#                    hoverinfo = "text",
#                    text = paste("Class.", i, "<br>",
#                                 "Mean: ", round(data$mean[i],2), "<br>",
#                                 "CI-95%: [", round(data$lower[i],2),
#                                 ",", round(data$upper[i],2),"]",sep="")
#     )
#   }
#   print(p)
# }

# Barplot for categorical data (only the levels included in the 95 confidence level for at least one component are plotted)
plotCategoricalData <- function(data, var, class, grl){
  formattedW <- lapply(1:length(class), 
                       function(k) list(y=data$probs[k,],
                                        type='bar',
                                        hoverinfo = "text",
                                        text=paste("class", class[k], sep="."),
                                        showlegend=FALSE,
                                        marker = list(line = list(color = 'black', width = 1.5)))
  )
  if (grl)
    formattedW <- c(formattedW,
                    list(list(y=data$probs[nrow(data$probs),],
                         type='bar',
                         hoverinfo = "text",
                         text="grl",
                         showlegend=FALSE,
                         marker = list(line = list(color = 'red', width = 1.5)))))
  # Reduce the list of plotly compliant objs, starting with the plot_ly() value and adding the `add_trace` at the following iterations
  p <- Reduce(function(acc, curr)  do.call(add_trace, c(list(p=acc),curr)),
              formattedW,
              init=plot_ly(x = data$levels)%>%
                layout(title = "Distribution per class",
                       paper_bgcolor='rgb(255,255,255)', plot_bgcolor='rgb(229,229,229)',
                       xaxis = list(title = "Levels",
                                    gridcolor = 'rgb(255,255,255)',
                                    showgrid = TRUE,
                                    showline = FALSE,
                                    showticklabels = TRUE,
                                    tickcolor = 'rgb(127,127,127)',
                                    ticks = 'outside',
                                    tickvals=data$levels,
                                    zeroline = FALSE),
                       yaxis = list(title = var,
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

# Mean and 95% confidence level confidence  for functional data
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