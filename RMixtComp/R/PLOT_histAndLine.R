
#' Histogram of the misclassification probabilities 
#'
#' @details Missclassification probability of observation i is denoted err_i
#' err_i = 1 - max_{k={1,\ldots,K}} P(Z_i=k|x_i)
#' Histgrams of err_i's can be plot for a specific class, all classes or every class
#'
#' @param output object returned by function \emph{mixtCompLearn}
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... arguments to be passed to plot_ly
#' 
#' @examples 
#' \donttest{
#' library(RMixtComp)
#' 
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- createAlgo()
#' 
#' # keep only 3 variables
#' desc <- simDesc$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")]
#' 
#' # run RMixtCompt in unsupervised clustering mode + data as matrix
#' res <- mixtCompLearn(simDataLearn$matrix, desc, algo, nClass = 2:4)
#' 
#' # plot
#' histMisclassif(res)
#' histMisclassif(res$res[[2]])
#' }
#' 
#' @author Matthieu MARBAC
#' @family plot
#' @export
histMisclassif <- function(output, pkg = c("ggplot2", "plotly"), ...)
{
  pkg = match.arg(pkg)
  
  ## Get information
  z <- output$variable$data$z_class$completed
  misclassifrisk <- 1 - apply(output$variable$data$z_class$stat, 1, max)
  G <- output$algo$nClass
  
  p <- switch(pkg,
              "ggplot2" = gghistMisclassif(z, misclassifrisk, G),
              "plotly" = plotlyhistMisclassif(z, misclassifrisk, G, ...))
  
  p
}


plotlyhistMisclassif <- function(z, misclassifrisk, G, ...){
  
  ## Create the buttons for selecting some data
  # General: all data
  # Each: all data by components
  # class.k: only data of component k
  listbuttons <- c(list(list(method = "restyle",
                             args = list("visible", as.list(c(0:G)==0)),
                             label = "General"),
                        list(method = "restyle", 
                             args = list("visible", as.list(c(0:G)>0)),
                             label = "Each")),
                   lapply(1:G, function(k) list(method = "restyle",
                                                args = list("visible", as.list(c(0:G)==k)),
                                                label = paste("class", k, sep="."))))
  
  # a list with plotly compliant formatted objects
  formattedW <- c(list(
    list(x = misclassifrisk,  name= "General", visible=TRUE, nbinsx=20)), 
    lapply(1:G, function(k) 
      list(x = misclassifrisk[which(z==k)], name= paste("class", k, sep="."), visible=FALSE)
    ))
  
  # Reduce the list of plotly compliant objs, starting with the plot_ly() value and adding the `add_trace` at the following iterations
  histerrors <- Reduce(function(acc, curr)  do.call(add_histogram, c(list(p=acc),curr)),
                       formattedW,
                       init=plot_ly()%>%layout(
                         showlegend = T,
                         title = "Misclassification risk",
                         xaxis = list(domain = c(-0.09, 0.9), title="Probabilities of misclassification"),
                         yaxis = list(title = "Percentile of observations with <br> a misclassification risk less than x"),
                         updatemenus = list(list(y=0.6, x=-0.15,buttons = listbuttons)))
  )
  
  histerrors
}


gghistMisclassif <- function(z, misclassifrisk, G)
{
  df = data.frame(class = factor(z, levels = 1:G), misclassifrisk = misclassifrisk)
  
  
  p <- list()
  
  # general
  p[[1]] = ggplot(df, aes(x = misclassifrisk, y = ..count../sum(..count..))) + 
    geom_histogram(position = "identity", binwidth = 0.05, colour = "black", alpha = 0.8) +
    labs(title = "Misclassification risk", x = "Probabilities of misclassification", y = "Percentile of observations with \na misclassification risk less than x")
  
  # each
  p[[2]] = ggplot(df, aes(x = misclassifrisk, y = ..count../sum(..count..), fill = class)) + 
    geom_histogram(position = "dodge", binwidth = 0.05) +
    labs(title = "Misclassification risk", x = "Probabilities of misclassification", y = "Percentile of observations with \na misclassification risk less than x")
  
  # class by class
  for(i in 1:G)
  {
    p[[2+i]] = ggplot(subset(df, class == i), aes(x = misclassifrisk, y = ..count../sum(..count..)), fill = class) + 
      geom_histogram(position = "dodge", binwidth = 0.05, fill = hue_pal()(G)[i], colour = "black") + 
      labs(title = paste0("Misclassification risk: class ", i), x = "Probabilities of misclassification", y = "Percentile of observations with \na misclassification risk less than x")
  }
  
  return(p)
}


#' Plot BIC and ICL 
#'
#' @param output \emph{MixtCompLearn} object
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... arguments to be passed to plot_ly
#' 
#' @examples 
#' \donttest{
#' library(RMixtComp)
#' 
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- createAlgo()
#' 
#' # keep only 3 variables
#' desc <- simDesc$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")]
#' 
#' # run RMixtCompt in unsupervised clustering mode + data as matrix
#' res <- mixtCompLearn(simDataLearn$matrix, desc, algo, nClass = 2:4)
#' 
#' # plot
#' plotCrit(res)
#' }
#' 
#' @family plot
#' @export
plotCrit <- function(output, pkg = c("ggplot2", "plotly"), ...)
{
  pkg = match.arg(pkg)
  
  p <- switch(pkg, 
              "ggplot2" = ggplotCrit(output$crit, output$nClass),
              "plotly" = plotlyCrit(output$crit, output$nClass, ...))
  
  p
}

plotlyCrit <- function(crit, nClass, ...)
{
  p <- plot_ly(x = nClass, y = crit[1,], type = "scatter", mode = "lines", name = "BIC", ...) %>%
    add_trace(x = nClass, y = crit[2,], name = "ICL")%>%
    layout(title = "Criterion", showlegend = TRUE, xaxis = list(title = "Number of classes"), yaxis = list(title = "Value"))
  
  p
}


ggplotCrit <- function(crit, nClass)
{
  df = data.frame(class = nClass, value = c(crit[1,], crit[2,]), crit = rep(c("BIC", "ICL"), each = length(nClass)))
  
  p <- ggplot(data = df, aes_string(x = "class", y = "value", col = "crit")) +
    geom_line() + 
      labs(title = "Criterion", x = "Number of classes", y = "value") +
      scale_color_discrete(name = "Criterion") 
  
  p
}
