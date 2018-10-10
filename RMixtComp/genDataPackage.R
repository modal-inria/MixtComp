library(RMixtComp)

set.seed(42)

var <- list()
var$z_class <- zParam()
var$Poisson1 <- poissonParam("Poisson1")
var$Gaussian1 <- gaussianParam("Gaussian1")
var$Categorical1 <- categoricalParam1("Categorical1")
var$nBinom1 <- nBinomParamRandom("nBinom1")
var$Weibull1 <- weibullParam("Weibull1")
var$Functional1 <- functionalInterPolyParam("Functional1")
var$functionalSharedAlpha1 <- functionalSharedAlphaInterPolyParam("functionalSharedAlpha1")
var$Rank1 <- rankParam("Rank1")

resGenLearn <- dataGeneratorNewIO(200, 0.9, var)
resGenPredict <- dataGeneratorNewIO(100, 0.9, var)

simDataLearn <- list(data.frame = as.data.frame(do.call(cbind, resGenLearn$data)), matrix = do.call(cbind, resGenLearn$data), list = resGenLearn$data)
simDataPredict <- list(data.frame = as.data.frame(do.call(cbind, resGenPredict$data[-1])), matrix = do.call(cbind, resGenPredict$data[-1]), list = resGenPredict$data[-1])
simDesc <- list(supervised = resGenLearn$descriptor, unsupervised = resGenLearn$descriptor[-1])

save(simDataLearn, simDataPredict, simDesc, file = "data/simData.rda", compress = TRUE)