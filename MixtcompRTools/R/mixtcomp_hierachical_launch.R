
extract_data_per_cluster <- function(dir) {
  outputMixtcomp <- fromJSON(paste0(dir, "/mixtcomp_output.json"))
  data <- outputMixtcomp$

  for (i in 1:output$mixture$nbCluster) {
      subdir <- paste0(dir, "/subcluster_", i)
    write.csv(data, paste0(subdir, "/data_mixtcomp.csv"))
    file.copy(paste0(dir, "/descriptor.csv"),
              paste0(subdir, "/descriptor.csv"))
  }
}

launch_mixtcomp <- function(dir, nClass) {
  # Check if JsonMixtComp executable is available
  paths = Sys.getenv("PATH")
  if(!any(file.exists(paste0(strsplit(paths,":")[[1]],"/JsonMixtComp")))){
    stop("JsonMixtComp cannot be called from MixtCompRTools.
         In order to be callable, 'JsonMixtComp' must be stored in any of the folder designated by the paths contained in the environement variable PATH")
  }
  path_input <- paste0(dir, "/mixtcomp_input.json")
  path_output <- paste0(dir, "/mixtcomp_output.json")

  resGetData <- getData(c(
    paste0(dir, "/data_mixtcomp.csv"),
    paste0(dir, "/descriptor.csv")
  ))

  for (i in 1:length(resGetData$lm)) {
    resGetData$lm[[i]]$data <- as.character(resGetData$lm[[i]]$data)
  }

  mcStrategy = list(
    nbBurnInIter = 2,
    nbIter = 2,
    nbGibbsBurnInIter = 2,
    nbGibbsIter = 2,
    ratioInitialization= 1
  )
  arg_list_json <- toJSON(
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

  cmd <- paste("JsonMixtComp", path_input, path_output)
  system(cmd)

}

#' Create Subdirectories during the expand phase
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
  function(data_path, descriptor_path, nClass, depth,output_dir=NULL) {
    # Get directory of the data_path
    if(is.null(output_dir)){output_dir = dirname(data_path)}
    data_name = basename(data_path)
    # If results dirs doesnt' exist, create it
    newDir = paste0(output_dir, "/", strsplit(data_name,".csv")[[1]])
    if (!dir.exists(newDir)) {
      dir.create(newDir)
      file.copy(descriptor_path, paste0(newDir, "/descriptor.csv"))
      file.copy(data_path, paste0(newDir, "/data_mixtcomp.csv"))
      launch_mixtcomp(dir = newDir,nClass = nClass)
    }
    # run expand on it
#
#     for (i in 1:depth) {
#       expand(newDir, nClass, data, descriptor)
#     }

    cat("Hierarchical clustering complete !")
  }

Aggregate_Clusters <- function(dir) {

}
