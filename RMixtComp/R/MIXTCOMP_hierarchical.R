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
 

# Hierarchical version of mixtCompLearn
# 
# 
# @param data data in matrix/data.frame/list format.
# @param desc list describing variables to keep and models to use
# @param algo list containing the parameters of the SEM-Gibbs algorithm
# @param nClass the number of class of the mixture model
# @param criterion criterion used to choose the order of clusters: "BIC", "ICL", "lnObservedLikelihood" or "lnCompletedLikelihood"
# @param minClassSize minimum number of elements in a cluster. If a cluster has less elements, this classes won't be splitted anymore.
# 
# @return a list containing res and history. res is a list containing the mixtComp outputs for 2 clusters to nClass clusters. History contains intermediate results.
# 
# @details
# 1) cluster data in 2 groups
# 2) A clustering in 2 groups is performed on each groups. The best clustering according to the specified criterion is kept to create a model with 3 groups
# 3) ...
# 
# @examples
# \donttest{
# set.seed(42, kind = "Mersenne-Twister", normal.kind = "Inversion")
# data <- matrix(nrow = 200, ncol = 2)
# data[,1] = c(rnorm(50, 0, 0.5), rnorm(50, -4, 0.8), rnorm(30, 2, 0.5), rnorm(70, -1, 0.4))
# data[,2] = c(rnorm(100, -2, 1), rnorm(100, 3, 0.4))
# colnames(data) = c("gauss1", "gauss2")
# truePartition <- rep(1:4, c(50, 50, 30, 70))
# 
# descriptor <- list(gauss1 = "Gaussian", gauss2 = "Gaussian")
# 
# algo <- list(nbBurnInIter = 100,
#                    nbIter = 100,
#                    nbGibbsBurnInIter = 50,
#                    nbGibbsIter = 50,
#                    nInitPerClass = 10,
#                    nSemTry = 20)
# 
# res <- hierarchicalMixtCompLearn(data, descriptor, algo, nClass = 4)
# }
# 
# @author Quentin Grimonprez
# @export
hierarchicalMixtCompLearn <- function(data, model, algo = createAlgo(), nClass, criterion = c("BIC", "ICL", "lnObservedLikelihood", "lnCompletedLikelihood"), minClassSize = 5,
                                      nRun = 1, nCore = min(max(1, ceiling(detectCores()/2)), nRun), verbose = TRUE)
{
  criterion = match.arg(criterion)
  
  
  data = formatData(data)
  model = formatModel(model)
  
  nInd <- length(data[[1]])
  currentPartition <- rep("?", nInd)
  
  data$z_class = currentPartition
  model$z_class = list(type = "LatentClass", paramStr = "")
  
  
  if(verbose)
  {
    cat(paste0("====== Run Hierarchical MixtComp in ", algo$mode, " mode with ", nRun, " run(s) per number of classes and ", nCore, " core(s)\n"))
    cat(paste0("Data: ", algo$nInd, " individuals and ", length(model), " variables.\n"))
  }
    
  
  newRes = leaves = res = allCrit <- list()
  
  ## initialization: K = 1
  t1 <- proc.time()
  if(verbose)
    cat(paste0("-- K = 1 \n"))
  
  algoTemp <- algo
  algoTemp$nInd = length(data[[1]])
  algoTemp$nClass = 1
  algoTemp$mode = "learn"
  
  res[[1]] <- rmcMultiRun(algoTemp, data, model, list(), nRun, nCore, verbose)
  class(res[[1]]) = "MixtComp"
  t2 <- proc.time()
  if(verbose)
    cat(paste0("Run time: ", round((t2-t1)[3], 3), "s\n"))
  
  ## if nClass = 1 there is no hierarchical process to perform
  if(nClass > 1)
  {
    leavesOrder = MAXCRIT <- c()
    
    # at the beginning everyone is in 1 cluster
    # a leaf has 3 modes for go: "toRun", "wait" and "stop":
    # "toRun" = need to split in two. 
    # "wait" = already split but not selected (will be split after being selected in a model)
    # "stop" = it won't be split anymore (not enough individuals)
    leaves[[1]] = list(partition = rep(TRUE, nInd) , indPartition = 1:nInd, go = "toRun", indParent = NULL)
    nbCurrentCluster <- 1
    nbLeavesToCompute <- 1
    
    
    while((nbCurrentCluster != nClass) & (nbLeavesToCompute != 0))
    {
      t1 <- proc.time()
      if(verbose)
      {
        cat(paste0("-- K = ", nbCurrentCluster + 1, "\n"))
        cat(paste0("Number of splits to perform: ", nbLeavesToCompute, "\n"))
      }
      
      # perform clustering for all leaves
      hasRun <- c()
      for(i in seq_along(leaves))
      {
        # run
        if(leaves[[i]]$go == "toRun")
        {
          if(verbose)
            cat(paste0(" Split a cluster in two\n"))
          
          # reduce the data to the classes to split
          resData <- data
          for(j in names(resData))
            resData[[j]] = data[[j]][leaves[[i]]$indPartition]
          
          algoTemp <- algo
          algoTemp$nInd = length(resData[[1]])
          algoTemp$nClass = 2
          algoTemp$mode = "learn"
          
          newRes[[i]] <- rmcMultiRun(algoTemp, resData, model, list(), nRun, nCore, verbose)
          class(newRes[[i]]) = "MixtComp"
          
          if(!is.null(newRes[[i]]$warnLog))
          {
            hasRun = c(hasRun, FALSE)
            warning(newRes[[i]]$warnLog)
          }else{
            hasRun = c(hasRun, TRUE)
          }
        }
      }
      
      if(all(!hasRun))
        break
      
      
      # compute criterion by computing all the models with K classes regarding the different available leaves
      if(verbose)
        cat(" Compute criterion\n")
      maxCrit <- list(crit = -Inf, ind = NA)
      bestRes = list(warnLog = "error")
      crit = list()
      for(i in seq_along(leaves))
      {
        if(leaves[[i]]$go %in% c("toRun", "wait"))
        {
          leaves[[i]]$go = "wait"
          if(is.null(leaves[[i]]$indParent))
          {
            maxCrit$ind = 1
            maxCrit$crit = newRes[[1]]$mixture[[criterion]]
            bestRes <- newRes[[1]]
          }else{
            if(is.null(newRes[[i]]$warnLog))
            {
              # indClass <- unique(res[[nbCurrentCluster]]$variable$data$z_class$completed[leaves[[i]]$indPartition])
              indClass <- unique(getPartition(res[[nbCurrentCluster]], empiric = FALSE)[leaves[[i]]$indPartition])
              
              tempRes <- computeNewModel(res[[nbCurrentCluster]], newRes[[i]], indClass, data, model, algo, nRun, nCore, verbose)
              if(!is.null(tempRes$warnLog))
              {
                crit[[i]] = -Inf
              }else{
                crit[[i]] <- tempRes$mixture[[criterion]]
              }
              
              if(crit[[i]] > maxCrit$crit)
              {
                maxCrit$crit = crit[[i]]
                maxCrit$ind = i
                bestRes = tempRes
              }
            }
          }
        }
      }
      MAXCRIT = c(MAXCRIT, maxCrit$crit)
      allCrit[[nbCurrentCluster]] = crit
      leavesOrder = c(leavesOrder, maxCrit$ind)
      
      
      if(!is.na(maxCrit$ind))
      {
        # update leaves for the next step: add the 2 new clusters
        leaves[[maxCrit$ind]]$go = "parent"
        part <- getPartition(newRes[[maxCrit$ind]], empiric = FALSE)
        leaves[[length(leaves) + 1]] = list(partition = (part == 1), indPartition = leaves[[maxCrit$ind]]$indPartition[part == 1], 
                                            go = "toRun", indParent = maxCrit$ind)
        leaves[[length(leaves) + 1]] = list(partition = (part == 2), indPartition = leaves[[maxCrit$ind]]$indPartition[part == 2], 
                                            go = "toRun", indParent = maxCrit$ind)
        
        # stop some classes due to size
        for(i in seq_along(leaves))
        {
          if(leaves[[i]]$go %in% c("toRun", "wait"))
          {
            if(sum(leaves[[i]]$partition) <= minClassSize)
              leaves[[i]]$go = "stop"
          }
        }
        nbLeavesToCompute = sum(sapply(leaves, FUN = function(x){x$go %in% c("toRun")}))
        nbCurrentCluster = nbCurrentCluster + 1
        res[[nbCurrentCluster]] = bestRes
      }else{
        # if maxCrit$ind == NA, no leaves has produced a model, we force the stop of hierarchical
        warning(paste0("The hierarchy was stop earlier because all clustering on leaves produced error. Try to increase the nRun parameter."))
        nbLeavesToCompute = 0
      }
      t2 <- proc.time()
      
      if(verbose)
        cat(paste0("Run time: ", round((t2-t1)[3], 3), "s\n"))
   
    }# end while
    
    # tell the user if we have not attained the desired number of cluster
    if(nbCurrentCluster != nClass)
      warning(paste0("Unable to get ", nClass," clusters. Only ", nbCurrentCluster," clusters were computed."))
    
    names(MAXCRIT) = 2:nbCurrentCluster

    out <- c(res[[length(res)]], list(res = res, crit = MAXCRIT, nClass = 1:nbCurrentCluster, history = list(leavesOrder = leavesOrder, leaves = leaves, resLeaves = newRes, critStep = allCrit)))
    if(is.null(out$warnLog))
      out$algo$hierarchicalMode = TRUE
    
    class(out) = c("MixtCompLearn", "MixtComp")
  }else{
    out <- c(res[[1]], list(res = res, nClass = 1))
    if(is.null(out$warnLog))
      out$algo$hierarchicalMode = TRUE
    
    class(out) = c("MixtCompLearn", "MixtComp")
  }
  
  
  
  return(out)
}


# Compute the new model with the given parent and child
#
# Compute the parameter of the model (in supervised) with the clusters of child replacing one cluster of the parent
#
# @param resParent result object of a MixtComp run.
# @param resChild result object of a MixtComp run. This run is performed on the individuals of one cluster of resParent
# @param classParentOfChild cluster of the parent used for the child
# @param data
# @param model
# @param algo
#
# @return mixtComp res
# @author Quentin Grimonprez
computeNewModel <- function(resParent, resChild, classParentOfChild, data, model, algo, nRun, nCore, verbose)
{
  ## Run Mixtcomp with fixed partition in order to estimate parameter and criterion for the given leaves
  
  # compute new partition
  oldZ_class = getPartition(resParent, empiric = FALSE) # resParent$variable$data$z_class$completed
  childPartition <- getPartition(resChild, empiric = FALSE) # resChild$variable$data$z_class$completed 
  newZ_class = rep(NA, length(oldZ_class))
  
  newZ_class[oldZ_class == classParentOfChild] = childPartition + (resParent$algo$nClass - 1)
  newZ_class[oldZ_class != classParentOfChild] <- refactorCategorical(oldZ_class[oldZ_class != classParentOfChild],
                                                                      unique(oldZ_class[oldZ_class != classParentOfChild]), 
                                                                      seq_along(unique(oldZ_class[oldZ_class != classParentOfChild])))
  
  data$z_class = as.character(newZ_class)
  
  
  # run with fixed partition
  algo$nClass = resParent$algo$nClass + 1
  algo$nInd = resParent$algo$nInd
  algo$mode = "learn"
  algo$nInitPerClass = algo$nInd
  
  res <- rmcMultiRun(algo, data, model, list(), nRun, nCore, verbose)
  class(res) = "MixtComp"
  
  return(res)
}
