# MixtComp version 4 - july 2019
# Copyright (C) Inria - Université de Lille - CNRS 

library(RMixtComp)

set.seed(42)

var <- list()
var$z_class <- RMixtCompIO:::zParam()
var$Poisson1 <- RMixtCompIO:::poissonParam("Poisson1")
var$Gaussian1 <- RMixtCompIO:::gaussianParam("Gaussian1")
var$Categorical1 <- RMixtCompIO:::categoricalParam1("Categorical1")
var$nBinom1 <- RMixtCompIO:::nBinomParamRandom("nBinom1")
var$Weibull1 <- RMixtCompIO:::weibullParam("Weibull1")
var$Functional1 <- RMixtCompIO:::functionalInterPolyParam("Functional1")
var$FunctionalSharedAlpha1 <- RMixtCompIO:::functionalSharedAlphaInterPolyParam("FunctionalSharedAlpha1")
var$Rank1 <- RMixtCompIO:::rankParam("Rank1")

resGenLearn <- RMixtCompIO:::dataGeneratorNewIO(200, 0.9, var)
resGenPredict <- RMixtCompIO:::dataGeneratorNewIO(100, 0.9, var)

simDataLearn <- list(data.frame = as.data.frame(do.call(cbind, resGenLearn$data)), matrix = do.call(cbind, resGenLearn$data), list = resGenLearn$data)
simDataLearn$data.frame$z_class = resGenLearn$z
simDataLearn$list$z_class = resGenLearn$z
simDataLearn$matrix[,1] = resGenLearn$z

simDataPredict <- list(data.frame = as.data.frame(do.call(cbind, resGenPredict$data[-1])), matrix = do.call(cbind, resGenPredict$data[-1]), list = resGenPredict$data[-1])
simModel <- list(supervised = resGenLearn$descriptor, unsupervised = resGenLearn$descriptor[-1])

simData <- list(dataLearn = simDataLearn, dataPredict = simDataPredict, model = simModel)
save(simData, file = "data/simData.rda", compress = TRUE)
