
#' During the expand phase, cut data according to clusters and copy to subfolder
#'
#' @param dir A String
#'
#' @return void
#'
#' @examples extract_data_per_cluster("data/data_mixtcomp/x.csv")
extract_data_per_cluster <- function(dir) {
  MixtCompOutput <-
    jsonlite::fromJSON(paste0(dir, "/mixtcomp_output.json"))
  data <- utils::read.csv(paste0(dir, "/data_mixtcomp.csv"),
                          sep = ";",
                          header = T)

  for (i in 1:MixtCompOutput$mixture$nbCluster) {
    idx_cluster <-
      which(MixtCompOutput$variable$data$z_class$completed == i)
    data_cluster <- data[idx_cluster, ]
    subdir <- paste0(dir, "/subcluster_", i)
    print(paste0(subdir, "/data_mixtcomp.csv"))

    utils::write.table(
      data_cluster,
      paste0(subdir, "/data_mixtcomp.csv"),
      sep = ";",
      row.names = F
    )
    file.copy(paste0(dir, "/descriptor.csv"),
              paste0(subdir, "/descriptor.csv"))
  }
}

#' Launch Mixtcomp on a data_mixtcomp.csv and descriptor.csv files contained in
#' the directory given by the dir param and with nClass number of clusters
#'
#' @param dir A String
#' @param nClass Positive intege
#'
#' @return void
#'
#' @examples
#' \dontrun {
#' launch_mixtcomp("data/data_mixtcomp",3)
#' }
launch_mixtcomp <- function(dir, nClass, strategy) {
  # Check if JsonMixtComp executable is available
  paths = Sys.getenv("PATH")
  if (!any(file.exists(paste0(strsplit(paths, ":")[[1]], "/JsonMixtComp")))) {
    stop(
      "JsonMixtComp executable cannot be found.
      In order to be callable, 'JsonMixtComp' must be stored in any of the folder designated by the paths contained in the environement variable PATH"
    )
  }
  path_input <- paste0(dir, "/mixtcomp_input.json")
  path_output <- paste0(dir, "/mixtcomp_output.json")

  resGetData <- getData(c(
    paste0(dir, "/data_mixtcomp.csv"),
    paste0(dir, "/descriptor.csv")
  ))

  # Check the conditions on the number of Observations min and purity of the categorical variable
  print(strategy)

  data_categorical_strategy = resGetData$lm[[which(unlist(lapply(resGetData$lm,function(x){x$id}))==strategy$var)]]$data
  data_categorical_strategy = data_categorical_strategy[which(data_categorical_strategy!="?")]

  print(paste0("length data : ",length(resGetData$lm[[1]]$data)))
  print(paste0("strategy nInd :", strategy$threshold_nInd))
  print(paste0("purity : ",(max(table(data_categorical_strategy) / length(data_categorical_strategy)))))
  print(paste0("strategy purity : ", strategy$threshold_purity))

  if ((length(resGetData$lm[[1]]$data) < strategy$threshold_nInd) |
      (max(
        table(data_categorical_strategy) / length(data_categorical_strategy)
      ) > strategy$threshold_purity)) {return("Strategy's terms not fulfilled")}



  for (i in 1:length(resGetData$lm)) {
    resGetData$lm[[i]]$data <- as.character(resGetData$lm[[i]]$data)
  }

  mcStrategy = list(
    nbBurnInIter = 15,
    nbIter = 15,
    nbGibbsBurnInIter = 15,
    nbGibbsIter = 15,
    nInitPerClass = length(resGetData$lm[[i]]$data),
    nSemTry = 10
  )

  arg_list_json <- jsonlite::toJSON(
    list(
      by_row = FALSE,
      resGetData_lm = resGetData$lm,
      mcStrategy = mcStrategy,
      nbClass = nClass,
      confidenceLevel = 0.95,
      mode = "learn"
    ),
    auto_unbox = T
  )
  write(x = arg_list_json, path_input)

  ###### Mixtcomp run

  cmd <-
    paste("JsonMixtComp",
          path_input,
          path_output)
  system(cmd)

}

#' Launch Mixtcomp on a data_mixtcomp.csv and descriptor.csv files contained in
#' the directory given by the dir param and with nClass number of clusters
#'
#' @param dir A String
#' @param nClass Positive intege
#'
#' @return void
#'
#' @examples
#' \dontrun {
#' launch_mixtcomp("data/data_mixtcomp",3)
#' }
launch_mixtcomp_predict <- function(param_dir, test_dir, nClass) {
  # Check if JsonMixtComp executable is available
  paths = Sys.getenv("PATH")
  if (!any(file.exists(paste0(strsplit(paths, ":")[[1]], "/JsonMixtComp")))) {
    stop(
      "JsonMixtComp executable cannot be found.
      In order to be callable, 'JsonMixtComp' must be stored in any of the folder designated by the paths contained in the environement variable PATH"
    )
  }

  path_input <- paste0(test_dir, "/mixtcomp_input.json")
  path_output <- paste0(test_dir, "/mixtcomp_output.json")

  resGetData <- getData(c(
    paste0(test_dir,"/data_predict.csv"),
    paste0(param_dir, "/descriptor.csv")
  ))

  for (i in 1:length(resGetData$lm)) {
    resGetData$lm[[i]]$data <- as.character(resGetData$lm[[i]]$data)
  }

  mcStrategy = list(
    nbBurnInIter = 15,
    nbIter = 15,
    nbGibbsBurnInIter = 15,
    nbGibbsIter = 15,
    nInitPerClass = length(resGetData$lm[[i]]$data) / nClass,
    nSemTry = 20
  )

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
      auto_unbox = T
    )

  write(x = arg_list_json, path_input)

  ###### Mixtcomp run

  cmd <-
    paste("JsonMixtComp",
          path_input,
          path_output)
  system(cmd)

}

#' Create Subdirectories during the expand phase
#'
#' @param dir A directory
#' @param nClass A positive integer
#'
#' @return void
#'
#' @examples create_subdirectories(getwd(),2)
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

#' Launch an expand phase : if the directory doesn't contain any subdirectories,
#' cut the data in clusters according to the clusters estimated by Mixtcomp and given
#' by an output contained in the dir parameter, create a set of subclusters, copy the data
#' into them and launch Mixtcomp in each of them.
#'
#' @param dir The path of the directory to expand
#' @param nClass The number of class for the Mixtcomp launches on the next subclusters
#'
#' @return void
#'
#' @examples expand()
expand <- function(dir, nClass, strategy) {
  parallel = TRUE
  existing_subdirs_in_current_dir <- list.dirs(dir, recursive = F)
  print("#######################")

  print(dir)
  print(existing_subdirs_in_current_dir)
  # If the directory already contains subdirectories, expand the subdirs
  if (length(existing_subdirs_in_current_dir) > 0) {
    for (subdir in existing_subdirs_in_current_dir) {
      expand(subdir, nClass, strategy)
    }
  } else {
    # If no subdirectories exist, create them, extract data into them and launch mixtcomp.
    if(!file.exists(paste0(dir, "/mixtcomp_output.json"))){return()}

    MixtCompOutput <-
      jsonlite::fromJSON(paste0(dir, "/mixtcomp_output.json"))
    if(MixtCompOutput$mixture$warnLog!=""){return()}

    create_subdirectories(dir, MixtCompOutput$mixture$nbCluster)
    extract_data_per_cluster(dir)
    subdirs = paste0(dir,
                     "/subcluster_",
                     1:MixtCompOutput$mixture$nbCluster)

    # execute mixtcomp in parallel mode or not
    if (parallel == FALSE) {
      for (subdir in subdirs) {
        launch_mixtcomp(subdir, nClass,strategy)
      }
    } else {
      cl <- makeCluster(length(subdirs))
      registerDoParallel(cl)
      foreach(subdir = subdirs, .packages = 'mixtcompRTools') %dopar% {
        launch_mixtcomp(subdir, nClass,strategy)
      }
      stopCluster(cl)
    }
  }
}


#' Launch the Hierarchical Mixtcomp.
#'
#' @param data_path A string
#' @param descriptor_path A string
#' @param nClass Integer between 0 and 10
#' @param depth Positive integer
#' @param output_dir (optional) if not null, the results will be written in this directory.
#' If Null, a directory with the name of the dataset will be created and used as output directory instead.
#'
#' @return void
#' @export
#'
#' @examples launch_Mixtcomp_Hierarchical("data/data.csv","data/descriptor.csv",3,3)
#' @author Etienne Goffinet
launch_Mixtcomp_Hierarchical <-
  function(data_path,
           descriptor_path,
           nClass,
           depth,
           output_dir = NULL,
           strategy = list(var = 1,
                           threshold_nInd = 1,
                           threshold_purity = 2)) {
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
      launch_mixtcomp(dir = newDir, nClass = nClass, strategy=strategy)
    }
    # run expand on it
    if (depth == 0) {
      cat("Hierarchical clustering complete !")
      return()
    }

    for (i in 1:depth) {
      expand(dir = newDir, nClass, strategy)
    }
    cat("Hierarchical clustering complete !")

  }

#' Launch the Mixtcomp prediction based on an existing hierarchical cluster
#'
#' @param data_path A string
#' @param descriptor_path A string
#' @param nClass Integer between 0 and 10
#' @param depth Positive integer
#' @param output_dir (optional) if not null, the results will be written in this directory.
#' If Null, a directory with the name of the dataset will be created and used as output directory instead.
#'
#' @return void
#' @export
#'
#' @examples launch_Mixtcomp_Hierarchical("data/data.csv","data/descriptor.csv",3,3)
#' @author Etienne Goffinet
launch_Mixtcomp_Hierarchical_predict <-
  function(data_path,
           param_dir,
           output_dir = NULL) {

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

    expand_predict(newDir,param_dir)
  }


expand_predict <-
  function(test_dir,
           param_dir) {

    print(paste0("test_dir : ",test_dir))

    print(paste0("param_dir : ",param_dir))

    output_learn = fromJSON(paste0(param_dir,"/mixtcomp_output.json"))
    nbCluster = output_learn$mixture$nbCluster
    launch_mixtcomp_predict(param_dir = param_dir,test_dir =  test_dir, nbCluster)
    data_predict = read.table(paste0(test_dir,"/data_predict.csv"),sep=";",stringsAsFactors = F,header = T)
    subdirs_list = list.dirs(recursive = F,param_dir)

    if(length(subdirs_list)>0){
      output_predict = fromJSON(paste0(test_dir,"/mixtcomp_output.json"))
      completed_clusters = output_predict$variable$data$z_class$completed
      for(cluster in 1:nbCluster){
        next_param_dir = paste0(param_dir,"/subcluster_",cluster)
        if(file.exists(paste0(next_param_dir,"/mixtcomp_output.json"))){
          next_output_learn = fromJSON(paste0(next_param_dir,"/mixtcomp_output.json"))
          if(next_output_learn$mixture$warnLog==""){
            next_test_dir = paste0(test_dir,"/subcluster_",cluster)
            data_next_predict = data_predict[which(completed_clusters==cluster),]
            dir.create(next_test_dir)
            write.table(data_next_predict,paste0(next_test_dir,"/data_predict.csv"),sep = ";",na = "",row.names = F)
            expand_predict(test_dir = next_test_dir,param_dir = next_param_dir)
          }
        }
      }
    }
  }

#' Prune a hierarchical clsutering. The pruning can be done according to three different strategies
#'
#'  - The purity of the clusters : if a cluster is purer than a certain threshold then the subclusters are not useful
#'  - The size of the cluster : below a certain size, a cluster should not have been subclusterized.
#'  - The quality of the completion : Is the global completion more accurate with this subclusterization ? Aims at avoiding overfitting.
#'
#'  These strategies can be cumulated.
#'
#' @param dir A string, the path of the hierarchical clustering to prune
#' @param purity_strategy a list of two elements, that defines which variable purity to check and according to which threshold (double between 0 and 1)
#' @param cluster_size_strategy a list of one element that define the cluster size threshold.
#' @param completion_quality_strategy a list of two elements, the variable to consider
#'
#' @return void
#' @export
#'
#' @examples prune_hierarchy(..)
#' @author Etienne Goffinet
prune_hierarchy <-
  function(dir,
           purity_strategy = list(var = NULL,threshold=NULL),
           cluster_size_strategy = list(threshold=NULL),
           completion_quality_strategy = list(var=NULL,data_observed=NULL,test_dir=NULL)) {

    list_subdir = list.dirs(path = dir,recursive = F,full.names = T)
    if(length(list_subdir)>0){
      # check the strategies

      output = fromJSON(paste0(dir,"/mixtcomp_output.json"))

      # Prune if needed
      if(all(!is.null(unlist(purity_strategy)))){
        purity = max(table(output$variable$data[[purity_strategy$var]]$completed)/output$mixture$nbInd)
        if(purity > purity_strategy$threshold){
          unlink(list_subdir,recursive = TRUE,force = TRUE)
        }
      }

      if(!is.null(cluster_size_strategy$threshold)){
        if(output$mixture$nbInd < cluster_size_strategy$threshold){
          unlink(list_subdir,recursive = TRUE,force = TRUE)
        }
      }

      if(all(!is.null(unlist(completion_quality_strategy)))){

        current_error =
        error_with_subdir
      }

      # Else, try to the prune the subdirectories
      for (subdir in list_subdir) {
        list_next_subdir = list.dirs(path = subdir,recursive = F,full.names = T)
        if(length(list_next_subdir)>0){

          prune_hierarchy(
            subdir,
            purity_strategy = purity_strategy,
            cluster_size_strategy = cluster_size_strategy,
            completion_quality_strategy = completion_quality_strategy
          )

          prune_hierarchy(
            dir,
            purity_strategy = purity_strategy,
            cluster_size_strategy = cluster_size_strategy,
            completion_quality_strategy = completion_quality_strategy
          )

        } else {
          output_curdir = fromJSON(paste0())
        }
      }
    } else {

    }
  }

