#' Barplot of the discriminative power of the variables
#' 
#' @details The discriminative power of variable j is defined by 1 - C(j)
#' \deqn{C(j)=  -\sum_{k=1}^K \sum_{i=1}^n P(Z_i=k|x_{ij}) ln(P(Z_i=k|x_{ij})) / (n*\log(K))}
#' 
#' @param output object returned by function \emph{mixtCompLearn}
#' @param ylim vector of length 2 defining the range of y-axis
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... arguments to be passed to plot_ly
#' 
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- createAlgo()
#' 
#' # keep only 3 variables
#' model <- simData$model$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")]
#' 
#' # run RMixtCompt in unsupervised clustering mode + data as matrix
#' res <- mixtCompLearn(simData$dataLearn$matrix, model, algo, nClass = 2:4)
#' 
#' # plot
#' plotDiscrimVar(res)
#' plotDiscrimVar(res$res[[2]])
#' }
#' 
#' @seealso \code{\link{computeDiscrimPowerVar}}
#' 
#' @author Matthieu MARBAC
#' @family plot
#' @export
plotDiscrimVar <- function(output, ylim = c(0, 1), pkg = c("ggplot2", "plotly"), ...){
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
                      "ggplot2" = ggbarplot(pvDiscrim, namesShort, main = "Discriminative level of the variables", ylim = ylim, col.text = "black")) 
  visuVbles
}

#' Barplot of the discriminative power of the classes
#' 
#' @details The discriminative power of class k is defined by 1 - D(k)
#' \deqn{D(k) =  -\sum_{i=1}^n P(Z_i=k|x_i) \log(P(Z_i=k|x_i)) / (n*\exp(-1))}
#' 
#' @param output object returned by function \emph{mixtCompLearn}
#' @param ylim vector of length 2 defining the range of y-axis
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... arguments to be passed to plot_ly
#' 
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- createAlgo()
#' 
#' # keep only 3 variables
#' model <- simData$model$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")]
#' 
#' # run RMixtCompt in unsupervised clustering mode + data as matrix
#' res <- mixtCompLearn(simData$dataLearn$matrix, model, algo, nClass = 2:4)
#' 
#' # plot
#' plotDiscrimClass(res)
#' plotDiscrimClass(res$res[[2]])
#' }
#' 
#' @seealso \code{\link{computeDiscrimPowerClass}}
#' 
#' @author Matthieu MARBAC
#' @family plot
#' @export
plotDiscrimClass <- function(output, ylim = c(0, 1), pkg = c("ggplot2", "plotly"), ...){
  pkg = match.arg(pkg)
  
  ## Get information
  # names of classes
  if(is.null(output$algo$dictionary$z_class))
    namesClass <- paste0("Class ", 1:output$algo$nClass)
  else
    namesClass <- output$algo$dictionary$z_class$old
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
                      "ggplot2" = ggbarplot(pvDiscrim, namesClass, main = "Discriminative level of the classes", ylim = ylim, col.text = "black"))
  
  visuClass
}


#' Plot the mixture's proportions 
#'
#' @param output object returned by function \emph{mixtCompLearn}
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... arguments to be passed to plot_ly
#' 
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- createAlgo()
#' 
#' # keep only 3 variables
#' model <- simData$model$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")]
#' 
#' # run RMixtCompt in unsupervised clustering mode + data as matrix
#' res <- mixtCompLearn(simData$dataLearn$matrix, model, algo, nClass = 2:4)
#' 
#' # plot
#' plotProportion(res)
#' plotProportion(res$res[[2]])
#' }
#' 
#' @family plot
#' @author Quentin Grimonprez
#' @export
plotProportion <- function(output, pkg = c("ggplot2", "plotly"), ...)
{
  pkg = match.arg(pkg)
  
  ylimProportion <- min(max(output$variable$param$z_class$stat[,1]) + 0.1, 1)
  if(is.null(output$algo$dictionary$z_class))
    label <- paste0("Class ", 1:output$algo$nClass)
  else
    label <- output$algo$dictionary$z_class$old
    
  
  p <- switch(pkg, 
              "ggplot2" = ggbarplot(output$variable$param$z_class$stat[,1], label, main = "Proportion", xlab = "", ylab = "", ylim = c(0, ylimProportion), col.text = "black"),
              "plotly" = barplotly(output$variable$param$z_class$stat[,1], label, main = "Proportion", xlab = "", ylab = "", ylim = c(0, ylimProportion), text = NULL, ...))
  
  p
}

# @author Quentin Grimonprez
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

# @author Matthieu Marbac
ggbarplot <- function(value, label, main, xlab = "", ylab = "", ylim = c(0, 1), col.text = "white")
{
  df = data.frame(var = factor(label, levels = label), discrim = value)
  ggplot(data = df, aes_string(x = "var", y = "discrim", fill = "var")) +
    geom_bar(stat = "identity") +
    scale_fill_manual(values = col_numeric("Blues", domain = c(0,100))(ceiling(value*100))) + 
    geom_text(aes(label = round(discrim, 2)), vjust = 1.6, color = col.text, size = 5) +
    theme_minimal() +
    ylim(ylim[1], ylim[2]) +
    labs(title = main, x = xlab, y = ylab) +
    theme(legend.position = "none",
          axis.text.x = element_text(angle = 315, hjust = 0))
}
