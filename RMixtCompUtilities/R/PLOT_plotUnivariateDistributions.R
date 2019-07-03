# MixtComp version 4 - july 2019
# Copyright (C) Inria - Université de Lille - CNRS 

#' Mean and 95\%-level confidence intervals per class
#' 
#' 
#' @param output object returned by function \emph{mixtCompLearn}
#' @param var name of the variable
#' @param class class to plot
#' @param grl if TRUE plot the CI for the dataset and not only classes
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... other parameters (see \emph{Details})
#' 
#' @details For functional data, three other parameters are available:
#' \describe{
#'  \item{add.obs}{if TRUE, observations are added to the plot. Default = FALSE.}
#'  \item{add.CI}{if FALSE, confidence intervals are removed from the plot. Default = TRUE.}
#'  \item{xlim}{xlim of the plot.}
#'  \item{ylim}{ylim of the plot.}  
#' }
#' 
#' 
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- createAlgo()
#' 
#' # run RMixtComp in unsupervised clustering mode + data as matrix
#' res <- mixtCompLearn(simData$dataLearn$matrix, simData$model$unsupervised, algo, nClass = 2:4)
#' # plot
#' plotDataCI(res, "Gaussian1")
#' plotDataCI(res$res[[2]], "Poisson1")
#' } 
#' 
#' @author Matthieu MARBAC
#' @family plot
#' @export
plotDataCI <- function(output, var, class = 1:output$algo$nClass, grl = FALSE, pkg = c("ggplot2", "plotly"), ...)
{
  pkg = match.arg(pkg)
  
  if(!(var%in%names(output$variable$type)))
    stop("This variable does not exist in the mixture model.")
  
  type <- output$variable$type[[var]]
  
  if(is.null(output$algo$dictionary$z_class))
    labelClass <- paste0("Class ", 1:output$algo$nClass)
  else
    labelClass <- output$algo$dictionary$z_class$old
  
  switch(type,
         "Gaussian" = plotCINumericData(extractCIGaussianVble(var, output, class, grl), var, labelClass, grl, pkg),
         "Weibull" = plotCINumericData(extractCIWeibullVble(var, output, class, grl), var, labelClass, grl, pkg),
         "Multinomial" = plotCategoricalData(extractCIMultiVble(var, output, class, grl), var, labelClass, grl, pkg),
         "Poisson" = plotCINumericData(extractCIPoissonVble(var, output, class, grl), var, labelClass, grl, pkg),
         "NegativeBinomial" = plotCINumericData(extractCINegBinomialVble(var, output, class, grl), var, labelClass, grl, pkg),
         "Func_CS" = plotFunctionalData(output, var, classToPlot = class, pkg = pkg, ...),
         "Func_SharedAlpha_CS" = plotFunctionalData(output, var, classToPlot = class, pkg = pkg, ...),
         warning(paste0("Not (yet) available for model ", type)))
}


# Mean and 95% confidence level per class for a numeric variable (Gaussian or Poisson)
# @author Matthieu Marbac
plotCINumericData <- function(data, var, class, grl, pkg = c("ggplot2", "plotly"), ...){
  pkg = match.arg(pkg)
  
  p <- switch(pkg, 
              "ggplot2" = ggplotCINumericData(data, var, class, grl),
              "plotly" = plotlyCINumericData(data, var, class, grl, ...))
  
  p
}

# @author Matthieu Marbac
plotlyCINumericData <- function(data, var, class, grl, ...){
  text1 <- paste0("Class.", class, "<br>",
                  "Mean: ", round(data$mean[1:length(class)],2), "<br>",
                  "CI-95%: [", round(data$lower[1:length(class)],2),
                  ",", round(data$uppers[1:length(class)],2),"]")
  if (grl) text1 <- c(text1, paste0("Grl <br>",
                                    "Mean: ", round(data$mean[length(data$mean)],2), "<br>",
                                    "CI-95%: [", round(data$lower[length(data$mean)],2),
                                    ",", round(data$uppers[length(data$mean)],2),"]"))
  p <- plot_ly(x = data$mean, 
               y = c(1:length(data$mean)),
               type = "scatter",
               mode = "markers",
               showlegend = FALSE,
               hoverinfo = "text",
               text = text1, ...
  )%>%layout(title = "Mean and 95%-level confidence intervals per class",
             paper_bgcolor = "rgb(255,255,255)", plot_bgcolor = "rgb(229,229,229)",
             xaxis = list(title = var,
                          titlefont = list(
                            family = "Arial, sans-serif",
                            size = 18
                          ),
                          gridcolor = "rgb(255,255,255)",
                          showgrid = TRUE,
                          showline = FALSE,
                          showticklabels = TRUE,
                          tickcolor = "rgb(127,127,127)",
                          ticks = "outside",
                          zeroline = FALSE,
                          range=c(min(data$lower), max(data$uppers)) + c(-1,1) * abs(max(data$uppers)-min(data$lower))/10),
             yaxis = list(title = "Class",
                          gridcolor = "rgb(255,255,255)",
                          showgrid = FALSE,
                          showline = FALSE,
                          showticklabels = FALSE,
                          tickcolor = "rgb(127,127,127)", ticks = "",
                          zeroline = FALSE))
  for(i in 1:length(class)){
    p <- add_trace(p,
                   x = c(data$lower[i], data$uppers[i]),  # x0, x1
                   y = c(i, i),  # y0, y1
                   mode = "lines",
                   line = list(color = "rgba(0,100,80,0.4)", width = 20),
                   showlegend = FALSE,
                   hoverinfo = "text",
                   text = paste("Class.", i, "<br>",
                                "Mean: ", round(data$mean[i],2), "<br>",
                                "CI-95%: [", round(data$lower[i],2),
                                ",", round(data$uppers[i],2),"]", sep = "")
    )
  }
  if (grl){
    i <- length(data$lower)
    p <- add_trace(p,
                   x = c(data$lower[i], data$uppers[i]),  # x0, x1
                   y = c(i, i),  # y0, y1
                   mode = "lines",
                   line = list(color = "rgba(100, 10,80,0.4)",width = 20),
                   showlegend = FALSE,
                   hoverinfo = "text",
                   text = paste("Class.", i, "<br>",
                                "Mean: ", round(data$mean[i],2), "<br>",
                                "CI-95%: [", round(data$lower[i],2),
                                ",", round(data$uppers[i],2),"]",sep="")
    )
  }
  
  p
}

# @author Quentin Grimonprez
ggplotCINumericData <- function(data, var, class, grl)
{
  labelClass <- class
  if(grl)
    labelClass = c(labelClass, "all")
  labelClass = factor(labelClass, levels = labelClass)
  
  df = data.frame(class = labelClass, classlo = seq_along(data$mean) - 0.1, classup =  seq_along(data$mean) + 0.1, mean = data$mean, lower = data$lower, uppers = data$uppers)
  p <- ggplot(df, aes_string(x = "mean", y = "class")) + 
    geom_point() +
    geom_rect(data = df, mapping = aes_string(xmin = "lower", xmax = "uppers", ymin = "classlo", ymax = "classup", fill = "class"), color = "black", alpha = 0.5) + 
    labs(title = "Mean and 95%-level confidence intervals per class", x = var, y = "Class") +
    theme(legend.position = "none")
  
  p
}

# @author Matthieu Marbac
plotCategoricalData <- function(data, var, class, grl, pkg = c("ggplot2", "plotly"), ...)
{
  pkg = match.arg(pkg)
  
  p <- switch(pkg, 
              "ggplot2" = ggplotCategoricalData(data, var, class, grl),
              "plotly" = plotlyCategoricalData(data, var, class, grl, ...))
  
  p
}

# Barplot for categorical data (only the levels included in the 95 confidence level for at least one component are plotted)
# @author Matthieu Marbac
plotlyCategoricalData <- function(data, var, class, grl, ...){
  formattedW <- lapply(1:length(class), 
                       function(k) list(y = data$probs[k,],
                                        type = "bar",
                                        hoverinfo = "text",
                                        text = paste0("class ", class[k] ,"\n", paste(data$levels, data$probs[k, ], sep = ": ", collapse = "\n")),
                                        showlegend = FALSE,
                                        marker = list(line = list(color = "black", width = 1.5)))
  )
  if (grl)
    formattedW <- c(formattedW,
                    list(list(y = data$probs[nrow(data$probs),],
                              type = "bar",
                              hoverinfo = "text",
                              text = "grl",
                              showlegend = FALSE,
                              marker = list(line = list(color = "red", width = 1.5)))))
  # Reduce the list of plotly compliant objs, starting with the plot_ly() value and adding the `add_trace` at the following iterations
  p <- Reduce(function(acc, curr)  do.call(add_trace, c(list(p = acc), curr)),
              formattedW,
              init = plot_ly(x = data$levels, ...)%>%
                layout(title = "Distribution per class",
                       paper_bgcolor = "rgb(255,255,255)", plot_bgcolor = "rgb(229,229,229)",
                       xaxis = list(title = var,
                                    gridcolor = "rgb(255,255,255)",
                                    showgrid = TRUE,
                                    showline = FALSE,
                                    showticklabels = TRUE,
                                    tickcolor = "rgb(127,127,127)",
                                    ticks = "outside",
                                    tickvals = data$levels,
                                    zeroline = FALSE),
                       yaxis = list(title = "Probability",
                                    gridcolor = "rgb(255,255,255)",
                                    showgrid = TRUE,
                                    showline = FALSE,
                                    showticklabels = TRUE,
                                    tickcolor = "rgb(127,127,127)",
                                    ticks = "outside",
                                    zeroline = FALSE))
  )
  p
}


# @author Quentin Grimonprez
ggplotCategoricalData <- function(data, var, class, grl)
{
  labelClass <- class
  if(grl)
    labelClass = c(labelClass, "all")
  labelClass = factor(labelClass, levels = labelClass)
  
  df = data.frame(value = as.numeric(t(data$probs)), categ = rep(data$levels, nrow(data$probs)), class = rep(labelClass, each = length(data$levels)))
  p <- ggplot(data = df, aes_string(x = "categ", y = "value", fill = "class")) +
    geom_bar(stat = "identity", position = position_dodge()) +
    labs(title = "Distribution per class", x = var, y = "Probability") 
  
  p
}


# Mean and 95% confidence level confidence  for functional data
# @author Matthieu Marbac
plotFunctionalData <- function(output, var, add.obs = FALSE, ylim = NULL, xlim = NULL, add.CI = TRUE, classToPlot = NULL, pkg = c("ggplot2", "plotly"), ...)
{
  pkg = match.arg(pkg)
  
  # Computation of the Confidence Intervals (CI)
  data <- extractCIFunctionnalVble(var, output)
  
  p <- switch(pkg,
              "ggplot2" = ggplotFunctionalData(data, output, var, add.obs, ylim, xlim, add.CI, classToPlot),
              "plotly" = plotlyFunctionalData(data, output, var, add.obs, ylim, xlim, add.CI, classToPlot, ...))
  
  p
}

# @author Matthieu Marbac
plotlyFunctionalData <- function(data, output, var, add.obs = FALSE, ylim = NULL, xlim = NULL, add.CI = TRUE, classToPlot = NULL, ...){
  G <- output$algo$nClass
  
  if(is.null(classToPlot))
    classToPlot = 1:G
  
  
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
    partition <- output$variable$data$z_class$completed
    
    for(i in 1:length(output$variable$data[[var]]$time)){
      if(partition[i]%in%classToPlot)
      {
        formattedW <- c(formattedW,  list(
          list(x = output$variable$data[[var]]$time[[i]],
               y = output$variable$data[[var]]$data[[i]], 
               type = 'scatter', mode = 'lines', showlegend = FALSE, line = list(color = 'rgba(0,100,80,0.4)', width = 1)))
        )
      }
    }
    
  }
  # mean curves and CI are added
  for(k in classToPlot){
    if(add.CI)
    {
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
    }else{
      formattedW <- c(formattedW,  list(
        list(y = data$mean[k,], 
             type = 'scatter', mode = 'lines',
             line = list(color=k, width = 4),showlegend = FALSE, 
             name = paste("mean class",k,sep="."))
      ))
    }
    
  }
  # Reduce the list of plotly compliant objs, starting with the plot_ly() value and adding the `add_trace` at the following iterations
  p <- Reduce(function(acc, curr)  do.call(add_trace, c(list(p=acc),curr)),
              formattedW,
              init=plot_ly(x = data$time, ...)%>%
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
# @author Quentin Grimonprez
ggplotFunctionalData <- function(data, output, var, add.obs = FALSE, ylim = NULL, xlim = NULL, add.CI = TRUE, classToPlot = NULL)
{
  if(!is.null(classToPlot))
  {
    data$mean = data$mean[classToPlot,, drop = FALSE] 
    data$inf = data$inf[classToPlot,, drop = FALSE] 
    data$sup = data$sup[classToPlot,, drop = FALSE] 
  }
  
  df = as.data.frame(t(data$mean))
  df$time = data$time

  p <- ggplot() 
  
  # add observation
  if(add.obs)
  {
    for(i in seq_along(output$variable$data[[var]]$time))
    {
      df2 = data.frame(time = output$variable$data[[var]]$time[[i]], value = output$variable$data[[var]]$data[[i]])
      p = p + geom_line(aes_string(y = "value", x = "time"), data = df2, stat = "identity", size = 0.5, alpha = 0.5)
    }
  }
  

  # confidence intervals
  if(add.CI)
  {
    for(i in 1:nrow(data$inf))
    {
      df2 = data.frame(inf = data$inf[i,], sup = data$sup[i,], time = data$time)
      p = p + geom_ribbon(data = df2, aes_string(x = "time",  ymin = "inf", ymax = "sup"), alpha = 0.3)
    }
    
  }
  
  # mean class
  for(i in 1:(ncol(df)-1))
    p = p + geom_line(aes_string(y = paste0("V",i), x = "time", col = factor(paste("Class", i), levels = paste("Class", 1:(ncol(df)-1)))), data = df, stat = "identity", size = 1.5)
  
  
  p = p  +
    labs(title = "Mean curves and 95%-level confidence intervals per class", x = "Time", y = var) +
    scale_color_discrete(name = "Class") 
  
  if(!is.null(xlim))
    p = p + layout(axis = list(range = xlim))
  if(!is.null(ylim))
    p = p + layout(yaxis = list(range = ylim))
  
  p
}
  