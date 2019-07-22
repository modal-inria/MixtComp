# MixtComp version 4 - july 2019
# Copyright (C) Inria - Université de Lille - CNRS 

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# 
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>
 

#' Heatmap of the similarities between variables about clustering
#' 
#' @details The similarities between variables j and h is defined by Delta(j,h)
#' \deqn{Delta(j,h) = 1 - \sqrt{(1/n) * \sum_{i=1}^n \sum_{k=1}^K (P(Z_i=k|x_{ij}) - P(Z_i=k|x_{ih}))^2}}
#' 
#' @param output object returned by function \emph{mixtCompLearn}
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... arguments to be passed to plot_ly. For pkg = "ggplot2", addValues = TRUE prints similarity values on the heatmap
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
#' # run RMixtComp in unsupervised clustering mode + data as matrix
#' res <- mixtCompLearn(simData$dataLearn$matrix, model, algo, nClass = 2:4)
#' 
#' # plot
#' heatmapVar(res)
#' heatmapVar(res$res[[2]])
#' }
#' 
#' @seealso \code{\link{computeSimilarityVar}}
#' 
#' @author Matthieu MARBAC
#' @family plot
#' @export
heatmapVar <- function(output, pkg = c("ggplot2", "plotly"), ...){
  pkg = match.arg(pkg)
  
  ## Get information
  # names of variables
  namesVbles <- names(output$variable$type)[names(output$variable$type) != "z_class"]
  namesShort <- abbreviate(namesVbles, 6, use.classes = FALSE)
  # similarities delta is saved at slot delta of JSON file
  similarities <- round(output$mixture$delta, 2)
  
  # discriminative power (1 - Cj), saved at slot pvdiscrimclasses of JSON file
  pvDiscrim <- round(1-colSums(output$mixture$IDClass), 2)
  
  ## Variables are sorted by decreasing order of their discriminative power
  ## Character must be convert in factor (otherwise alphabetic order is considered)
  orderVbles <- order(pvDiscrim, decreasing = TRUE)
  namesVbles <- factor(namesVbles[orderVbles], levels=namesVbles[orderVbles])
  if (length(namesVbles)>1){
    similarities <- similarities[,orderVbles]
    similarities <- similarities[orderVbles,]
  }else{
    similarities <- matrix(1, 1, 1)
  }
  
  text <- round(similarities, 2)
  for(i in 1:nrow(text))
  {
    for(j in 1:ncol(text))
    {
      text[i, j] = paste0("similarity between ", namesVbles[i], "\nand ", namesVbles[j], ": ", text[i,j])
    }
  }
  
  heatmap <- switch(pkg,
                    "plotly" = heatmapplotly(similarities, xname = namesShort, yname = namesShort, main = "Similarities between variables", text = text, ...),
                    "ggplot2" = ggheatmap(similarities, xname = namesShort, yname = namesShort, main = "Similarities between variables", legendName = "Similarities", ...))
  heatmap
}

#' Heatmap of the similarities between classes about clustering
#'
#' @details The similarities between classes k and g is defined by 1 - Sigma(k,g)
#' \deqn{Sigma(k,g)^2 = (1/n) * \sum_{i=1}^n (P(Z_i=k|x_i) - P(Z_i=g|x_i))^2}
#' 
#' @param output object returned by function \emph{mixtCompLearn}
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... arguments to be passed to plot_ly. For pkg = "ggplot2", addValues = TRUE prints similarity values on the heatmap
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
#' # run RMixtComp in unsupervised clustering mode + data as matrix
#' res <- mixtCompLearn(simData$dataLearn$matrix, model, algo, nClass = 2:4)
#' # plot
#' heatmapClass(res)
#' heatmapClass(res$res[[2]])
#' } 
#' 
#' @seealso \code{\link{computeSimilarityClass}}
#'
#' @author Matthieu MARBAC
#' @family plot
#' @export
heatmapClass <- function(output, pkg = c("ggplot2", "plotly"), ...){
  pkg = match.arg(pkg)
  
  ## Get information
  # names of variables  
  if(is.null(output$algo$dictionary$z_class))
    namesClass <- paste0("Class ", 1:output$algo$nClass)
  else
    namesClass <- output$algo$dictionary$z_class$old
  
  # discriminative power (1 - Dk), saved at slot pvdiscrimvbles of JSON file
  pvDiscrim <-   round(1 - (-colMeans(log(output$variable$data$z_class$stat**output$variable$data$z_class$stat)) / exp(-1)), 2)
  # similarities  (1 - sigma), sigma is saved at slot sigma of JSON file
  similarities <- round(1-sqrt(sapply(1:output$algo$nClass,
                                      function(k) colMeans(sweep(output$variable$data$z_class$stat,
                                                                 1,
                                                                 output$variable$data$z_class$stat[,k],
                                                                 "-")**2)
  )), 4)
  
  ## Classes are sorted by decreasing order of their discriminative power
  ## Character must be convert in factor (otherwise alphabetic order is considered)
  orderClass <- order(pvDiscrim, decreasing = TRUE)
  namesClass <- factor(namesClass[orderClass], levels = namesClass[orderClass])
  if (output$algo$nClass>1){
    similarities <- similarities[,orderClass]
    similarities <- similarities[orderClass, ]
  }else{
    similarities <- matrix(1, 1, 1)
  }
  
  text <- round(similarities, 2)
  for(i in 1:nrow(text))
  {
    for(j in 1:ncol(text))
    {
      text[i, j] = paste0("similarity between ", namesClass[i], "\nand ", namesClass[j], ": ", text[i,j])
    }
  }

  heatmap <- switch(pkg,
                    "plotly" = heatmapplotly(similarities, xname = namesClass, yname = namesClass, main = "Similarities between classes", text = text, ...),
                    "ggplot2" = ggheatmap(similarities, xname = namesClass, yname = namesClass, main = "Similarities between classes", legendName = "Similarities", ...))
  heatmap
}

#' Heatmap of the tik = P(Z_i=k|x_i)
#'  
#' @details Observation are sorted according to the hard partition then for each component
#' they are sorted by decreasing order of their tik's
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
#' # run RMixtComp in unsupervised clustering mode + data as matrix
#' res <- mixtCompLearn(simData$dataLearn$matrix, model, algo, nClass = 2:4)
#' 
#' # plot
#' heatmapTikSorted(res)
#' heatmapTikSorted(res$res[[2]])
#' }
#' 
#' @seealso \code{\link{getTik}}
#' 
#' @author Matthieu MARBAC
#' @family plot
#' @export
heatmapTikSorted <- function(output, pkg = c("ggplot2", "plotly"), ...){
  pkg = match.arg(pkg)
  
  # orderTik, they are saved at slot ordertik of JSON file
  if(is.null(output$algo$dictionary$z_class))
    classNames <- 1:output$algo$nClass
  else
    classNames <- output$algo$dictionary$z_class$old
  
  orderTik <- unlist(sapply(1:output$algo$nClass, 
                            function(k) order(output$variable$data$z_class$stat[,k] * (output$variable$data$z_class$completed == classNames[k]),
                                              decreasing = TRUE)[1:sum(output$variable$data$z_class$completed == classNames[k])]))

  
  tiksorted <- output$variable$data$z_class$stat[orderTik,]
  if(output$algo$nClass == 1) 
    tiksorted <- matrix(tiksorted, ncol = 1)
  
  # Text to display
  textMous <- sapply(1:output$algo$nClass, 
                     function(k) paste("Probability that <br> observation", 
                                       orderTik, 
                                       " <br>belongs to class",
                                       k, 
                                       ": <br>",
                                       round(tiksorted[,k],4)))
  
  
  orderTik <- factor(as.character(orderTik), levels=as.character(orderTik))
  
  if(is.null(output$algo$dictionary$z_class))
    namesClass <- paste0("Class ", 1:output$algo$nClass)
  else
    namesClass <- output$algo$dictionary$z_class$old
  
  heatmap <- switch(pkg,
                    "plotly" = heatmapplotly(tiksorted, xname = namesClass, main = "Probabilities of classification", text = textMous, ...),
                    "ggplot2" = ggheatmap(tiksorted, xname = namesClass, main = "Probabilities of classification", legendName = "Probabilities"))
  
  heatmap
}  


# @author Quentin Grimonprez
ggheatmap <- function(dat, xname, yname = 1:nrow(dat), main, xlab = "", ylab = "", legendName = "Value", addValues = FALSE)
{
  meltedX = data.frame(ind = factor(rep(yname, ncol(dat)), levels = yname), key = factor(rep(xname, each = nrow(dat)), levels = xname), value = as.numeric(dat))
  
  p <- ggplot(data = meltedX, aes_string(x = "key", y = "ind", fill = "value")) + 
    geom_tile() +
    theme_minimal() +
    theme(plot.title = element_text(hjust = 0.5), axis.text.x = element_text(angle = 45, vjust = 1, hjust = 1)) +
    labs(title = main, x = xlab, y = ylab) + 
    scale_fill_gradient(low = "#F7FBFF", high = "#08306B", limit = c(0, 1), name = legendName)
  
  if(all(yname == 1:nrow(dat)))
    p = p + theme(axis.text.y = element_blank(),
                  axis.ticks.y = element_blank())
  
  if(addValues)
  {
    p = p + geom_text(aes(label = round(value, 2)), color = "red", size = 4)
  }
  
  return(p)
}

# @author Matthieu Marbac
heatmapplotly <- function(dat, xname, yname = NULL, main, xlab = "", ylab = "", text = NULL, ...)
{
  heatmap <- plot_ly(text = text,
                     hoverinfo = "text",
                     z = as.table(dat),
                     x = xname,
                     y = yname,
                     colorscale = cbind(0:1, c("#F7FBFF", "#08306B")),
                     zmin = 0, zmax = 1, zauto = FALSE,
                     type = "heatmap",
                     showscale = TRUE, ...) %>%
    layout(title = main, showlegend = FALSE, 
           xaxis = list(ticks = "", title = ylab),
           yaxis = list(
             title = ylab,
             zeroline = FALSE,
             showline = FALSE,
             showticklabels = !is.null(yname),
             showgrid = FALSE, 
             ticks = ""
           ))
  
  heatmap
}
