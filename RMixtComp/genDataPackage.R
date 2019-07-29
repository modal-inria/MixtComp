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
