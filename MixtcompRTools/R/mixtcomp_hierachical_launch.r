



#' During the expand phase, cut data according to clusters and copy to subfolder
#'
#' @param dir A String
#'
#' @return void
#' @export
#'
#' @examples extract_data_per_cluster("data/data_mixtcomp/x.csv")
extract_data_per_cluster <- function(dir) {
  print(dir)
  MixtCompOutput <- fromJSON(paste0(dir, "/mixtcomp_output.json"))
  data <-
    read.csv(paste0(dir, "/data_mixtcomp.csv"),
             sep = ";",
             header = T)

  for (i in 1:MixtCompOutput$mixture$nbCluster) {
    idx_cluster <-
      which(MixtCompOutput$variable$data$z_class$completed == i)
    data_cluster <- data[idx_cluster,]
    subdir <- paste0(dir, "/subcluster_", i)
    write.table(
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
    nbBurnInIter = 15,
    nbIter = 15,
    nbGibbsBurnInIter = 15,
    nbGibbsIter = 15,
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

  cmd <-
    paste(
      "/home/etienne/Mixtcomp_Docker_Flask/bin/JsonMixtComp",
      path_input,
      path_output
    )
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
  print(dir)
  print(existing_subdirs_in_current_dir)
  if (length(existing_subdirs_in_current_dir) > 0) {
    for (subdir in existing_subdirs_in_current_dir) {
      expand(subdir, nClass)
    }
  } else {
    create_subdirectories(dir, nClass)
    extract_data_per_cluster(dir)
    subdirs = paste0(dir, "/subcluster_", 1:nClass)

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
    print(newDir)
    if (!dir.exists(newDir)) {
      dir.create(newDir)
      file.copy(descriptor_path, paste0(newDir, "/descriptor.csv"))
      file.copy(data_path, paste0(newDir, "/data_mixtcomp.csv"))
      launch_mixtcomp(dir = newDir, nClass = nClass)
    }
    # run expand on it

    for (i in 1:depth) {
      expand(newDir, nClass)
    }

    cat("Hierarchical clustering complete !")
  }

#' Title
#'
#' @param dir
#'
#' @return
#' @export
#'
#' @examples
aggregate_clusters <- function(dir) {
  # Si le dossier contient un fichier output
  # Si le warnLog de ce fichier output est vide
  # l'indicatif du cluster est l'indicatif précédent suivi du cluster actuel
  # pour chaque sous cluster contenu dans ce dossier, l'indicatif est le
  # nouvel indicatif suivi de - et de l'indicatif fourni par le clsuter suivant
  print(dir)
  output_path = paste0(dir, "/mixtcomp_output.json")
  if (!file.exists(output_path)) {
    return("")
  }
  output = fromJSON(paste0(dir, "/mixtcomp_output.json"))
  if (output$mixture$warnLog != "") {
    return("")
  }

  clusters = output$variable$data$z_class$completed

  existing_subdirs_in_dir = list.dirs(dir, recursive = F)
  if (length(existing_subdirs_in_dir) > 0) {
    for (k in unique(clusters)) {
      path_subdir = paste0(dir, "/subcluster_", k)
      clusters[which(clusters == k)] = paste0(clusters[which(clusters ==
                                                               k)], "-", aggregate_clusters(path_subdir))

    }
  }

  return(clusters)

}

#' Title
#'
#' @param clusters
#' @param var
#' @param output
#' @param depth
#' @param max_nb_lines
#'
#' @return
#' @export
#'
#' @examples
plot_functional_hierarchique <-
  function(clusters,
           var,
           output,
           depth,
           max_nb_lines = 100) {

    clusters = substr(clusters, 1, (2 * (depth - 1) + 1))
    data = output$variable$data[[var]]$data
    time = output$variable$data[[var]]$time
    unique_clusters = sort(unique(clusters))

    pal = colorRampPalette(brewer.pal(11, "Spectral"))(length(unique_clusters))
    p = plot_ly()
    max_line_per_cluster = max(max_nb_lines %/% length(unique_clusters),2)
    for (i in ((1:length(unique_clusters)))) {
      data_cl = data[which(clusters == unique_clusters[i])]
      time_cl = time[which(clusters == unique_clusters[i])]

      p = p %>% add_trace(
        y = data_cl[1][[1]],
        x = time_cl[1][[1]],
        type = "scatter",
        mode = "lines",
        legendgroup = unique_clusters[i],
        name = unique_clusters[i],
        showlegend = TRUE,
        line = list(color = pal[i],
                    width = 2)
      )

      for (k in 2:min(max_line_per_cluster,length(data_cl))) {
        # print("#####")
        # print(data_cl[k][[1]])
        p = p %>% add_trace(
          y = data_cl[k][[1]],
          x = time_cl[k][[1]],
          type = "scatter",
          mode = "lines",
          legendgroup = unique_clusters[i],
          name = unique_clusters[i],
          showlegend = FALSE,
          line = list(color = pal[i],
                      width = 2)
        )

      }
    }
    p = p %>%
      layout(
        title = paste0(
          "Distribution of the variable ",var,
          " in the sub-clustering of depth ", depth
        ),
        xaxis = list(title = "time"),
        yaxis = list(title = var),
        plot_bgcolor='#8a8a5c'
      )
    return(p)
  }
