testNewLearn <- function(nbClass = 2,
                         nbInd = 100,
                         proportionPresent = 0.8,
                         nbBurnInIter = 100,
                         nbSampleLearn = 50,
                         nbSamplePredict = 100,
                         confidenceLevel = 0.95,
                         regen = TRUE) {  
  if (regen == TRUE) {
    dataGeneratorNewLearn(nbInd,
                          proportionPresent)
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
  if (nchar(res$mixture$warnLog) > 0) {
    warning(res$mixture$warnLog)
  }
  
  confMat <- confusionMatrix(nbClass,
                             "dataGenNew/learn/classIn.csv",
                             res$variable)
  print(confMat)
  cat("lnObservedLikelihood: "     , res$mixture$lnObservedLikelihood     , "\n",
      "lnCompletedLikelihood: "    , res$mixture$lnCompletedLikelihood    , "\n",
      sep = "")
  
  return(res)
}

testNewLearnPredict <- function(nbClass = 2,
                                nbInd = 100,
                                proportionPresent = 0.7,
                                nbBurnInIter = 100,
                                nbSampleLearn = 50,
                                nbSamplePredict = 1000,
                                confidenceLevel = 0.95,
                                regen = TRUE) {
  res <- testNewLearn(nbClass,
                      nbInd,
                      proportionPresent,
                      nbBurnInIter,
                      nbSampleLearn,
                      nbSamplePredict,
                      confidenceLevel,
                      regen)
  
  if (regen == TRUE) {
    dataGeneratorNewPredict(nbInd,
                            proportionPresent)
  }
  
  resGetData <- getData(c("dataGenNew/predict/data.csv",
                          "dataGenNew/predict/desc.csv"))
  
  # creation of strategy list
  mcStrategy <- list(nbBurnInIter = nbBurnInIter,
                     nbIter = 100,
                     nbGibbsBurnInIter = 100,
                     nbGibbsIter = 100)
  
  # launch of the MixtComp algorithm
  res <- mixtCompPredict(resGetData$lm,
                         res$variable$param,
                         mcStrategy,
                         nbClass,
                         confidenceLevel)
  if (nchar(res$mixture$warnLog) > 0) {
    warning(res$mixture$warnLog)
  }
  
  confMat <- confusionMatrix(nbClass,
                             "dataGenNew/predict/classIn.csv",
                             res$variable)
  print(confMat)
  cat("lnObservedLikelihood: "     , res$mixture$lnObservedLikelihood     , "\n",
      "lnCompletedLikelihood: "    , res$mixture$lnCompletedLikelihood    , "\n",
      sep = "")
  
  return(res)
}