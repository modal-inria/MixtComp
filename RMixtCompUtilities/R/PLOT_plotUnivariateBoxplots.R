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
 

#' Boxplot per class
#'
#' Display a boxplot (5%-25%-50%-75%-95% quantile) for the given variable for all classes
#'   
#' @param output object returned by \emph{mixtCompLearn} function from \emph{RMixtComp} or \emph{rmcMultiRun} function from \emph{RMixtCompIO}
#' @param var name of the variable
#' @param grl if TRUE plot the general distribution of the data
#' @param ... other parameters (see \emph{Details})
#'
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
#' plotDataBoxplot(resLearn, "var1")
#' 
#' 
#' @author Matthieu MARBAC
#' @family plot
#' @export
plotDataBoxplot <- function(output, var, grl = TRUE, ...)
{
  if(!(var%in%names(output$variable$type)))
    stop("This variable does not exist in the mixture model.")
  
  type <- output$variable$type[[var]]
  
  type <- ifelse(type %in% c("Gaussian", "Weibull", "Poisson", "NegativeBinomial"), "Numerical", type)
  
  switch(type,
         "Numerical" = plotBoxplotperClass(extractBoundsBoxplotNumericalVble(var, output, class = 1:output$algo$nClass, grl = grl), var),
         "Multinomial" = plotCategoricalData(extractBoundsBoxplotCategoricalVble(var, output, class = 1:output$algo$nClass, grl = grl), var, class = 1:output$algo$nClass, grl),
         "Func_CS" = plotFunctionalData(output, var, ...),
         "Func_SharedAlpha_CS" = plotFunctionalData(output, var, ...),
         warning(paste0("Not (yet) available for model ", type)))
}

# here bounds is a matrix
# @author Quentin Grimonprez
plotBoxplotperClass <- function(bounds, var, pkg = c("ggplot2", "plotly"), ...){
  pkg = match.arg(pkg)
  
  p <- switch(pkg, 
              "ggplot2" = ggplotBoxplotperClass(bounds, var),
              "plotly" = ggplotBoxplotperClass(bounds, var))
  
  p
}


# @author Quentin Grimonprez
ggplotBoxplotperClass  <- function(bounds, var)
{
  labelClass <- factor(rownames(bounds), levels = rownames(bounds))
  
  df = as.data.frame(bounds)
  df$class = labelClass
  df$classlo = 1:nrow(df) - 0.1
  df$classup = 1:nrow(df) + 0.1
  df$classmid = 1:nrow(df) 

  p <- ggplot() +
    geom_rect(data = df, mapping = aes_string(xmin = "`quantil. 0.25`", xmax = "`quantil. 0.75`", ymin = "classlo", ymax = "classup", fill = "class"), color = "black", alpha = 0.5) + 
    geom_rect(data = df, mapping = aes_string(xmin = "`quantil. 0.5`", xmax = "`quantil. 0.5`", ymin = "classlo", ymax = "classup"), color = "black")  + 
    geom_rect(data = df, mapping = aes_string(xmin = "`quantil. 0.05`", xmax = "`quantil. 0.25`", ymin = "classmid", ymax = "classmid"), color = "black")  + 
    geom_rect(data = df, mapping = aes_string(xmin = "`quantil. 0.75`", xmax = "`quantil. 0.95`", ymin = "classmid", ymax = "classmid"), color = "black") +
    labs(title = paste0("Boxplot per class for variable ", var), x = var, y = element_blank()) +
    theme(legend.position = "none") +
    scale_y_continuous(breaks = 1:nrow(df), labels = df$class)
  
  p
}

