require(jsonlite)
require(RMixtComp)

test_mixtcomp <- function(data_training,data_test,descriptor){
  
  path_mixtcomp_learn_input <- "working_data/mixtcomp_learn_input.json"
  path_mixtcomp_predict_input <- "working_data/mixtcomp_predict_input.json"
  path_mixtcomp_learn_output <- "working_data/mixtcomp_learn_output.json"
  path_mixtcomp_predict_output <- "working_data/mixtcomp_predict_output.json"
  
  # préparation Json Mixtcomp Learn
  
  dataPath <- "working_data/data_training_test_func.csv"
  descriptorPath <-"working_data/descriptor_test_func.csv"
  write.table(data_training,file=dataPath,sep=";",row.names = F)
  write.table(descriptor,file=descriptorPath,sep=";",row.names = F)
  resGetData <- getData(c(dataPath,descriptorPath))
  for(i in 1:length(resGetData$lm)){
    resGetData$lm[[i]]$data <- as.character(resGetData$lm[[i]]$data)
  }
  mcStrategy = list(nbBurnInIter=50,nbIter=50,nbGibbsBurnInIter=50,nbGibbsIter=50,parameterEdgeAuthorized=T)
  arg_list_json <- toJSON(list(by_row = FALSE,resGetData_lm = resGetData$lm,mcStrategy=mcStrategy,nbClass = 5,confidenceLevel = 0.95,mode="learn"),auto_unbox = T)
  write(x = arg_list_json,path_mixtcomp_learn_input)
  
  # Mixtcomp Learn
  
  system(paste("/home/etienne/mixtcomp/JsonMixtcomp/bin/JsonMixtComp", path_mixtcomp_learn_input,path_mixtcomp_learn_output,sep=" "))
  
  # préparation Json Mixtcomp Predict
  
  dataPath <- "working_data/data_training_test_func.csv"
  descriptorPath <-"working_data/descriptor_test_func.csv"
  write.table(data_test,file=dataPath,sep=";",row.names = F)
  write.table(descriptor,file=descriptorPath,sep=";",row.names = F)
  resGetData <- getData(c(dataPath,descriptorPath))
  for(i in 1:length(resGetData$lm)){
    resGetData$lm[[i]]$data <- as.character(resGetData$lm[[i]]$data)
  }
  mcStrategy = list(nbBurnInIter=50,nbIter=50,nbGibbsBurnInIter=50,nbGibbsIter=50,parameterEdgeAuthorized=T)
  arg_list_json <- toJSON(list(by_row = FALSE,resGetData_lm = resGetData$lm,mcStrategy=mcStrategy,nbClass = 5,confidenceLevel = 0.95,pathParamList=path_mixtcomp_learn_output,mode="predict"),auto_unbox = T)
  write(x = arg_list_json,path_mixtcomp_predict_input)
  
  # Mixtcomp Predict
  
  system(paste("/home/etienne/mixtcomp/JsonMixtcomp/bin/JsonMixtComp", path_mixtcomp_predict_input,path_mixtcomp_predict_output,sep=" "))
  result <- fromJSON(path_mixtcomp_predict_output)
  return(result)
}

test_mixtcomp_by_row <- function(data_training,data_test,descriptor){
  
  path_mixtcomp_learn_input <- "working_data/mixtcomp_learn_input.json"
  path_mixtcomp_predict_input <- "working_data/mixtcomp_predict_input.json"
  path_mixtcomp_learn_output <- "working_data/mixtcomp_learn_output.json"
  path_mixtcomp_predict_output <- "working_data/mixtcomp_predict_output.json"
  
  # préparation Json Mixtcomp Learn
  
  dataPath <- "working_data/data_training_test_func.csv"
  descriptorPath <-"working_data/descriptor_test_func.csv"
  write.table(data_training,file=dataPath,sep=";",row.names = F)
  write.table(descriptor,file=descriptorPath,sep=";",row.names = F)
  resGetData <- getData(c(dataPath,descriptorPath))
  data <- list()
  v_data <- c()
  for(i in 1:length(resGetData$lm[[1]]$data)){
    for(j in 1:length(resGetData$lm)){
      v_data[j] <- as.character(resGetData$lm[[j]]$data[i])
    }
    data[[i]] <- v_data
  }
  for(i in 1:length(resGetData$lm)){
    resGetData$lm[[i]]$data <- NULL
  }
  
  mcStrategy = list(nbBurnInIter=50,nbIter=50,nbGibbsBurnInIter=50,nbGibbsIter=50,parameterEdgeAuthorized=T)
  arg_list_json <- toJSON(list(by_row = TRUE,
                               resGetData_lm = resGetData$lm,
                               data=data,
                               mcStrategy=mcStrategy,
                               nbClass = 5,
                               confidenceLevel = 0.95,
                               mode="learn"),
                          auto_unbox = T)
  
  write(x = arg_list_json,path_mixtcomp_learn_input)
  
  # Mixtcomp Learn
  
  system(paste("/home/etienne/mixtcomp/JsonMixtcomp/bin/JsonMixtComp", path_mixtcomp_learn_input,path_mixtcomp_learn_output,sep=" "))
  
  # préparation Json Mixtcomp Predict
  
  dataPath <- "working_data/data_training_test_func.csv"
  descriptorPath <-"working_data/descriptor_test_func.csv"
  write.table(data_test,file=dataPath,sep=";",row.names = F)
  write.table(descriptor,file=descriptorPath,sep=";",row.names = F)
  resGetData <- getData(c(dataPath,descriptorPath))
  data <- list()
  v_data <- c()
  for(i in 1:length(resGetData$lm[[1]]$data)){
    for(j in 1:length(resGetData$lm)){
      v_data[j] <- as.character(resGetData$lm[[j]]$data[i])
    }
    data[[i]] <- v_data
  }
  for(i in 1:length(resGetData$lm)){
    resGetData$lm[[i]]$data <- NULL
  }
  
  mcStrategy = list(nbBurnInIter=50,nbIter=50,nbGibbsBurnInIter=50,nbGibbsIter=50,parameterEdgeAuthorized=T)
  arg_list_json <- toJSON(list(by_row = TRUE,data=data,resGetData_lm = resGetData$lm,mcStrategy=mcStrategy,nbClass = 5,confidenceLevel = 0.95,pathParamList=path_mixtcomp_learn_output,mode="predict"),auto_unbox = T)
  write(x = arg_list_json,path_mixtcomp_predict_input)
  
  # Mixtcomp Predict
  
  system(paste("/home/etienne/mixtcomp/JsonMixtcomp/bin/JsonMixtComp", path_mixtcomp_predict_input,path_mixtcomp_predict_output,sep=" "))
  result <- fromJSON(path_mixtcomp_predict_output)
  
}

test_mixtcomp_donnees_func <- function(data_training,data_test,descriptor){
  
  path_mixtcomp_learn_input <- "/home/etienne/mixtcomp/JsonMixtcomp/test/test_fonctionnel/working_data/mixtcomp_learn_input.json"
  path_mixtcomp_predict_input <- "/home/etienne/mixtcomp/JsonMixtcomp/test/test_fonctionnel/working_data/mixtcomp_predict_input.json"
  path_mixtcomp_learn_output <- "/home/etienne/mixtcomp/JsonMixtcomp/test/test_fonctionnel/working_data/mixtcomp_learn_output.json"
  path_mixtcomp_predict_output <- "/home/etienne/mixtcomp/JsonMixtcomp/test/test_fonctionnel/working_data/mixtcomp_predict_output.json"
  
  # préparation Json Mixtcomp Learn
  
  ####### Json file preparation
  resGetData <- getData(c("working_data/data_training_test_data_fonctionnelle.csv",
                          "working_data/descriptor_data_fonctionnelle.csv"))
  for(i in 1:length(resGetData$lm)){
    resGetData$lm[[i]]$data <- as.character(resGetData$lm[[i]]$data)
  }
  mcStrategy = list(nbBurnInIter=100,nbIter=100,nbGibbsBurnInIter=10,nbGibbsIter=10,parameterEdgeAuthorized=T)
  arg_list_json <- toJSON(list(by_row=FALSE,
                               resGetData_lm = resGetData$lm,
                               mcStrategy=mcStrategy,
                               nbClass = 2,
                               confidenceLevel = 0.95,
                               mode="learn"),
                          auto_unbox = T)
  write(x = arg_list_json,"working_data/mixtcomp_input.json")
  ###### Mixtcomp run
  cmd <- paste("/home/etienne/mixtcomp/JsonMixtcomp/bin/JsonMixtComp","working_data/mixtcomp_input.json", path_mixtcomp_learn_output,sep=" ")
  system(cmd)
  result <- fromJSON(path_mixtcomp_learn_output)
  
  # préparation Json Mixtcomp Predict
  
  resGetData <- getData(c("working_data/data_training_test_data_fonctionnelle.csv",
                          "working_data/descriptor_data_fonctionnelle.csv"))
  for(i in 1:length(resGetData$lm)){
    resGetData$lm[[i]]$data <- as.character(resGetData$lm[[i]]$data)
  }
  mcStrategy = list(nbBurnInIter=100,nbIter=100,nbGibbsBurnInIter=10,nbGibbsIter=10,parameterEdgeAuthorized=T)
  arg_list_json <- toJSON(list(by_row=FALSE,
                               resGetData_lm = resGetData$lm,
                               mcStrategy=mcStrategy,
                               nbClass = 2,
                               confidenceLevel = 0.95,
                               mode="predict",
                               pathParamList=path_mixtcomp_learn_output),
                          auto_unbox = T)
  write(x = arg_list_json,"working_data/mixtcomp_predict_input.json")
  
  # Mixtcomp Predict
  
  system(paste("/home/etienne/mixtcomp/JsonMixtcomp/bin/JsonMixtComp", "working_data/mixtcomp_predict_input.json",path_mixtcomp_predict_output,sep=" "))
  
  detach("package:RMixtComp", unload=TRUE)  
  require(RMixtComp)
  res_learn <- mixtCompCluster(dataList = resGetData$lm,mcStrategy = mcStrategy,nbClass = 2,confidenceLevel = 0.95)
  res_pred <- mixtCompPredict(dataList = resGetData$lm,paramList = res_learn$variable$param,mcStrategy = mcStrategy,nbClass = 2,confidenceLevel = 0.95)
  
  result <- fromJSON(path_mixtcomp_predict_output)
  
}