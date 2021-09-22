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
 

#' Histogram of the misclassification probabilities 
#'
#' @details Missclassification probability of observation i is denoted err_i
#' err_i = 1 - max_{k={1,\ldots,K}} P(Z_i=k|x_i)
#' Histograms of err_i's can be plotted for a specific class, all classes or every class
#'
#' @param output object returned by \emph{mixtCompLearn} function from \emph{RMixtComp} or \emph{rmcMultiRun} function from \emph{RMixtCompIO}
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... arguments to be passed to plot_ly
#' 
#' @examples 
#' require(RMixtCompIO) # for learning a mixture model
#' dataLearn <- list(var1 = as.character(c(rnorm(50, -2, 0.8), rnorm(50, 2, 0.8))),
#'                   var2 = as.character(c(rnorm(50, 2), rpois(50, 8))))
#'                   
#' model <- list(var1 = list(type = "Gaussian", paramStr = ""),
#'               var2 = list(type = "Poisson", paramStr = ""))
#' 
#' algo <- list(
#'   nClass = 2,
#'   nInd = 100,
#'   nbBurnInIter = 100,
#'   nbIter = 100,
#'   nbGibbsBurnInIter = 100,
#'   nbGibbsIter = 100,
#'   nInitPerClass = 3,
#'   nSemTry = 20,
#'   confidenceLevel = 0.95,
#'   ratioStableCriterion = 0.95,
#'   nStableCriterion = 10,
#'   mode = "learn"
#' )
#' 
#' resLearn <- rmcMultiRun(algo, dataLearn, model, nRun = 3)
#' 
#' # plot
#' histMisclassif(resLearn)
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
  if(is.null(output$algo$dictionary$z_class))
    classNames <- 1:output$algo$nClass
  else
    classNames <- output$algo$dictionary$z_class$old
  
  p <- switch(pkg,
              "ggplot2" = gghistMisclassif(z, misclassifrisk, classNames),
              "plotly" = plotlyhistMisclassif(z, misclassifrisk, classNames, ...))
  
  p
}

# @author Matthieu Marbac
plotlyhistMisclassif <- function(z, misclassifrisk, classNames, ...){
  
  G <- length(classNames)
  
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
                                                label = paste("class", classNames[k], sep="."))))
  
  # a list with plotly compliant formatted objects
  formattedW <- c(list(
    list(x = misclassifrisk,  name= "General", visible=TRUE, nbinsx=20)), 
    lapply(1:G, function(k) 
      list(x = misclassifrisk[which(z == classNames[k])], name= paste("class", classNames[k], sep = "."), visible = FALSE)
    ))
  
  # Reduce the list of plotly compliant objs, starting with the plot_ly() value and adding the `add_trace` at the following iterations
  histerrors <- Reduce(function(acc, curr)  do.call(add_histogram, c(list(p=acc),curr)),
                       formattedW,
                       init=plot_ly()%>%layout(
                         showlegend = TRUE,
                         title = "Misclassification risk",
                         xaxis = list(domain = c(-0.09, 0.9), title="Probabilities of misclassification"),
                         yaxis = list(title = "Percentile of observations with <br> a misclassification risk less than x"),
                         updatemenus = list(list(y=0.6, x=-0.15,buttons = listbuttons)))
  )
  
  histerrors
}

# to avoid note during R CMD check
globalVariables("..count..")

# @author Quentin Grimonprez
gghistMisclassif <- function(z, misclassifrisk, classNames)
{
  df = data.frame(class = factor(z, levels = classNames), misclassifrisk = misclassifrisk)
  G <- length(classNames)
  
  p <- list()
  
  # general
  p$general = ggplot(df, aes(x = misclassifrisk, y = ..count../sum(..count..))) + 
    geom_histogram(position = "identity", binwidth = 0.05, colour = "black", alpha = 0.8) +
    labs(title = "Misclassification risk", x = "Probabilities of misclassification", y = "Percentile of observations with \na misclassification risk less than x")
  
  # each
  p$each = ggplot(df, aes(x = misclassifrisk, y = ..count../sum(..count..), fill = class)) + 
    geom_histogram(position = "dodge", binwidth = 0.05) +
    labs(title = "Misclassification risk", x = "Probabilities of misclassification", y = "Percentile of observations with \na misclassification risk less than x")
  
  # class by class
  for(i in 1:G)
  {
    p[[paste0("class.", classNames[i])]] = ggplot(subset(df, class == classNames[i]), aes(x = misclassifrisk, y = ..count../sum(..count..)), fill = class) + 
      geom_histogram(position = "dodge", binwidth = 0.05, fill = hue_pal()(G)[i], colour = "black") + 
      labs(title = paste0("Misclassification risk: class ", classNames[i]), x = "Probabilities of misclassification", y = "Percentile of observations with \na misclassification risk less than x")
  }
  
  return(p)
}

