
extract_data_per_cluster <- function(dir){
  outputMixtcomp <- fromJSON(paste0(dir,"/mixtcomp_output.json"))

  for(i in 1:output$mixture$nbCluster){
    data <-
    write
    file.copy(paste0(dir, "/descriptor.csv"),
              paste0(paste0(dir,"/subcluster_",i), "/descriptor.csv"))
  }

}

launch_mixtcomp <- function(dir, nClass) {

}

create_subdirectories <- function(dir,nClass){
  for(i in 1:nClass){dir.create(paste0(dir,"/subcluster_",i))}
}

expand <- function(dir) {
  existing_subdirs_in_current_dir <- list.dirs(dir)
  if (length(existing_subdirs_in_current_dir) > 0) {
    for (subdir in existing_subdirs_in_current_dir) {
      expand(subdir)
    }
  } else {
    create_subdirectories(dir,nClass)
    extract_data_per_cluster(dir,nClass)


    launch_mixtcomp(subdir, nClass)
  }
}

Mixtcomp_Hierarchical <- function(data, descriptor, nClass, depth) {
  for (i in 1:depth) {
    expand(dir, nClass, data, descriptor)
  }
  cat("Hierarchical clustering complete !")
}

Aggregate_Clusters <- function(dir) {

}
