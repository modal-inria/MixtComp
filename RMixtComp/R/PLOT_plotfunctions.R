#' Barplot of the discriminative power of the variables
#' 
#' @details The discriminative power of variable j is defined by 1 - Cj
#' Cj=  - sum_{k=1}^K sum_{i=1}^n P(Z_i=k|x_{ij}) ln(P(Z_i=k|x_{ij})) / (n*lnK)
#' 
#' @param output object returned by function \emph{mixtCompCluster}
#' @param ylim vector of length 2 defining the range of y-axis
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... arguments to be passed to plot_ly
#' 
#' @examples 
#' \donttest{
#' 
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
#' plotDiscrimVbles(res)
#' plotDiscrimVbles(res$res[[2]])
#' }
#' 
#' @seealso \link{computeDiscrimPowerVar}
#' 
#' @author Matthieu MARBAC
#' @family plot
#' @export
plotDiscrimVbles <- function(output, ylim = c(0, 1), pkg = c("ggplot2", "plotly"), ...){
  pkg = match.arg(pkg)
  
  ## Get information
  # names of variables
  namesVbles <- names(output$variable$type)[names(output$variable$type) != "z_class"]
  namesShort <- abbreviate(namesVbles, 6, use.classes = FALSE)
  # discriminative power (1 - Cj), saved at slot pvdiscrimclasses of JSON file
  pvDiscrim <- round(1-colSums(output$mixture$IDClass), 2)
  # Full names and type of the variables
  textMous <- paste(namesVbles, '<br>', as.character(output$variable$type[-1]))
  
  ## Variables are sorted by descreasing order of their discriminative power
  ## Character must be convert in factor (otherwise variables are plotted with alphabetic order)
  orderVbles <- order(pvDiscrim, decreasing = TRUE)
  pvDiscrim  <- pvDiscrim[orderVbles]
  namesVbles <- factor(namesVbles[orderVbles], levels=namesVbles[orderVbles])
  namesShort <- factor(namesShort[orderVbles], levels=namesShort[orderVbles])
  textMous   <- factor(textMous[orderVbles], levels=textMous[orderVbles])
  
  ## Barplot
  visuVbles <- switch(pkg,
                      "plotly" = barplotly(pvDiscrim, namesShort, main = "Discriminative level of the variables", ylim = ylim, text = textMous, ...),
                      "ggplot2" = ggbarplot(pvDiscrim, namesShort, main = "Discriminative level of the variables", ylim = ylim)) 
  visuVbles
}

#' Barplot of the discriminative power of the classes
#' 
#' @details The discriminative power of class k is defined by 1 - Dk
#' Dk =  - sum_{i=1}^n P(Z_i=k|x_i) ln(P(Z_i=k|x_i)) / (n*exp(-1))
#' 
#' @param output object returned by function \emph{mixtCompCluster}
#' @param ylim vector of length 2 defining the range of y-axis
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
#' plotDiscrimClass(res)
#' plotDiscrimClass(res$res[[2]])
#' }
#' 
#' @seealso \link{computeDiscrimPowerClass}
#' 
#' @author Matthieu MARBAC
#' @family plot
#' @export
plotDiscrimClass <- function(output, ylim = c(0, 1), pkg = c("ggplot2", "plotly"), ...){
  pkg = match.arg(pkg)
  
  ## Get information
  # names of classes
  namesClass <- paste("Class", 1:output$algo$nClass)
  # discriminative power (1 - Dk), saved at slot pvdiscrimvbles of JSON file
  pvDiscrim <-   round(1 - (-colMeans(log(output$variable$data$z_class$stat**output$variable$data$z_class$stat)) / exp(-1)), 2)

  ## Classes are sorted by descreasing order of their discriminative power
  ## Character must be convert in factor (otherwise alphabetic order is considered)
  orderClass <- order(pvDiscrim, decreasing = TRUE)
  namesClass <- factor(namesClass[orderClass], levels=namesClass[orderClass])
  pvDiscrim <- pvDiscrim[orderClass]
  ## Barplot
  visuClass <- switch(pkg,
                      "plotly" = barplotly(pvDiscrim, namesClass, main = "Discriminative level of the classes", ylim = ylim, ...),
                      "ggplot2" = ggbarplot(pvDiscrim, namesClass, main = "Discriminative level of the classes", ylim = ylim))
  
  visuClass
}


barplotly <- function(value, label, main, xlab = "", ylab = "", ylim = c(0, 1), text = NULL, ...)
{
  p <- plot_ly(x = label,
               y = value,
               type = "bar",
               text = text,
               marker = list(color = col_numeric("Blues", domain = c(0,100))(ceiling(value*100)),
                             line = list(color = "black", width = 1.5), ...)
  ) %>%
    layout(title = main,
           xaxis = list(title = xlab),
           yaxis = list(title = ylab, range = ylim),
           annotations = list(x = label,
                              y = value,
                              text = round(value, 2),
                              xanchor = "center",
                              yanchor = "bottom",
                              showarrow = FALSE)
    )
  p
}

ggbarplot <- function(value, label, main, xlab = "", ylab = "", ylim = c(0, 1))
{
  df = data.frame(var = factor(label, levels = label), discrim = value)
  ggplot(data = df, aes(x = var, y = discrim, fill = var)) +
    geom_bar(stat = "identity")+
    scale_fill_manual(values=col_numeric("Blues", domain = c(0,100))(ceiling(value*100))) + 
    geom_text(aes(label = round(discrim, 2)), vjust = 1.6, color = "white", size = 5) +
    theme_minimal() +
    ylim(ylim[1], ylim[2]) +
    labs(title = main, x = xlab, y = ylab) +
    theme(legend.position = "none",
          axis.text.x = element_text(angle = 315, hjust = 0))
}





#' Heatmap of the similarities between variables about clustering
#' 
#' @details The similarities between variables j and h is defined by 1 - Delta(j,h)
#' Delta(j,h)^2 = (1/n) * sum_{i=1}^n sum_{k=1}^K (P(Z_i=k|x_{ij}) - P(Z_i=k|x_{ih}))^2
#' 
#' @param output object returned by function \emph{mixtCompCluster}
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
#' heatmapVbles(res)
#' heatmapVbles(res$res[[2]])
#' }
#' 
#' @seealso \link{computeSimilarityVar}
#' 
#' @author Matthieu MARBAC
#' @family plot
#' @export
heatmapVbles <- function(output, ...){
  ## Get information
  # names of variables
  namesVbles <- names(output$variable$type)[names(output$variable$type) != "z_class"]
  namesShort <- abbreviate(namesVbles, 6, use.classes = FALSE)
  # similarities  (1 - delta), delta is saved at slot delta of JSON file
  similarities <- round(1 - output$mixture$delta, 2)
  
  # discriminative power (1 - Cj), saved at slot pvdiscrimclasses of JSON file
  pvDiscrim <- round(1-colSums(output$mixture$IDClass), 2)
  
  ## Variables are sorted by descreasing order of their discriminative power
  ## Character must be convert in factor (otherwise alphabetic order is considered)
  orderVbles <- order(pvDiscrim, decreasing = TRUE)
  namesVbles <- factor(namesVbles[orderVbles], levels=namesVbles[orderVbles])
  if (length(namesVbles)>1){
    similarities <- similarities[,orderVbles]
    similarities <- similarities[orderVbles,]
  }else{
    similarities <- matrix(1, 1, 1)
  }
  
  # Text to display
  textMous <- sapply(1:length(namesVbles),
                     function(j) paste("Similarity beween<br>", 
                                       namesVbles[j], 
                                       " and ", 
                                       namesVbles, 
                                       ": <br>",
                                       similarities[,j],
                                       sep="")
  )
  
  namesShort <- factor(namesShort[orderVbles], levels=namesShort[orderVbles])
  ## heatmap
  heatmap <- plot_ly(text=textMous,
                     hoverinfo='text',
                     z = as.table(similarities),
                     x=namesShort,
                     y=namesShort,
                     colorscale = cbind(0:1,(col_numeric("Blues", domain = c(0,100))(range(similarities*100)))),
                     type = "heatmap",
                     showscale = FALSE, ...) %>%
    layout(title = "Similarities between variables", showlegend = FALSE, yaxis=list(ticks=""), xaxis=list(ticks=""))
  heatmap
}

#' Heatmap of the similarities between classes about clustering
#'
#' @details The similarities between classes k and g is defined by 1 - Sigma(k,g)
#' Sigma(k,g)^2 = (1/n) * sum_{i=1}^n (P(Z_i=k|x_i) - P(Z_i=g|x_i))^2
#' 
#' @param output object returned by function \emph{mixtCompCluster}
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
#' # plot
#' heatmapClass(res)
#' heatmapClass(res$res[[2]])
#' } 
#' 
#' @seealso \link{computeSimilarityClass}
#'
#' @author Matthieu MARBAC
#' @family plot
#' @export
heatmapClass <- function(output, ...){
  ## Get information
  # names of variables  
  namesClass <- paste("class", 1:output$algo$nClass, sep=".")
  # discriminative power (1 - Dk), saved at slot pvdiscrimvbles of JSON file
  pvDiscrim <-   round(1 - (-colMeans(log(output$variable$data$z_class$stat**output$variable$data$z_class$stat)) / exp(-1)), 2)
  # similarities  (1 - sigma), sigma is saved at slot sigma of JSON file
  similarities <- round(1-sqrt(sapply(1:output$algo$nClass,
                                      function(k) colMeans(sweep(output$variable$data$z_class$stat,
                                                                 1,
                                                                 output$variable$data$z_class$stat[,k],
                                                                 "-")**2)
  )), 4)
  
  ## Classes are sorted by descreasing order of their discriminative power
  ## Character must be convert in factor (otherwise alphabetic order is considered)
  orderClass <- order(pvDiscrim, decreasing = TRUE)
  namesClass <- factor(namesClass[orderClass], levels=namesClass[orderClass])
  if (output$algo$nClass>1){
    similarities <- similarities[,orderClass]
    similarities <- similarities[orderClass, ]
  }else{
    similarities <- matrix(1, 1, 1)
  }
  
  # Text to display
  textMous <- sapply(1:output$algo$nClass, 
                     function(k) paste("Similarity beween<br>",
                                       namesClass[k],
                                       " and ",
                                       namesClass,
                                       ": <br>",
                                       similarities[,k],
                                       sep=""))
  
  ## heatmap
  heatmap <- plot_ly(text=textMous,
                     hoverinfo='text',
                     z = as.table(similarities),
                     x=namesClass,
                     y=namesClass,
                     colorscale = cbind(0:1,(col_numeric("Blues", domain = c(0,100))(range(similarities*100)))),
                     type = "heatmap",
                     showscale = FALSE, ...) %>%
    layout(title = "Similarities between classes", showlegend = FALSE, yaxis=list(ticks=""), xaxis=list(ticks=""))
  heatmap
}

#' Heatmap of the tik = P(Z_i=k|x_i)
#'  
#' @details Observation are sorted according to the hard partition then for each component
#' they are sorted by decreasing order of their tik's
#' 
#' @param output object returned by function \emph{mixtCompCluster}
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
#' heatmapTikSorted(res)
#' heatmapTikSorted(res$res[[2]])
#' }
#' 
#' @seealso getTik
#' 
#' @author Matthieu MARBAC
#' @family plot
#' @export
heatmapTikSorted <- function(output, ...){
  # orderTik, they are saved at slot ordertik of JSON file
  orderTik <- unlist(sapply(1:output$algo$nClass, 
                            function(k) order(output$variable$data$z_class$stat[,k] * (output$variable$data$z_class$completed == k ),
                                              decreasing = T)[1:(table(output$variable$data$z_class$completed)[k])]
  ))
  
  tiksorted <- output$variable$data$z_class$stat[orderTik,]
  if(output$algo$nClass ==1 ) 
    tiksorted <- matrix(tiksorted, ncol=1)
  
  # Text to display
  textMous <- sapply(1:output$algo$nClass, 
                     function(k) paste("Probability that <br> observation", 
                                       orderTik, 
                                       " <br>belongs to class",
                                       k, 
                                       ": <br>",
                                       round(tiksorted[,k],4)))
  
  
  orderTik <- factor(as.character(orderTik), levels=as.character(orderTik))
  
  ## heatmap
  tuneyaxis <- list(
    title = "",
    zeroline = FALSE,
    showline = FALSE,
    showticklabels = FALSE,
    showgrid = FALSE, 
    ticks=""
  )
  heatmap <- plot_ly(text=textMous,
                     hoverinfo='text',
                     z = as.table(tiksorted),
                     x = paste("class", 1:output$algo$nClass, sep="."),
                     colorscale = cbind(0:1, (col_numeric("Blues", domain = c(0,100))(range(output$variable$data$z_class$stat*100)))),
                     type = "heatmap",
                     showscale = FALSE, ...) %>%
    layout(title = "Probabilities of classification", showlegend = FALSE, yaxis=tuneyaxis, xaxis=list(ticks=""))
  heatmap
}  

#' Histogram of the misclassification probabilities 
#'
#' @details Missclassification probability of observation i is denoted err_i
#' err_i = 1 - max_{k={1,\ldots,K}} P(Z_i=k|x_i)
#' Histgrams of err_i's can be plot for a specific class, all classes or every class
#'
#' @param output object returned by function \emph{mixtCompCluster}
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
histMisclassif <- function(output, ...){
  ## Get information
  z <- output$variable$data$z_class$completed
  misclassifrisk <- 1 - apply(output$variable$data$z_class$stat, 1, max)
  G <- output$algo$nClass
  
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
                       init=plot_ly(...)%>%layout(
                         showlegend = T,
                         title = "Misclassification risk",
                         xaxis = list(domain = c(-0.09, 0.9), title="Probabilities of misclassification"),
                         yaxis = list(title = "Percentile of observations with <br> a misclassification risk less than x"),
                         updatemenus = list(list(y=0.6, x=-0.15,buttons = listbuttons)))
  )
  histerrors
}
