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
 

#' Convergence of algorithm
#' 
#' Plot the evolution of the completed loglikelihood during the SEM algorithm. The vertical line denotes the end of the burn-in phase.
#' 
#' @param output object returned by function \emph{mixtCompLearn}
#' @param ... graphical parameters
#' 
#' @details 
#' This function can be used to check the convergence and choose the parameters nbBurnInIter and nbIter from mcStrategy.
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
#' plotConvergence(res)
#' plotConvergence(res$res[[2]])
#' } 
#' 
#' @author Quentin Grimonprez
#' @family plot
#' @export
plotConvergence <- function(output, ...)
{
  plot(c(output$mixture$completedProbabilityLogBurnIn, output$mixture$completedProbabilityLogRun), xlab = "Iteration",
       ylab = "Completed loglikelihood", type = "l", main = "Completed loglikelihood during and\n after the burn-in period", ...)
  abline(v = length(output$mixture$completedProbabilityLogBurnIn) + 0.5, lty = "dotted", col = "red")
}

#' Evolution of parameters
#' 
#' Plot the evolution of estimated parameters after the burn-in phase.
#' 
#' @param output object returned by function \emph{mixtCompLearn}
#' @param var name of the variable
#' @param ... graphical parameters
#' 
#' 
#' 
#' @examples
#' \donttest{
#' data(simData)
#' 
#' # define the algorithm's parameters
#' algo <- createAlgo()
#' 
#' # run RMixtComp in unsupervised clustering mode + data as matrix
#' res <- mixtCompLearn(simData$dataLearn$matrix, simData$model$unsupervised, algo, nClass = 2:4)
#' 
#' # plot
#' plotParamConvergence(res, "Gaussian1")
#' plotParamConvergence(res, "Poisson1")
#' 
#' }
#' 
#' @author Quentin Grimonprez
#' @family plot
#' @export
plotParamConvergence <- function(output, var, ...)
{
  if(!(var%in%names(output$variable$type)))
    stop("This variable does not exist in the mixture model.")
  
  type <- output$variable$type[[var]]
  nbCluster <- output$algo$nClass
  
  type <- ifelse(type %in% c("Gaussian", "Weibull", "Multinomial", "Poisson", "NegativeBinomial", "LatentClass"), "Simple", type)
  type = ifelse(type %in% c("Func_SharedAlpha_CS"), "Func_CS", type)
  
  switch(type,
         "Simple" = plotLog(output$variable$param[[var]]$log, nbCluster, var, ...),
         "Rank_ISR" = plotLog(output$variable$param[[var]]$pi$log, nbCluster, var, ...),
         "Func_CS" = {plotLog(output$variable$param[[var]]$alpha$log, nbCluster, var, ...)
           title("alpha", line = -1, outer = TRUE)
           plotLog(output$variable$param[[var]]$beta$log, nbCluster, var, ...)
           title("beta", line = -1, outer = TRUE)
           plotLog(output$variable$param[[var]]$sd$log, nbCluster, var, ...)
           title("sd", line = -1, outer = TRUE)},
         warning(paste0("Not (yet) available for model ", type)))
}


# @author Quentin Grimonprez
plotLog <- function(paramLog, nbCluster, var, ...)
{
  nrowParam <- nrow(paramLog)
  nbParamPerClass <- nrowParam/nbCluster
  
  par(mfrow = n2mfrow(nbParamPerClass))
  for(i in 1:nbParamPerClass)
  {
    ind <- nbParamPerClass*(1:nbCluster) + (i-nbParamPerClass)
    name <- rownames(paramLog)[ind[1]]
    name <- gsub("^[^,]*, ", "", name)
    
    matplot(t(paramLog[ind,]), type = "l", main = paste0(var, ": ", name), xlab = "Iteration", ylab = name)
    legend("left", paste0("class ", 1:nbCluster), lty = 1:5, col = 1:6)
  }
  par(mfrow = c(1, 1))
}

