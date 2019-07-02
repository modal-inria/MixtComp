#' @title MixtComp Object Summaries
#'
#' @description Summary of a \emph{MixtComp} object
#'
#' @param object \emph{MixtComp} object
#' @param ... Not used.
#' 
#' 
#' @method summary MixtComp
#' 
#' @examples 
#' \donttest{
#' data(iris)
#' 
#' # run RMixtComp in unsupervised clustering mode and in basic mode
#' resLearn <- mixtCompLearn(iris[, -5], nClass = 2:4)
#' 
#' summary(resLearn$res[[2]])
#' }
#' 
#' @seealso \code{mixtCompLearn} \code{\link{print.MixtComp}}
#' @author Quentin Grimonprez
#' @export
summary.MixtComp <- function(object, ...)
{
  cat("########### MixtComp Run ###########\n")
  if(!is.null(object$warnLog))
  {
    cat("Run failed:\n")
    cat(object$warnLog)
  }else{
    discVar <- sort(computeDiscrimPowerVar(object), decreasing = TRUE)
    nameDiscVar <- names(discVar)
    cat("Number of individuals:", object$algo$nInd,"\n")
    cat("Number of variables:", length(object$variable$type)-1,"\n")
    cat("Number of clusters:", object$algo$nClass,"\n")
    cat("Mode:", object$algo$mode,"\n")
    cat("Time:", object$mixture$runTime$total,"s\n")
    cat("SEM burn-in iterations done:", paste0(length(object$mixture$completedProbabilityLogBurnIn),"/", object$algo$nbBurnInIter),"\n")
    cat("SEM run iterations done:", paste0(length(object$mixture$completedProbabilityLogRun),"/", object$algo$nbIter),"\n")
    cat("Observed log-likelihood:", object$mixture$lnObservedLikelihood,"\n")
    cat("BIC:", object$mixture$BIC,"\n")
    cat("ICL:", object$mixture$ICL,"\n")
    cat("Discriminative power:\n")
    print(round(discVar, 3))
    cat("Proportions of the mixture:\n")
    cat(round(getParam(object, "z_class"), 3), "\n")
    cat("Parameters of the most discriminant variables:\n")
    for(name in nameDiscVar[1:min(3, length(nameDiscVar))])
    {
      cat(paste0("- ", name, ":"), object$variable$type[[name]], "\n")
      param <- getParam(object, name)
      if(is.list(param))
        print(lapply(param, round, 3))
      else
        print(round(param, 3))
    }
  }
  cat("####################################\n")
  
}


#' @title Print Values
#'
#' @description Print a \emph{MixtComp} object
#'  
#' @param x \emph{MixtComp} object
#' @param nVarMaxToPrint number of variables to display (including z_class)
#' @param ... Not used.
#' 
#' @examples 
#' \donttest{
#' data(iris)
#' 
#' # run RMixtComp in unsupervised clustering mode and in basic mode
#' resLearn <- mixtCompLearn(iris[, -5], nClass = 2:4)
#' 
#' print(resLearn$res[[2]])
#' }
#' 
#' @method print MixtComp
#' 
#' @seealso \code{mixtCompLearn} \code{mixtCompPredict}
#' @author Quentin Grimonprez
#' @export
print.MixtComp <- function(x, nVarMaxToPrint = 5, ...)
{
  if(!is.null(x$warnLog))
  {
    cat("$warnLog:\n")
    print(x$warnLog)
  }else{
    nVar <- length(x$variable$type)
    nVarToPrint <- max(1, min(nVarMaxToPrint, nVar))
    
    discVar <- sort(computeDiscrimPowerVar(x), decreasing = TRUE)
    nameDiscVar <- names(discVar)
    
    nameVar <- c("z_class", nameDiscVar)
    
    cat("$algo\n")
    algoToPrint <- c("nInd", "nClass", "mode")
    for(i in algoToPrint)
      cat(paste0("$algo$",i,":"), x$algo[[i]], "\n")
    cat("\t Other outputs:", setdiff(names(x$algo), algoToPrint), "\n")
    cat("\n")
    
    cat("$mixture\n")
    mixtureToPrint <- c("lnObservedLikelihood", "nbFreeParameters", "BIC", "ICL")
    for(i in mixtureToPrint)
      cat(paste0("$mixture$",i,":"), x$mixture[[i]], "\n")
    cat("\t Other outputs:", setdiff(names(x$mixture), mixtureToPrint), "\n")
    cat("\n")
    
    cat("$variable\n")
    cat("$variable$type\n")
    for(i in 1:nVarToPrint)
      cat(paste0("$variable$type$", nameVar[i]), ": ", x$variable$type[[nameVar[i]]], "\n")
    if(nVarToPrint < nVar)
      cat("\t Other variables:", nameVar[-c(1:nVarToPrint)],"\n")
    cat("\n")
    
    cat("$variable$data\n")
    cat("$variable$data$z_class$completed: ", head(x$variable$data$z_class$completed), "...\n")
    cat("$variable$data$z_class$stat:\n ")
    print(head(x$variable$data$z_class$stat))
    cat("\n")
    cat("\t Other variables:", nameVar[-1],"\n")
    cat("\n")
    
    cat("$variable$param\n")
    for(i in 1:nVarToPrint)
    {
      cat(paste0("$variable$param$", nameVar[i], ": \n"))
      print(x$variable$param[[nameVar[i]]]$stat)
    }
    if(nVarToPrint < nVar)
      cat("\t Other variables:", nameVar[-c(1:nVarToPrint)],"\n")
    
  }
  
}


#'
#' Plot of a \emph{MixtComp} object
#'
#' @param x \emph{MixtComp} object
#' @param nVarMaxToPlot number of variables to display
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... Not used.
#' 
#' @examples 
#' \donttest{
#' data(iris)
#' 
#' # run RMixtComp in unsupervised clustering mode and in basic mode
#' resLearn <- mixtCompLearn(iris[, -5], nClass = 2:4)
#' 
#' plot(resLearn$res[[2]])
#' }
#' 
#' @method plot MixtComp
#' 
#' @seealso \code{mixtCompLearn} \code{mixtCompPredict}
#' @family plot
#' @author Quentin Grimonprez
#' @export
plot.MixtComp <- function(x, nVarMaxToPlot = 3, pkg = c("ggplot2", "plotly"), ...)
{
  pkg = match.arg(pkg)
  
  p <- list()
  
  if(is.null(x$warnLog))
  {
    nVar <- length(x$variable$type) - 1
    nVarToPrint <- max(1, min(nVarMaxToPlot, nVar))
    
    discVar <- sort(computeDiscrimPowerVar(x), decreasing = TRUE)
    nameDiscVar <- names(discVar)
    
    
    p$discrimPowerVar = plotDiscrimVar(x, pkg = pkg)
    
    p$proportion = plotProportion(x, pkg = pkg)
    
    for(i in 1:nVarToPrint)
      p[[nameDiscVar[i]]] = plotDataCI(x, nameDiscVar[i], pkg = pkg)
    
  }
  
  p
}


