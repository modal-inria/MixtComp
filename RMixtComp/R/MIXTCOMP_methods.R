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
 

#' @title MixtCompLearn Object Summaries
#'
#' @description Summary of a \emph{MixtCompLearn} object  
#'
#' @param object \emph{MixtCompLearn} object
#' @param nClass number of classes of the model to print
#' @param ... Not used.
#' 
#' 
#' @method summary MixtCompLearn
#' 
#' @examples 
#' data(iris)
#' 
#' # run RMixtComp in unsupervised clustering mode and in basic mode
#' resLearn <- mixtCompLearn(iris[, -5], nClass = 2:4)
#' 
#' summary(resLearn)
#' summary(resLearn, nClass = 3)
#' 
#' @seealso \code{\link{mixtCompLearn}} \code{print.MixtCompLearn}
#' @author Quentin Grimonprez
#' @export
summary.MixtCompLearn <- function(object, nClass = NULL, ...)
{
  cat("############### MixtCompLearn Run ###############\n")
  cat("nClass:", object$nClass,"\n")
  cat("Criterion used:", object$criterion,"\n")
  print(object$crit)
  cat("Best model:", ifelse(is.null(object$warnLog), paste0(object$algo$nClass, " clusters"), "none"), "\n")
  
  if(!is.null(nClass) && (nClass[1] %in% object$nClass))
  {
    RMixtCompUtilities:::summary.MixtComp(object$res[[which(object$nClass == nClass[1])]])
  }else{
    RMixtCompUtilities:::summary.MixtComp(object)
  }
}




#' @title Print Values
#'
#' @description Print a \emph{MixtCompLearn} object
#'
#' @param x \emph{MixtCompLearn} object
#' @param nVarMaxToPrint number of variables to display (including z_class)
#' @param nClass number of classes of the model to print
#' @param ... Not used.
#' 
#' @examples 
#' data(iris)
#' 
#' # run RMixtComp in unsupervised clustering mode and in basic mode
#' resLearn <- mixtCompLearn(iris[, -5], nClass = 2:4)
#' 
#' print(resLearn)
#' print(resLearn, nClass = 3)
#' 
#' 
#' @method print MixtCompLearn
#' 
#' @seealso \code{\link{mixtCompLearn}} \code{\link{mixtCompPredict}}
#' @author Quentin Grimonprez
#' @export
print.MixtCompLearn <- function(x, nVarMaxToPrint = 5, nClass = NULL, ...)
{
  cat("$nClass:", x$nClass, "\n")
  cat("$criterion: ", x$criterion,"\n")
  cat("$crit: \n")
  print(x$crit)
  
  if(!is.null(nClass) && (nClass[1] %in% x$nClass))
  {
    cat(paste0("\n Result for ", nClass[1], " classes\n"))
    RMixtCompUtilities:::print.MixtComp(x$res[[which(x$nClass == nClass[1])]], nVarMaxToPrint = nVarMaxToPrint)
  }else{
    cat("\n Best result\n")
    RMixtCompUtilities:::print.MixtComp(x, nVarMaxToPrint = nVarMaxToPrint)
  }
  cat("\n $res: results from all MixtComp Run\n")
}


#'
#' Plot of a \emph{MixtCompLearn} object
#'
#' @param x \emph{MixtCompLearn} object
#' @param nVarMaxToPlot number of variables to display
#' @param nClass number of classes of the model to plot
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param plotData "CI" or "Boxplot". If "CI", uses \link{plotDataCI} function. If "Boxplot", uses \link{plotDataBoxplot}
#' @param ... extra parameter for \link{plotDataCI} or \link{plotDataBoxplot}
#' 
#' @examples 
#' data(iris)
#' 
#' # run RMixtComp in unsupervised clustering mode and in basic mode
#' resLearn <- mixtCompLearn(iris[, -5], nClass = 2:4)
#' 
#' plot(resLearn)
#' plot(resLearn, nClass = 3, plotData = "Boxplot")
#' 
#' @method plot MixtCompLearn
#' 
#' @seealso \code{\link{mixtCompLearn}} \code{\link{mixtCompPredict}}
#' @family plot
#' @author Quentin Grimonprez
#' @export
plot.MixtCompLearn <- function(x, nVarMaxToPlot = 3, nClass = NULL, pkg = c("ggplot2", "plotly"), plotData = c("CI", "Boxplot"), ...)
{
  pkg = match.arg(pkg)
  
  p <- list()
  
  if(is.null(x$warnLog))
  {
    p$criteria = plotCrit(x, pkg, ...)
    
    if(!is.null(nClass) && (nClass[1] %in% x$nClass))
    {
      p2 <- plot(x$res[[which(x$nClass == nClass[1])]], nVarMaxToPlot = nVarMaxToPlot, nClass = nClass, pkg = pkg, plotData = plotData, ...)
    }else{
      p2 <- plot(x$res[[which(x$nClass == x$algo$nClass)]], nVarMaxToPlot = nVarMaxToPlot, nClass = nClass, pkg = pkg, plotData = plotData, ...)
    }
    
    p = c(p, p2)
  }
  
  p
}

