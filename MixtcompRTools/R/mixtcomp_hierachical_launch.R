
#' During the expand phase, cut data according to clusters and copy to subfolder
#'
#' @param dir A String
#'
#' @return void
#' @export
#'
#' @examples extract_data_per_cluster("data/data_mixtcomp/x.csv")
extract_data_per_cluster <- function(dir){
  MixtCompOutput <- fromJSON(paste0(dir, "/mixtcomp_output.json"))
  data <-
    read.csv(paste0(dir, "/data_mixtcomp.csv"),
             sep = ";",
             header = T)

  for (i in 1:MixtCompOutput$mixture$nbCluster) {
    idx_cluster <-
      which(MixtCompOutput$variable$data$z_class$completed == i)
    data_cluster <- data[idx_cluster, ]
    subdir <- paste0(dir, "/subcluster_", i)
    write.table(data_cluster, paste0(subdir, "/data_mixtcomp.csv"),sep=";",row.names = F)
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
#' @export
#'
#' @examples launch_mixtcomp("data/data_mixtcomp",3)
launch_mixtcomp <- function(dir, nClass) {
  # Check if JsonMixtComp executable is available
  paths = Sys.getenv("PATH")
  if (!any(file.exists(paste0(strsplit(paths, ":")[[1]], "/JsonMixtComp")))) {
    stop(
      "JsonMixtComp cannot be called from MixtCompRTools.
      In order to be callable, 'JsonMixtComp' must be stored in any of the folder designated by the paths contained in the environement variable PATH"
    )
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
    ratioInitialization = 1
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
#' @export
#'
#' @examples expand()
expand <- function(dir, nClass) {
  existing_subdirs_in_current_dir <- list.dirs(dir, recursive = F)
  if (length(existing_subdirs_in_current_dir) > 0) {
    for (subdir in existing_subdirs_in_current_dir) {
      expand(subdir)
    }
  } else {
    create_subdirectories(dir, nClass)
    extract_data_per_cluster(dir)
    subdirs=paste0(dir,"/subcluster_",1:nClass)

    for (subdir in subdirs) {
      launch_mixtcomp(subdir, nClass)
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
#' If Null, a directory with the dame of the dataset will be created and used as output directory instead.
#'
#' @return void
#' @export
#'
#' @examples launch_Mixtcomp_Hierarchical("data/data.csv","data/descriptor.csv",3,3)
launch_Mixtcomp_Hierarchical <-
  function(data_path,
           descriptor_path,
           nClass,
           depth,
           output_dir = NULL) {
    # Get directory of the data_path
    if (is.null(output_dir)) {
      output_dir = dirname(data_path)
    }
    data_name = basename(data_path)
    # If results dirs doesnt' exist, create it
    newDir = paste0(output_dir, "/", strsplit(data_name, ".csv")[[1]])
    if (!dir.exists(newDir)) {
      dir.create(newDir)
      file.copy(descriptor_path, paste0(newDir, "/descriptor.csv"))
      file.copy(data_path, paste0(newDir, "/data_mixtcomp.csv"))
      launch_mixtcomp(dir = newDir, nClass = nClass)
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
