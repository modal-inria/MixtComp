library(RJsonMixtComp)

pathToData <- system.file("extdata", "data.csv", package = "RJsonMixtComp")
pathToDescriptor <- system.file("extdata", "descriptor.csv", package = "RJsonMixtComp")

#pb rang et fonctionnel
data = read.table(pathToData, sep = ";", header = TRUE)
data = data[,-c(1,3)]
write.table(data, file = "data.csv", col.names = TRUE, row.names = FALSE, quote = FALSE, sep = ";")

descriptor = read.table(pathToDescriptor, sep = ";", header = TRUE)
descriptor = descriptor[-c(1,3)]
write.table(descriptor, file = "descriptor.csv", col.names = TRUE, row.names = FALSE, quote = FALSE, sep = ";")

# resGetData <- getData(c(pathToData, pathToDescriptor)) 
resGetData <- getData(c("data.csv", "descriptor.csv")) 


resLearn <- JsonMixtCompCluster(dataList = resGetData, mcStrategy = list(nbBurnInIter = 100, 
                                                                         nbIter = 100, 
                                                                         nbGibbsBurnInIter = 100, 
                                                                         nbGibbsIter = 100, 
                                                                         parameterEdgeAuthorized = TRUE),
                                nbClass = 2, confidenceLevel = 0.95, jsonInputFile = "datalearn.json", jsonOutputFile = "reslearn.json")




# RmixtComp ---------------------------------------------------------------


library(RMixtComp)
resLearnRmixtComp <- mixtCompCluster(dataList = resGetData$lm, mcStrategy = list(nbBurnInIter = 100, 
                                                                              nbIter = 100, 
                                                                              nbGibbsBurnInIter = 100, 
                                                                              nbGibbsIter = 100, 
                                                                              parameterEdgeAuthorized = TRUE),
                                     nbClass = 2, confidenceLevel = 0.95)


resLearn$mixture
resLearnRmixtComp$mixture

#bug pas de delta dans sorties json