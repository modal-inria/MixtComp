#' Hierarchical version of mixtCompLearn
#'
#'
#' @param dataList \emph{lm} element of the output of \link{getData}
#' @param mcStrategy a list containing the parameters of the SEM-Gibbs algorithm
#' @param nbClass the number of class of the mixture model
#' @param confidenceLevel quantile for confidence interval of estimated parameters.
#' @param criterion criterion used to choose the order of clusters: "BIC", "ICL", "lnObservedLikelihood" or "lnCompletedLikelihood"
#' @param minClassSize minimum number of elements in a cluster. If a cluster has less elements, this classes won't be splitted anymore.
#'
#' @return a list containing res and history. res is a list containing the mixtComp outputs for 2 clusters to nbClass clusters. History contains intermediate results.
#'
#' @details
#' 1) cluster data in 2 groups
#' 2) A clustering in 2 groups is performed on each groups. The best clustering according to the specified criterion is kept to create a model with 3 groups
#' 3) ...
#'
#' @examples
#' \donttest{
#' set.seed(42)
#' data <- matrix(nrow = 200, ncol = 2)
#' data[,1] = c(rnorm(50, 0, 0.5), rnorm(50, -4, 0.8), rnorm(30, 2, 0.5), rnorm(70, -1, 0.4))
#' data[,2] = c(rnorm(100, -2, 1), rnorm(100, 3, 0.4))
#' colnames(data) = c("gauss1", "gauss2")
#' truePartition <- rep(1:4, c(50, 50, 30, 70))
#'
#' descriptor <- matrix("Gaussian_sjk", nrow = 1, ncol = 2, dimnames = list(NULL, colnames(data)))
#'
#' resGetData <- getData(list(data, descriptor))
#'
#' mcStrategy <- list(nbBurnInIter = 100,
#'                    nbIter = 100,
#'                    nbGibbsBurnInIter = 50,
#'                    nbGibbsIter = 50,
#'                    nInitPerClass = 10,
#'                    nSemTry = 20)
#'
#' res <- hierarchicalMixtCompLearn(resGetData$lm, mcStrategy, nbClass = 4)
#' }
#'
#' @author Quentin Grimonprez
#'
#' @export
hierarchicalMixtCompLearn <- function(dataList, mcStrategy = list(nbBurnInIter = 100,
                                                             nbIter = 100,
                                                             nbGibbsBurnInIter = 50,
                                                             nbGibbsIter = 50,
                                                             nInitPerClass = 10,
                                                             nSemTry = 20), nbClass, confidenceLevel = 0.95, criterion = c("BIC", "ICL", "lnObservedLikelihood", "lnCompletedLikelihood"), minClassSize = 5)
{
  criterion = match.arg(criterion)

  newRes = leaves = res = allCrit <- list()
  nInd <- length(dataList[[1]]$data)

  currentPartition <- rep("?", nInd)
  leavesOrder <- c()

  leaves[[1]] = list(partition = rep(TRUE, nInd) , indPartition = 1:nInd, go = "toRun", indParent = NULL)
  nbCurrentCluster <- 1
  nbLeavesToCompute <- 1

  dataList[[length(dataList)+1]] = list(data = rep("?", nInd), model = "LatentClass", id = "z_class", paramStr = "")


  while((nbCurrentCluster != nbClass) & (nbLeavesToCompute != 0))
  {
    hasRun <- c()
    for(i in seq_along(leaves))
    {
      # run
      if(leaves[[i]]$go == "toRun")
      {
        # reduce the data to the class to split
        resData <- dataList
        for(j in seq_along(resData))
          resData[[j]]$data = dataList[[j]]$data[leaves[[i]]$indPartition]

        newRes[[i]] <- RMixtComp::mixtCompCluster(resData, mcStrategy, nbClass = 2, confidenceLevel = confidenceLevel)


        if(!is.null(newRes[[i]]$mixture$warnLog))
        {
          hasRun = c(hasRun, FALSE)
          warning(newRes[[i]]$mixture$warnLog)
        }else{
          hasRun = c(hasRun, TRUE)
        }
      }
    }

    if(all(!hasRun))
      break

    # compute crit
    maxCrit <- list(crit = -Inf, ind = NA)
    bestRes = NULL
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
          if(is.null(newRes[[i]]$mixture$warnLog))
          {
            indClass <- unique(res[[nbCurrentCluster]]$variable$data$z_class$completed[leaves[[i]]$indPartition])

            tempRes <- computeNewModel(res[[nbCurrentCluster]], newRes[[i]], indClass, dataList, mcStrategy)
            crit[[i]] <- tempRes$mixture[[criterion]]
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

    allCrit[[nbCurrentCluster]] = crit
    leavesOrder = c(leavesOrder, maxCrit$ind)

    # update leaves for the next step
    leaves[[maxCrit$ind]]$go = "parent"
    part <- newRes[[maxCrit$ind]]$variable$data$z_class$completed
    leaves[[length(leaves) + 1]] = list(partition = (part == 1), indPartition = leaves[[maxCrit$ind]]$indPartition[part == 1], go = "toRun", indParent = maxCrit$ind)
    leaves[[length(leaves) + 1]] = list(partition = (part == 2), indPartition = leaves[[maxCrit$ind]]$indPartition[part == 2], go = "toRun", indParent = maxCrit$ind)

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

  }# end while


  if(nbCurrentCluster != nbClass)
    warning(paste0("Unable to get ", nbClass," clusters. Only ", nbCurrentCluster," clusters were computed."))


  return(list(res = res, history = list(leavesOrder = leavesOrder, leaves = leaves, resLeaves = newRes, critStep = allCrit)))
}
