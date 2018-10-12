#' Hierarchical version of mixtCompLearn
#'
#'
#' @param data data in matrix/data.frame/list format.
#' @param desc list describing variables to keep and models to use
#' @param algo list containing the parameters of the SEM-Gibbs algorithm
#' @param nClass the number of class of the mixture model
#' @param criterion criterion used to choose the order of clusters: "BIC", "ICL", "lnObservedLikelihood" or "lnCompletedLikelihood"
#' @param minClassSize minimum number of elements in a cluster. If a cluster has less elements, this classes won't be splitted anymore.
#'
#' @return a list containing res and history. res is a list containing the mixtComp outputs for 2 clusters to nClass clusters. History contains intermediate results.
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
#' descriptor <- list(gauss1 = "Gaussian", gauss2 = "Gaussian")
#'
#' algo <- list(nbBurnInIter = 100,
#'                    nbIter = 100,
#'                    nbGibbsBurnInIter = 50,
#'                    nbGibbsIter = 50,
#'                    nInitPerClass = 10,
#'                    nSemTry = 20)
#'
#' res <- hierarchicalMixtCompLearn(data, descriptor, algo, nClass = 4)
#' }
#'
#' @author Quentin Grimonprez
#'
#' @export
hierarchicalMixtCompLearn <- function(data, desc, algo = list(nbBurnInIter = 100,
                                                             nbIter = 100,
                                                             nbGibbsBurnInIter = 50,
                                                             nbGibbsIter = 50,
                                                             nInitPerClass = 10,
                                                             nSemTry = 20,
                                                             confidenceLevel = 0.95), nClass, criterion = c("BIC", "ICL", "lnObservedLikelihood", "lnCompletedLikelihood"), minClassSize = 5)
{
  criterion = match.arg(criterion)


  data = RMixtComp:::formatData(data)
  desc = RMixtComp:::formatDesc(desc)

  nInd <- length(data[[1]])
  currentPartition <- rep("?", nInd)

  data$z_class = currentPartition
  desc$z_class = list(type = "LatentClass", paramStr = "")


  newRes = leaves = res = allCrit <- list()

  leavesOrder = MAXCRIT <- c()

  leaves[[1]] = list(partition = rep(TRUE, nInd) , indPartition = 1:nInd, go = "toRun", indParent = NULL)
  nbCurrentCluster <- 1
  nbLeavesToCompute <- 1


  while((nbCurrentCluster != nClass) & (nbLeavesToCompute != 0))
  {
    hasRun <- c()
    for(i in seq_along(leaves))
    {
      # run
      if(leaves[[i]]$go == "toRun")
      {
        # reduce the data to the class to split
        resData <- data
        for(j in names(resData))
          resData[[j]] = data[[j]][leaves[[i]]$indPartition]

        algoTemp <- algo
        algoTemp$nInd = length(resData[[1]])
        algoTemp$nClass = 2
        algoTemp$mode = "learn"

        newRes[[i]] <- RMixtComp:::rmc(algoTemp, resData, desc, list())
        class(newRes) = "MixtComp"

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
          if(is.null(newRes[[i]]$warnLog))
          {
            indClass <- unique(res[[nbCurrentCluster]]$variable$data$z_class$completed[leaves[[i]]$indPartition])

            tempRes <- computeNewModel(res[[nbCurrentCluster]], newRes[[i]], indClass, data, desc, algo)
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


  if(nbCurrentCluster != nClass)
    warning(paste0("Unable to get ", nClass," clusters. Only ", nbCurrentCluster," clusters were computed."))

  names(MAXCRIT) = 2:nbCurrentCluster

  out <- c(res[[length(res)]], list(res = res, crit = MAXCRIT, nClass = 2:nbCurrentCluster, history = list(leavesOrder = leavesOrder, leaves = leaves, resLeaves = newRes, critStep = allCrit)))
  class(out) = c("MixtCompLearn", "MixtComp")

  return(out)
}
