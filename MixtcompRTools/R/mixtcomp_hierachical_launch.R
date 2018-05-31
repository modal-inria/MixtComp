


extract_data_per_cluster <- function(dir) {
  outputMixtcomp <- fromJSON(paste0(dir, "/mixtcomp_output.json"))
  for (i in 1:output$mixture$nbCluster) {
    data <-
      subdir <- paste0(dir, "/subcluster_", i)
    write.csv(data, paste0(subdir, "/data_mixtcomp.csv"))
    file.copy(paste0(dir, "/descriptor.csv"),
              paste0(subdir, "/descriptor.csv"))
  }
}


launch_mixtcomp <- function(dir, nClass) {

  path_input <- paste0(dir,"/mixtcomp_input.json")
  path_output <- paste0(dir,"/mixtcomp_output.json")
  resGetData <- getData(c(paste0(dir,"/data_mixtcomp.csv"),
                          paste0(dir,"/descriptor.csv")))

  for(i in 1:length(resGetData$lm)){
    resGetData$lm[[i]]$data <- as.character(resGetData$lm[[i]]$data)
  }

  mcStrategy = list(nbBurnInIter=15,nbIter=15,nbGibbsBurnInIter=15,nbGibbsIter=15)
  arg_list_json <- toJSON(list(by_row=FALSE,
                               resGetData_lm = resGetData$lm,
                               mcStrategy=mcStrategy,
                               nbClass = n_class,
                               confidenceLevel = 0.95,
                               mode="learn"),
                          auto_unbox = T)
  write(x = arg_list_json,path_input)

  ###### Mixtcomp run

  cmd <- paste("../src/JsonMixtComp", path_input,path_output)
  system(cmd)

}

#' Title
#'
#' @param dir A directory
#' @param nClass A positive integer
#'
#' @return void
#' @export
#'
#' @examples create_subdirectories(getwd(),2)
create_subdirectories <- function(dir, nClass) {
  if (!dir.exists(dir)) {
    stop(
      "Directory path given in input doesn't exist.
      This function should not be used out of the scope of the expand function."
    )
  }
  if (nClass < 1) {
    stop("nClass parameter should be positive and not greater than 10.")
  }

  for (i in 1:nClass) {
    dir.create(paste0(dir, "/subcluster_", i))
  }

}

expand <- function(dir, nClass) {
  existing_subdirs_in_current_dir <- list.dirs(dir)
  if (length(existing_subdirs_in_current_dir) > 0) {
    for (subdir in existing_subdirs_in_current_dir) {
      expand(subdir)
    }
  } else {
    create_subdirectories(dir, nClass)
    extract_data_per_cluster(dir, nClass)

    for (i in 1:nClass) {
      launch_mixtcomp(subdir, nClass)
    }
  }
}

launch_Mixtcomp_Hierarchical <-
  function(data_path, descriptor_path, nClass, depth) {
    # Get directory of the data_path
    dir = dirname(data_path)
    data_name = basename(data_path)
    # If results dirs doesnt' exist, create it
    newDir = paste0(dir, "/", data_name)
    if (!dir.exists(newDir)) {
      dir.create(newDir)
      file.copy(descriptor_path, paste0(newDir, "/descriptor.csv"))
      file.copy(data_path, paste0(newDir, "/data_mixtcomp.csv"))
    }
    # run expand on it

    for (i in 1:depth) {
      expand(dir, nClass, data, descriptor)
    }

    cat("Hierarchical clustering complete !")
  }

Aggregate_Clusters <- function(dir) {

}
