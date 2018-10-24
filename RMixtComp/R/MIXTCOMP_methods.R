#' @title MixtComp Object Summaries
#'
#' Summary of a \code{\link{MixtComp}} object
#'  
#'
#' @param object \code{\link{MixtComp}} object
#' @param ... Not used.
#' 
#' 
#' @method summary MixtComp
#' 
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- list(nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # keep only 3 variables
#' desc <- simDesc$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")]
#' 
#' # run RMixtCompt in unsupervised clustering mode + data as matrix
#' resLearn <- mixtCompLearn(simDataLearn$matrix, desc, algo, nClass = 2:4)
#' 
#' summary(resLearn$res[[1]])
#' 
#' }
#' 
#' @seealso \link{MixtCompLearn} \link{print.MixtComp}
#' 
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
    cat("Time:", object$mixture$runTime,"s\n")
    cat("Observed log-likelihood:",object$mixture$lnObservedLikelihood,"\n")
    cat("BIC:",object$mixture$BIC,"\n")
    cat("ICL:",object$mixture$ICL,"\n")
    cat("Discriminative power:\n")
    print(discVar)
    cat("Proportion of the mixture: ")
    cat(round(getParam(object, "z_class"), 3), "\n")
    cat("Parameters of the most discriminant variables:\n")
    for(name in nameDiscVar[1:min(3, length(nameDiscVar))])
    {
      cat(name, ":", object$variable$type[[name]], "\n")
      print(getParam(object, name))
    }
  }
  cat("####################################\n")
  
}
  
#' @title MixtCompLearn Object Summaries
#'
#' Summary of a \code{\link{MixtCompLearn}} object
#'  
#'
#' @param object \code{\link{MixtCompLearn}} object
#' @param ... Not used.
#' 
#' 
#' @method summary MixtCompLearn
#' 
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- list(nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # keep only 3 variables
#' desc <- simDesc$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")]
#' 
#' # run RMixtCompt in unsupervised clustering mode + data as matrix
#' resLearn <- mixtCompLearn(simDataLearn$matrix, desc, algo, nClass = 2:4)
#' 
#' summary(resLearn)
#' }
#' 
#' @seealso \link{MixtCompLearn} \link{print.MixtCompLearn}
#' 
#' @export
summary.MixtCompLearn <- function(object, ...)
{
  cat("############### MixtCompLearn Run ###############\n")
  cat("nClass:", object$nClass,"\n")
  cat("Criterion used:", object$criterion,"\n")
  print(object$crit)
  cat("Best model:", ifelse(is.null(object$warnLog), paste0(object$algo$nClass, " clusters"), "none"), "\n")
  summary.MixtComp(object)
}


#' @title Print Values
#'
#' Print a \code{\link{MixtComp}} object
#'
#'  
#' @param x \code{\link{MixtComp}} object
#' @param nVarMaxToPrint number of variables to display (including z_class)
#' @param ... Not used.
#' 
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- list(nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # keep only 3 variables
#' desc <- simDesc$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")]
#' 
#' # run RMixtCompt in unsupervised clustering mode + data as matrix
#' resLearn <- mixtCompLearn(simDataLearn$matrix, desc, algo, nClass = 2:4)
#' 
#' print(resLearn$res[[1]])
#' }
#' 
#' @method print MixtComp
#' 
#' @seealso \link{mixtCompLearn} \link{mixtCompPredict}
#' 
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
      cat(paste0("$variable$param$", nameVar[i]), ": \n")
      print(x$variable$param[[nameVar[i]]])
    }
    if(nVarToPrint < nVar)
      cat("\t Other variables:", nameVar[-c(1:nVarToPrint)],"\n")
    
  }

}
    



#' @title Print Values
#'
#' Print a \code{\link{MixtCompLearn}} object
#'
#'  
#' @param x \code{\link{MixtCompLearn}} object
#' @param nVarMaxToPrint number of variables to display (including z_class)
#' @param ... Not used.
#' 
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- list(nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # keep only 3 variables
#' desc <- simDesc$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")]
#' 
#' # run RMixtCompt in unsupervised clustering mode + data as matrix
#' resLearn <- mixtCompLearn(simDataLearn$matrix, desc, algo, nClass = 2:4)
#' 
#' print(resLearn)
#' }
#' 
#' @method print MixtCompLearn
#' 
#' @seealso \link{mixtCompLearn} \link{mixtCompPredict}
#' 
#' @export
print.MixtCompLearn <- function(x, nVarMaxToPrint = 5, ...)
{
  cat("$nClass:", x$nClass, "\n")
  cat("$criterion: ", x$criterion,"\n")
  cat("$crit: \n")
  print(x$crit)
  cat("\n Best result\n")
  print.MixtComp(x, nVarMaxToPrint = nVarMaxToPrint, ...)
  cat("\n $res: results from all MixtComp Run\n")
}


#'
#' Plot of a \code{\link{MixtComp}} object
#'
#' @param x \code{\link{MixtComp}} object
#' @param nVarMaxToPrint number of variables to display
#' @param ... Not used.
#' 
#' @examples 
#' \donttest{
#' data(simData)
#'  
#' # define the algorithm's parameters
#' algo <- list(nbBurnInIter = 100,
#'              nbIter = 100,
#'              nbGibbsBurnInIter = 50,
#'              nbGibbsIter = 50,
#'              nInitPerClass = 10,
#'              nSemTry = 20,
#'              confidenceLevel = 0.95)
#' 
#' # keep only 3 variables
#' desc <- simDesc$unsupervised[c("Gaussian1", "Poisson1", "Categorical1")]
#' 
#' # run RMixtCompt in unsupervised clustering mode + data as matrix
#' resLearn <- mixtCompLearn(simDataLearn$matrix, desc, algo, nClass = 2:4)
#' 
#' plot(resLearn$res[[1]])
#' }
#' @method plot MixtComp
#' 
#' @seealso \link{mixtCompLearn} \link{mixtCompPredict}
#' @family plot
#' 
#' @export
plot.MixtComp <- function(x, nVarMaxToPlot = 5, pkg = c("ggplot2", "plotly"), ...)
{
  pkg = match.arg(pkg)
  
  p <- list()
  
  if(is.null(x$warnLog))
  {
    nVar <- length(x$variable$type) - 1
    nVarToPrint <- max(1, min(nVarMaxToPlot, nVar))
    
    discVar <- sort(computeDiscrimPowerVar(x), decreasing = TRUE)
    nameDiscVar <- names(discVar)
    
    
    p[[1]] = plotDiscrimVar(x, pkg = pkg)
    
    p[[2]] = plotProportion(x, pkg = pkg)
    
    for(i in 1:nVarToPrint)
      p[[i + 2]] = plotDataCI(x, nameDiscVar[i], pkg = pkg)
    
  }
  
  p
}

