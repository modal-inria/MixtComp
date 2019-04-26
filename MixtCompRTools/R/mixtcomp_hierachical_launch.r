

check_strategy <- function(strategy, resGetData) {

  if(!is.null(strategy$var)){
    vars_in_resGetData = unlist(lapply(resGetData$lm,function(x){x$id}))
    print(vars_in_resGetData)
    print(strategy)
    if(!(strategy$var %in% vars_in_resGetData)){
      return(list("error",paste0("Variable ",strategy$var," indicated in the strategy doesn't exist in the dataset")))
    }

    idx_var = which(strategy$var == vars_in_resGetData)
    var_strategy_type = resGetData$lm[[idx_var]]$model
    if(var_strategy_type != "Categorical_pjk"){
      return(list("error",paste0("Variable ",strategy$var," is not indicated as Categorical in the descriptor file")))
    }
    data_var = resGetData$lm[[idx_var]]$data
    data_var = data_var[which(data_var!="?")]
    print(paste0("length data : ",length(resGetData$lm[[1]]$data)))
    print(paste0("strategy nInd :", strategy$threshold_nInd))
    print(paste0("purity : ",(max(table(data_var) / length(data_var)))))
    print(paste0("strategy purity : ", strategy$threshold_purity))

    if (max(table(data_var) / length(data_var)) > strategy$threshold_purity){return(list("","Strategy's terms not fulfilled"))}
  }
  if( length(resGetData$lm[[1]]$data) < strategy$threshold_nInd ) {return(list("","Strategy's terms not fulfilled"))}

  return(list("",""))
}

# During the expand phase, cut data according to clusters and copy to subfolder
#
# @param dir A String
#
extract_data_per_cluster <- function(dir) {
  MixtCompOutput <- fromJSON(paste0(dir, "/mixtcomp_output.json"))
  data <- read.csv(paste0(dir, "/data_mixtcomp.csv"),
                   sep = ";", header = TRUE)

  oldMC <- is.null(MixtCompOutput$algo)
  nClass <- ifelse(oldMC, MixtCompOutput$mixture$nbCluster, MixtCompOutput$algo$nClass)

  for (i in 1:nClass) {
    if(oldMC)
      idx_cluster <- which(MixtCompOutput$variable$data$z_class$completed == i)
    else
      idx_cluster <- which(MixtCompOutput$variable$data$z_class$completed$data == i)

    data_cluster <- data[idx_cluster, ]
    subdir <- paste0(dir, "/subcluster_", i)
    print(paste0(subdir, "/data_mixtcomp.csv"))

    write.table(
      data_cluster,
      paste0(subdir, "/data_mixtcomp.csv"),
      sep = ";",
      row.names = FALSE
    )
    file.copy(paste0(dir, "/descriptor.csv"),
              paste0(subdir, "/descriptor.csv"))
  }
}

# Launch Mixtcomp on a data_mixtcomp.csv and descriptor.csv files contained in
# the directory given by the dir param and with nClass number of clusters
#
# @param dir A String
# @param nClass number of classes to compute at each node
# @param strategy (optional) List of three elements which defines the strategy to follow during the hierarchical clustering.
# var (name of the categorical variable on which to base the strategy), threshold_nInd (the minimal number of observations in the dataset under which the subclustering cannot be accepted)
# threshold_purity (the maximal purity observed in the dataset above which the subclustering is not necessary). At each hierarchical iterations these elements will be tested.
# @param mcStrategy (optional) Additional parameters for mixtcomp launch (nbBurnInIter, nbIter nbGibbsBurnInIter, nbGibbsIter)
#
#
launch_mixtcomp <- function(dir, nClass, strategy = NULL, mcStrategy = NULL) {
  # Check if JsonMixtComp executable is available
  pathToJsonMixtComp <- system.file("exe", "JsonMixtComp", package = "MixtCompRTools")

  path_input <- paste0(dir, "/mixtcomp_input.json")
  path_output <- paste0(dir, "/mixtcomp_output.json")

  resGetData <- getData(c(
    paste0(dir, "/data_mixtcomp.csv"),
    paste0(dir, "/descriptor.csv")
  ))

  # Check the conditions on the number of Observations min and purity of the categorical variable
  result_strategy = check_strategy(strategy = strategy,resGetData = resGetData)
  if(result_strategy[[1]]=="error"){stop(result_strategy[[2]])}
  if(result_strategy[[2]]!=""){print(result_strategy[[2]]);return(result_strategy[[2]])}


  for (i in 1:length(resGetData$lm)) {
    resGetData$lm[[i]]$data <- as.character(resGetData$lm[[i]]$data)
  }

  if(is.null(mcStrategy)){
    mcStrategy = list(
      nbBurnInIter = 15,
      nbIter = 15,
      nbGibbsBurnInIter = 15,
      nbGibbsIter = 15,
      ratioInitialization = 0.1,
      nSemTry = 10
    )
  } else {
    mcStrategy$ratioInitialization = ifelse(is.null(mcStrategy$ratioInitialization), 0.1, mcStrategy$ratioInitialization)
    mcStrategy$nSemTry = ifelse(is.null(mcStrategy$nSemTry), 10, mcStrategy$nSemTry)
  }

  arg_list_json <- toJSON(
    list(
      by_row = FALSE,
      resGetData_lm = resGetData$lm,
      mcStrategy = mcStrategy,
      nbClass = nClass,
      confidenceLevel = 0.95,
      mode = "learn"
    ),
    auto_unbox = TRUE
  )
  write(x = arg_list_json, path_input)

  ###### Mixtcomp run

  cmd <- paste(pathToJsonMixtComp,
               path_input,
               path_output)
  system(cmd)

}

# Launch Mixtcomp on a data_mixtcomp.csv and descriptor.csv files contained in
# the directory given by the dir param and with nClass number of clusters
#
# @param param_dir A string defining the path of the directory in which to find the parameters hierarchical tree on which the hierarchical predict will be based
# @param test_dir A String
# @param nClass Positive intege
# @param mcStrategy Additional parameters for mixtcomp launch (nbBurnInIter, nbIter nbGibbsBurnInIter, nbGibbsIter)
#
launch_mixtcomp_predict <- function(param_dir, test_dir, nClass, mcStrategy) {
  # Check if JsonMixtComp executable is available
  pathToJsonMixtComp <- system.file("exe", "JsonMixtComp", package = "MixtCompRTools")

  path_input <- paste0(test_dir, "/mixtcomp_input.json")
  path_output <- paste0(test_dir, "/mixtcomp_output.json")

  resGetData <- getData(c(
    paste0(test_dir,"/data_predict.csv"),
    paste0(param_dir, "/descriptor.csv")
  ))

  for (i in 1:length(resGetData$lm)) {
    resGetData$lm[[i]]$data <- as.character(resGetData$lm[[i]]$data)
  }

  if(is.null(mcStrategy)){
    mcStrategy = list(
      nbBurnInIter = 15,
      nbIter = 15,
      nbGibbsBurnInIter = 15,
      nbGibbsIter = 15,
      ratioInitialization = 0.1,
      nSemTry = 10
    )
  } else {
    mcStrategy$ratioInitialization = ifelse(is.null(mcStrategy$ratioInitialization), 0.1, mcStrategy$ratioInitialization)
    mcStrategy$nSemTry = ifelse(is.null(mcStrategy$nSemTry), 10, mcStrategy$nSemTry)
  }


  arg_list_json <-
    toJSON(
      list(
        by_row = FALSE,
        resGetData_lm = resGetData$lm,
        mcStrategy = mcStrategy,
        nbClass = nClass,
        confidenceLevel = 0.95,
        pathParamList = paste0(param_dir,"/mixtcomp_output.json"),
        mode = "predict"
      ),
      auto_unbox = TRUE
    )

  write(x = arg_list_json, path_input)

  ###### Mixtcomp run

  cmd <- paste(pathToJsonMixtComp,
               path_input,
               path_output)
  system(cmd)

}

# Create Subdirectories during the expand phase
#
# @param dir A directory
# @param nClass A positive integer
#
#
create_subdirectories <- function(dir, nClass) {
  if (!dir.exists(dir)) {
    stop(
      "Directory path given in input doesn't exist.
      This function should not be used out of the scope of the expand function."
    )
  }
  if ((nClass < 1) & (nClass > 10)) {
    stop("nClass parameter should be positive and not greater than 10.")
  }

  for (i in 1:nClass) {
    dir.create(paste0(dir, "/subcluster_", i))
  }

}

# Launch an expand phase : if the directory doesn't contain any subdirectories,
# cut the data in clusters according to the clusters estimated by Mixtcomp and given
# by an output contained in the dir parameter, create a set of subclusters, copy the data
# into them and launch Mixtcomp in each of them.
#
# @param dir The path of the directory to expand
# @param nClass The number of class for the Mixtcomp launches on the next subclusters
# @param strategy (optional) List of three elements which defines the strategy to follow during the hierarchical clustering.
# var (name of the categorical variable on which to base the strategy), threshold_nInd (the minimal number of observations in the dataset under which the subclustering cannot be accepted)
# threshold_purity (the maximal purity observed in the dataset above which the subclustering is not necessary). At each hierarchical iterations these elements will be tested.
# @param mcStrategy (optional) Additional parameters for mixtcomp launch (nbBurnInIter, nbIter nbGibbsBurnInIter, nbGibbsIter)
# @param nCore
#
#
expand <- function(dir, nClass, strategy = NULL, mcStrategy = NULL, nCore = 1) {
  existing_subdirs_in_current_dir <- list.dirs(dir, recursive = FALSE)
  print("#######################")

  print(dir)
  print(existing_subdirs_in_current_dir)
  # If the directory already contains subdirectories, expand the subdirs
  if (length(existing_subdirs_in_current_dir) > 0) {
    for (subdir in existing_subdirs_in_current_dir) {
      expand(subdir, nClass, strategy, mcStrategy = mcStrategy, nCore = nCore)
    }
  } else {
    # If no subdirectories exist, create them, extract data into them and launch mixtcomp.
    if(!file.exists(paste0(dir, "/mixtcomp_output.json"))){return()}

    MixtCompOutput <- fromJSON(paste0(dir, "/mixtcomp_output.json"))
    oldMC <- is.null(MixtCompOutput$algo)
    if(!is.null(MixtCompOutput$warnLog) || (!is.null(MixtCompOutput$mixture$warnLog) && (MixtCompOutput$mixture$warnLog != ""))){return()}

    nbCluster <- ifelse(is.null(MixtCompOutput$mixture$nbCluster), MixtCompOutput$algo$nClass, MixtCompOutput$mixture$nbCluster)
    create_subdirectories(dir, nbCluster)
    extract_data_per_cluster(dir)
    subdirs = paste0(dir,
                     "/subcluster_",
                     1:nbCluster)

    # execute mixtcomp in parallel mode or not
    launchFunc <- launch_mixtcomp
    if(!oldMC)
      launchFunc = launch_mixtcompNew
    if (nCore == 1) {
      for (subdir in subdirs) {
        launchFunc(subdir, nClass, strategy = strategy, mcStrategy = mcStrategy)
      }
    } else {
      cl <- makeCluster(length(subdirs))
      registerDoParallel(cl)
      foreach(subdir = subdirs, .packages = 'MixtCompRTools') %dopar% {
        launchFunc(subdir, nClass, strategy = strategy, mcStrategy = mcStrategy)
      }
      stopCluster(cl)
    }
  }
}


#' Launch the Hierarchical Mixtcomp.
#'
#' @param data_path data file path
#' @param descriptor_path descriptor file path
#' @param nClass Integer between 0 and 10: number of classes to compute at each node
#' @param depth depth of the hierarchy
#' @param output_dir (optional) if not null, the results will be written in this directory.
#' If Null, a directory with the name of the dataset will be created and used as output directory instead.
#' @param strategy (optional) List of three elements which defines the strategy to follow during the hierarchical clustering.
#' var (name of the categorical variable on which to base the strategy), threshold_nInd (the minimal number of observations in the dataset under which the subclustering cannot be accepted)
#' threshold_purity (the maximal purity observed in the dataset above which the subclustering is not necessary). At each hierarchical iterations these elements will be tested.
#' @param mcStrategy (optional) Additional parameters for mixtcomp launch list containing elements: (nbBurnInIter, nbIter, nbGibbsBurnInIter, nbGibbsIter). Default is 15 for all parameters.
#' @param nCore number of cores for parallelization
#' @param oldMC if TRUE use old MixtComp, else use JMixtComp
#'
#' @details Please clean the output_dir after a run fails
#'
#' @examples
#' \dontrun{
#' data_path <- "res/func/dat/dat.csv"
#' dataTestPath <- "res/func/dat/data_predict.csv"
#' descriptor_path <- "res/func/dat/desc.csv"
#'
#' res <- launch_Mixtcomp_Hierarchical(data_path, descriptor_path, nClass = 3, depth = 5,
#'                                    output_dir = "res/func/out/",
#'                                    strategy = list(var = NULL, threshold_nInd = 10,
#'                                                    threshold_purity = 0.95), nCore = 3)
#'
#'
#' resPred <- launch_Mixtcomp_Hierarchical_predict(data_path = dataTestPath,
#'                                                 param_dir = "res/func/out/dat/",
#'                                                 output_dir = "res/func/outPred/"
#'
#' # get partition
#' partition <- aggregate_clusters("res/func/outPred/data_predict/")
#'
#' # get completed data
#' compl <- aggregate_completed("res/func/outPred/data_predict/", var = "Degradation_type")
#'
#' }
#'
#' @seealso \link{launch_Mixtcomp_Hierarchical_predict} \link{aggregate_clusters} \link{prune_hierarchy}
#'
#' @export
#'
#' @author Etienne Goffinet
launch_Mixtcomp_Hierarchical <- function(data_path, descriptor_path, nClass, depth, output_dir = NULL,
                                         strategy = list(var = NULL, threshold_nInd = 1, threshold_purity = 2),
                                         mcStrategy = NULL, nCore = 1, oldMC = TRUE) {
  # Get directory of the data_path

  if (is.null(output_dir)) {
    output_dir = dirname(data_path)
  }
  data_name = basename(data_path)
  # If output dirs doesnt' exist, create it
  newDir = paste0(output_dir, "/", strsplit(data_name, ".csv")[[1]])
  print(newDir)

  if (!dir.exists(newDir)) {
    dir.create(newDir)
    file.copy(descriptor_path, paste0(newDir, "/descriptor.csv"))
    file.copy(data_path, paste0(newDir, "/data_mixtcomp.csv"))
    if(oldMC)
      launch_mixtcomp(dir = newDir, nClass = nClass, strategy = strategy, mcStrategy = mcStrategy)
    else
      launch_mixtcompNew(dir = newDir, nClass = nClass, strategy = strategy, mcStrategy = mcStrategy)

  }
  # run expand on it
  if (depth == 0) {
    cat("Hierarchical clustering completed !\n")
    return()
  }

  for (i in 1:depth) {
    expand(dir = newDir, nClass = nClass, strategy = strategy, mcStrategy = mcStrategy, nCore = nCore)
  }
  cat("Hierarchical clustering completed !\n")
  return()
}

#' Launch the Mixtcomp prediction based on an existing hierarchical cluster
#'
#' @param data_path data file path
#' @param param_dir A string defining the path of the directory in which to fint the parameters hierarchical tree on which the hierarchical predict will be based
#' @param output_dir (optional) if not null, the results will be written in this directory.
#' If Null, a directory with the name of the dataset will be created and used as output directory instead.
#' @param mcStrategy (optional) Additional parameters for mixtcomp launch list containing elements: (nbBurnInIter, nbIter, nbGibbsBurnInIter, nbGibbsIter)
#'
#' @export
#'
#' @examples
#' \dontrun{
#' data_path <- "res/func/dat/dat.csv"
#' dataTestPath <- "res/func/dat/data_predict.csv"
#' descriptor_path <- "res/func/dat/desc.csv"
#'
#' res <- launch_Mixtcomp_Hierarchical(data_path, descriptor_path, nClass = 3, depth = 5,
#'                                    output_dir = "res/func/out/",
#'                                    strategy = list(var = NULL, threshold_nInd = 10,
#'                                                    threshold_purity = 0.95), nCore = 3)
#'
#'
#' resPred <- launch_Mixtcomp_Hierarchical_predict(data_path = dataTestPath,
#'                                                 param_dir = "res/func/out/dat/",
#'                                                 output_dir = "res/func/outPred/"
#'
#' # get partition
#' partition <- aggregate_clusters("res/func/outPred/data_predict/")
#'
#' # get completed data
#' compl <- aggregate_completed("res/func/outPred/data_predict/", var = "Degradation_type")
#'
#' }
#'
#' @seealso \link{launch_Mixtcomp_Hierarchical} \link{aggregate_clusters}
#'
#' @author Etienne Goffinet
launch_Mixtcomp_Hierarchical_predict <- function(data_path, param_dir, output_dir = NULL, mcStrategy = NULL) {
  # oldMC is not a parameter, it is guessed from object saved in param_dir

  # Get directory of the data_path
  if (is.null(output_dir)) {
    output_dir = dirname(data_path)
  }
  data_name = basename(data_path)
  # If output dirs doesnt' exist, create it
  newDir = paste0(output_dir, "/", strsplit(data_name, ".csv")[[1]])
  print(newDir)
  if (!dir.exists(newDir)) {
    dir.create(newDir)
    file.copy(data_path, paste0(newDir, "/data_predict.csv"))
  }

  expand_predict(newDir, param_dir, mcStrategy = mcStrategy)
}


expand_predict <- function(test_dir, param_dir, mcStrategy = NULL) {

  print(paste0("test_dir : ", test_dir))

  print(paste0("param_dir : ", param_dir))

  output_learn = fromJSON(paste0(param_dir,"/mixtcomp_output.json"))
  oldMC <- is.null(output_learn$algo)

  nbCluster <- ifelse(oldMC, output_learn$mixture$nbCluster, output_learn$algo$nClass)

  if(oldMC)
    launch_mixtcomp_predict(param_dir = param_dir,test_dir =  test_dir, nbCluster, mcStrategy)
  else
    launch_mixtcompNew_predict(param_dir = param_dir,test_dir =  test_dir, nbCluster, mcStrategy)

  data_predict = read.table(paste0(test_dir,"/data_predict.csv"), sep = ";", stringsAsFactors = FALSE, header = TRUE)
  subdirs_list = list.dirs(recursive = FALSE, param_dir)

  if(length(subdirs_list) > 0){
    output_predict = fromJSON(paste0(test_dir,"/mixtcomp_output.json"))

    if(oldMC)
      completed_clusters = output_predict$variable$data$z_class$completed
    else
      completed_clusters = output_predict$variable$data$z_class$completed$data


    for(cluster in unique(completed_clusters)){
      next_param_dir = paste0(param_dir,"/subcluster_",cluster)

      if(file.exists(paste0(next_param_dir,"/mixtcomp_output.json"))){
        next_output_learn = fromJSON(paste0(next_param_dir,"/mixtcomp_output.json"))

        if(is.null(next_output_learn$warnLog) && (is.null(next_output_learn$mixture$warnLog) || (next_output_learn$mixture$warnLog == ""))) {
          next_test_dir = paste0(test_dir,"/subcluster_",cluster)
          data_next_predict = data_predict[which(completed_clusters == cluster),]
          dir.create(next_test_dir)
          write.table(data_next_predict, paste0(next_test_dir,"/data_predict.csv"), sep = ";", na = "", row.names = FALSE)
          expand_predict(test_dir = next_test_dir, param_dir = next_param_dir)
        }
      }
    }
  }
}

#' Prune a hierarchical clustering. The pruning can be done according to three different strategies
#'
#'  - The purity of the clusters : if a cluster is purer than a certain threshold then the subclusters are not useful
#'  - The size of the cluster : below a certain size, a cluster should not have been subclusterized.
#'  - The quality of the completion : Is the global completion more accurate with this subclusterization ? Aims at avoiding overfitting.
#'
#'  These strategies can be cumulated.
#'
#' @param dir A string, the path of the hierarchical clustering to prune
#' @param purity_strategy a list of two elements (var and threshold), that defines which variable purity to check and according to which threshold (double between 0 and 1)
#' @param cluster_size_strategy a list of one element (threshold) that define the cluster size threshold.
#'
#' @export
#'
#' @author Etienne Goffinet
prune_hierarchy <- function(dir, purity_strategy = list(var = NULL, threshold = NULL), cluster_size_strategy = list(threshold = NULL)) {

  list_subdir = list.dirs(path = dir, recursive = FALSE, full.names = TRUE)

  if(length(list_subdir)>0){
    # check the strategies

    output = fromJSON(paste0(dir,"/mixtcomp_output.json"))

    # Apply purity strategy
    if(all(!is.null(unlist(purity_strategy)))){
      purity = max(table(output$variable$data[[purity_strategy$var]]$completed)/output$mixture$nbInd)
      if(purity > purity_strategy$threshold){
        unlink(list_subdir, recursive = TRUE, force = TRUE)
      }
    }

    # Apply cluster size strategy
    if(!is.null(cluster_size_strategy$threshold)){
      if(output$mixture$nbInd < cluster_size_strategy$threshold){
        unlink(list_subdir,recursive = TRUE,force = TRUE)
      }
    }

    # Else, try to the prune the subdirectories
    list_subdir = list.dirs(path = dir, recursive = FALSE, full.names = TRUE)
    if(length(list_subdir)>0){
      for (subdir in list_subdir) {
        prune_hierarchy(
          subdir,
          purity_strategy = purity_strategy,
          cluster_size_strategy = cluster_size_strategy
        )
      }
    }
  }
}

