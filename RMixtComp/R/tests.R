testRMC <- function (){
  lm <- getData(c("data/composite1.csv", "data/descriptor1.csv"),
                c("data/composite2.csv", "data/descriptor2.csv"))
  
  mcCluster <- getMixtCompCluster(2, 20, 100)
  mixtCompCluster(lm, mcCluster, 3)
  return(mcCluster)
}

testRMCZ <- function (){
  lm <- getDataZ(c("data/composite1.csv.gz", "data/descriptor1.csv.gz"),
                c("data/composite2.csv.gz", "data/descriptor2.csv.gz"))
  
  mcCluster <- getMixtCompCluster(2, 20, 100)
  mixtCompCluster(lm, mcCluster, 3)
  return(mcCluster)
}

testRMixtCompDev <- function ()
{
  lm <- getData(c("data/composite1.csv", "data/descriptor1.csv"),
                c("data/composite2.csv", "data/descriptor2.csv"))
  
  mcCluster <- getMixtCompCluster(2, 20, 100)
#   mixtCompCluster(lm, mcCluster, 3)
#   return(mcCluster)
}

testSimpleSampler <- function()
{
  nbSamples <- 10000
  resLine <- 1000
  meanVal <- -4.
  lower <- 0.
  upper <- -3.
  
  samples <- numeric(nbSamples)
  
  for (i in 1:nbSamples)
  {
    samples[i] <- simpleSampler(meanVal,  # mean,
                                1.,  # sd
                                lower, # infBound
                                upper, # supBound
                                F,   # lb
                                T)
  }
  
  hist(samples,
       freq = F,
       nclass = 20)
  
  lowBound <- min(samples)
  highBound <- max(samples)
  
  xVals <- seq(from = lowBound,
               to = highBound,
               length = 1000)
  yVals <- dtruncnorm(x = xVals,
                      a = lowBound,
                      b = highBound,
                      mean = meanVal,
                      sd = 1.)
  points(xVals,
         yVals,
         type="l")
}
