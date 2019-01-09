library(RMixtComp)

set.seed(42)

var <- list()
var$z_class <- RMixtComp:::zParam()
var$Poisson1 <- RMixtComp:::poissonParam("Poisson1")
var$Gaussian1 <- RMixtComp:::gaussianParam("Gaussian1")
var$Categorical1 <- RMixtComp:::categoricalParam1("Categorical1")
var$nBinom1 <- RMixtComp:::nBinomParamRandom("nBinom1")
var$Weibull1 <- RMixtComp:::weibullParam("Weibull1")
var$Functional1 <- RMixtComp:::functionalInterPolyParam("Functional1")
var$FunctionalSharedAlpha1 <- RMixtComp:::functionalSharedAlphaInterPolyParam("FunctionalSharedAlpha1")
var$Rank1 <- RMixtComp:::rankParam("Rank1")

resGenLearn <- RMixtComp:::dataGeneratorNewIO(200, 0.9, var)
resGenPredict <- RMixtComp:::dataGeneratorNewIO(100, 0.9, var)

simDataLearn <- list(data.frame = as.data.frame(do.call(cbind, resGenLearn$data)), matrix = do.call(cbind, resGenLearn$data), list = resGenLearn$data)
simDataLearn$data.frame$z_class = resGenLearn$z
simDataLearn$list$z_class = resGenLearn$z
simDataLearn$matrix[,1] = resGenLearn$z

simDataPredict <- list(data.frame = as.data.frame(do.call(cbind, resGenPredict$data[-1])), matrix = do.call(cbind, resGenPredict$data[-1]), list = resGenPredict$data[-1])
simModel <- list(supervised = resGenLearn$descriptor, unsupervised = resGenLearn$descriptor[-1])

simData <- list(dataLearn = simDataLearn, dataPredict = simDataPredict, model = simModel)
save(simData, file = "data/simData.rda", compress = TRUE)
