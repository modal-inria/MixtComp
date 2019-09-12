# MixtComp version 4.0  - july 2019
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

#' Plot BIC and ICL 
#'
#' @param output \emph{MixtCompLearn} object
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... arguments to be passed to plot_ly
#' 
#' @examples 
#' data(iris)
#'  
#' # define the algorithm's parameters
#' algo <- createAlgo()
#' 
#' # keep only 3 variables
#' model <- list(Petal.Width = "Gaussian", Petal.Length = "Gaussian",
#'               Sepal.Width = "Gaussian", Sepal.Length = "Gaussian")
#' 
#' # run RMixtComp in unsupervised clustering mode + data as matrix
#' res <- mixtCompLearn(iris, model, algo, nClass = 1:4)
#' 
#' # plot
#' plotCrit(res)
#' 
#' @family plot
#' @author Quentin Grimonprez
#' @export
plotCrit <- function(output, pkg = c("ggplot2", "plotly"), ...)
{
  pkg = match.arg(pkg)
  
  p <- switch(pkg, 
              "ggplot2" = ggplotCrit(output$crit, output$nClass),
              "plotly" = plotlyCrit(output$crit, output$nClass, ...))
  
  p
}

# @author Quentin Grimonprez
plotlyCrit <- function(crit, nClass, ...)
{
  if(length(nClass) == 1)
  {
    p <- plot_ly(x = nClass, y = crit[1,], type = "scatter", mode = "markers", symbol = 1, name = "BIC", size = 2, ...) %>%
      add_trace(x = nClass, y = crit[2,], name = "ICL", symbol = 2, size = 2)%>%
      layout(title = "Criterion", showlegend = TRUE, xaxis = list(title = "Number of classes"), yaxis = list(title = "Value"))
  }else{
    p <- plot_ly(x = nClass, y = crit[1,], type = "scatter", mode = "lines", name = "BIC", ...) %>%
      add_trace(x = nClass, y = crit[2,], name = "ICL")%>%
      layout(title = "Criterion", showlegend = TRUE, xaxis = list(title = "Number of classes"), yaxis = list(title = "Value"))
  }
  
  
  p
}

# @author Quentin Grimonprez
ggplotCrit <- function(crit, nClass)
{
  df = data.frame(class = nClass, value = c(crit[1,], crit[2,]), Criterion = rep(c("BIC", "ICL"), each = length(nClass)))
  
  p <- ggplot(data = df, aes_string(x = "class", y = "value", col = "Criterion")) +
    labs(title = "Criterion", x = "Number of classes", y = "value")
  
  if(length(nClass) == 1)
    p = p + geom_point(aes_string(shape = "Criterion"), size = 3)
  else
    p = p + geom_line()
  p
}
