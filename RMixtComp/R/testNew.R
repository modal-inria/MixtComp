testRank <- function(nbClass = 2,
                     nbBurnInIter = 100,
                     nbSampleLearn = 50,
                     nbSamplePredict = 1000,
                     confidenceLevel = 0.95,
                     regen = FALSE)
{  
  if (regen == TRUE)
  {
    dataGeneratorNewTest()
  }
  
  resGetData <- getData(c("dataGenNew/learn/data.csv",
                          "dataGenNew/learn/desc.csv"))
  
  # creation of strategy list
  mcStrategy <- list(nbBurnInIter = nbBurnInIter,
                     nbIter = 100,
                     nbGibbsBurnInIter = 100,
                     nbGibbsIter = 100)
  
  # launch of the MixtComp algorithm
  res <- mixtCompCluster(resGetData$lm,
                         mcStrategy,
                         nbClass,
                         confidenceLevel)
  if (nchar(res$mixture$warnLog) > 0)
  {
    warning(res$mixture$warnLog)
  }
  
  confMat <- confusionMatrix(nbClass,
                             "dataGenNew/learn/classIn.csv",
                             res$variable)
  print(confMat)
  cat("lnObservedLikelihood: "     , res$mixture$lnObservedLikelihood     , "\n",
      "lnSemiCompletedLikelihood: ", res$mixture$lnSemiCompletedLikelihood, "\n",
      "lnCompletedLikelihood: "    , res$mixture$lnCompletedLikelihood    , "\n",
      sep = "")
  
  return(res)
}