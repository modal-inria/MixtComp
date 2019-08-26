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
#' @param output object returned by \emph{mixtCompLearn} function from \emph{RMixtComp} or \emph{rmcMultiRun} function from \emph{RMixtCompIO}
#' @param var name of the variable
#' @param grl if TRUE plot the general distribution of the data
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
         "Numerical" = plotBoxplotperClass(extractBoundsBoxplotNumericalVble(var, output, class = 1:output$algo$nClass, grl = grl)),
         "Multinomial" = plotCategoricalData(extractBoundsBoxplotCategoricalVble(var, output, class = 1:output$algo$nClass, grl = grl), var, class = 1:output$algo$nClass, grl),
         "Func_CS" = plotFunctionalData(output, var, ...),
         "Func_SharedAlpha_CS" = plotFunctionalData(output, var, ...),
         warning(paste0("Not (yet) available for model ", type)))
}

# here bounds is a matrix
# @author Matthieu Marbac
plotBoxplotperClass <- function(bounds){
  # Définition des paramètres du graphiques
  plot(NA,
       xlim=range(as.numeric(bounds)),
       ylim=c(0.2, nrow(bounds) + .8 ),
       xlab="value",
       ylab="class",
       yaxt = "n",
       lab=c(10, nrow(bounds), 5))
  ytick <- 1:nrow(bounds)
  axis(side = 2, at = ytick, labels = rownames(bounds))
  # Boucle sur les classes
  for (k in 1:nrow(bounds)){
    # Lines horizontales
    lines(c(bounds[k,1], bounds[k,2]), c(k, k) )
    lines(c(bounds[k,2], bounds[k,4]), (-.3)* c(1, 1) + k )
    lines(c(bounds[k,2], bounds[k,4]), (.3)* c(1, 1) + k )
    lines(c(bounds[k,4], bounds[k,5]), c(k, k) )
    # Lines verticales
    lines(rep(bounds[k,1], 2), c(-.15,.15) + k)
    lines(rep(bounds[k,2], 2), c(-.3,.3) + k)
    lines(rep(bounds[k,3], 2), c(-.3,.3) + k, lwd=2)
    lines(rep(bounds[k,4], 2), c(-.3,.3) + k)
    lines(rep(bounds[k,5], 2), c(-.15,.15) + k)
  }
}
