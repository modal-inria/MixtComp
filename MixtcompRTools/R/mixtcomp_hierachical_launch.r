



#' During the expand phase, cut data according to clusters and copy to subfolder
#'
#' @param dir A String
#'
#' @return void
#' @export
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
    data_cluster <- data[idx_cluster,]
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
#' @export
#'
#' @examples
#' \dontrun {
#' launch_mixtcomp("data/data_mixtcomp",3)
#' }
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
    nbBurnInIter = 1,
    nbIter = 15,
    nbGibbsBurnInIter = 15,
    nbGibbsIter = 15,
    ratioInitialization = 1
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
expand <- function(dir, nClass,strategy) {
  parallel = TRUE
  existing_subdirs_in_current_dir <- list.dirs(dir, recursive = F)
  print("#######################")

  print(dir)
  print(existing_subdirs_in_current_dir)
  # If the directory already contains subdirectories, expand the subdirs
  if (length(existing_subdirs_in_current_dir) > 0) {
    for (subdir in existing_subdirs_in_current_dir) {
      expand(subdir, nClass,strategy)
    }
  } else {
    # If no subdirectories exist, create them, extract data into them and launch mixtcomp.
    MixtCompOutput <-
      jsonlite::fromJSON(paste0(dir, "/mixtcomp_output.json"))
    print(strategy)

    print(MixtCompOutput$mixture$nbInd)
    print(table(MixtCompOutput$variable$data[[strategy$var]]$completed) / MixtCompOutput$mixture$nbInd)
    if ((MixtCompOutput$mixture$nbInd > strategy$threshold_nInd) &
        (max(
          table(MixtCompOutput$variable$data[[strategy$var]]$completed) / MixtCompOutput$mixture$nbInd
        ) < strategy$threshold_purity)) {
      create_subdirectories(dir, MixtCompOutput$mixture$nbCluster)
      extract_data_per_cluster(dir)
      subdirs = paste0(dir,
                       "/subcluster_",
                       1:MixtCompOutput$mixture$nbCluster)

      # execute mixtcomp in parallel mode or not
      if (parallel == FALSE) {
        for (subdir in subdirs) {
          launch_mixtcomp(subdir, nClass)
        }
      } else {
        cl <- makeCluster(length(subdirs))
        registerDoParallel(cl)
        foreach(subdir = subdirs, .packages = 'mixtcompRTools') %dopar% {
          launch_mixtcomp(subdir, nClass)
        }
        stopCluster(cl)

      }
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
#' @importFrom RMixtComp functionalInterPolyGenerator
#'
#' @examples launch_Mixtcomp_Hierarchical("data/data.csv","data/descriptor.csv",3,3)
#' \dontrun{
#' param <- list(
#' x = c(0., 10., 20.),
#' y = c(0., 10., 0.),
#' sd = 0.1,
#' tMin = 0.,
#' tMax = 20.,
#' nTime = 100)
#'
#' data = c()
#' for(i in 1:2000){
#'   data <- c(data,functionalInterPolyGenerator(param=param))
#' }
#'
#' data <- cbind(data,rnorm(n = 2000,mean = c(0,5,10,100),sd=c(1,2,3,4)))
#' colnames(data) = c("F1","X1")
#' descriptor <-
#'   setNames(data.frame(matrix(
#'     ncol = ncol(data),
#'     nrow = 2,
#'    byrow = TRUE
#'  )), colnames(data))
#' descriptor[1,] <-
#'  c("Functional","Gaussian_sjk")
#' descriptor[2, 1] <- rep("nSub: 5, nCoeff: 2", 1)
#'
#' write.table(
#'   data,
#'   "data/data_mixtcomp.csv" ,
#'   sep = ";",
#'   na = "",
#'   row.names = F
#' )
#' write.table(descriptor,file="data/descriptor.csv",sep=";",na = "",row.names = F)
#' }
#' @author Etienne Goffinet
launch_Mixtcomp_Hierarchical <-
  function(data_path,
           descriptor_path,
           nClass,
           depth,
           output_dir = NULL,
           strategy=list(var=1,threshold_nInd=1,threshold_purity=2)) {
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
      launch_mixtcomp(dir = newDir, nClass = nClass)
    }
    # run expand on it
    if (depth == 0) {
      cat("Hierarchical clustering complete !")
      return()
    }

    for (i in 1:depth) {
      expand(dir=newDir, nClass,strategy)
    }
    cat("Hierarchical clustering complete !")

  }

#' Aggregate the hierarchical clusters belongings
#'
#' @param dir String, a path to the directory in which are the subclusters to be aggregated
#'
#' @return void
#' @export
#'
#' @examples aggregate_clusters(dir_path_result)
aggregate_clusters <- function(dir) {
  # Si le dossier contient un fichier output
  # Si le warnLog de ce fichier output est vide
  # l'indicatif du cluster est l'indicatif précédent suivi du cluster actuel
  # pour chaque sous cluster contenu dans ce dossier, l'indicatif est le
  # nouvel indicatif suivi de - et de l'indicatif fourni par le cluster suivant
  print(dir)
  output_path = paste0(dir, "/mixtcomp_output.json")
  if (!file.exists(output_path)) {
    return("")
  }
  output = jsonlite::fromJSON(paste0(dir, "/mixtcomp_output.json"))
  if (output$mixture$warnLog != "") {
    return("")
  }
  if (output$mixture$nbInd < 10) {
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



#' Aggregate the hierarchical clusters completed data
#'
#' @param dir String, a path to the directory in which are the subclusters to be aggregated
#' @param var The variable to be aggregated
#'
#' @return void
#' @export
#'
#' @examples aggregate_clusters(dir_path_result)
aggregate_completed <- function(dir, var) {
  # Si le dossier contient un fichier output
  # Si le warnLog de ce fichier output est vide
  # l'indicatif du cluster est l'indicatif précédent suivi du cluster actuel
  # pour chaque sous cluster contenu dans ce dossier, l'indicatif est le
  # nouvel indicatif suivi de - et de l'indicatif fourni par le clsuter suivant
  print(dir)
  output_path = paste0(dir, "/mixtcomp_output.json")
  if (!file.exists(output_path)) {
    return(NA)
  }
  output = jsonlite::fromJSON(paste0(dir, "/mixtcomp_output.json"))

  if (output$mixture$warnLog != "") {
    return(NA)
  }

  clusters = output$variable$data$z_class$completed
  data_completed =  output$variable$data[[var]]$completed

  existing_subdirs_in_dir = list.dirs(dir, recursive = F)
  if (length(existing_subdirs_in_dir) > 0) {
    for (k in unique(clusters)) {
      path_subdir = paste0(dir, "/subcluster_", k)
      idx_cluster = which(clusters == k)
      data_completed_cluster = aggregate_completed(path_subdir, var)
      data_completed[idx_cluster][!is.na(data_completed_cluster)] = data_completed_cluster[!is.na(data_completed_cluster)]
    }
  }
  return(data_completed)
}
#' Display a visual representation of a function variable among the clusters
#'
#' @param clusters vector of the cluster associated to each observations
#' @param var Which functional variable to be plotted
#' @param data The data extracted from Mixtcomp output
#' @param depth The maximal depth of the clustering to be plotted
#' @param max_nb_lines The maximal number of functional observations to plot
#'
#' @return A plotly plot
#' @export
#' @import RColorBrewer
#' @import plotly
#' @examples plot_functional_hierarchique(dir_path_result)
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

    pal = grDevices::colorRampPalette(brewer.pal(11, "Spectral"))(length(unique_clusters))
    p = plot_ly()
    max_line_per_cluster = max(max_nb_lines %/% length(unique_clusters), 2)
    for (i in ((1:length(unique_clusters)))) {
      data_cl = data[which(clusters == unique_clusters[i])]
      time_cl = time[which(clusters == unique_clusters[i])]

      p = p %>% plotly::add_trace(
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

      for (k in 2:min(max_line_per_cluster, length(data_cl))) {
        p = p %>% plotly::add_trace(
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
      plotly::layout(
        title = paste0(
          "Distribution of the variable ",
          var,
          " in the sub-clustering of depth ",
          depth
        ),
        xaxis = list(title = "time"),
        yaxis = list(title = var),
        plot_bgcolor = '#8a8a5c'
      )
    return(p)
  }


#' Plot functional data by clusters
#'
#' @param clusters vector of the clusters associated with data
#' @param var The name of the functional data to plot
#' @param output The output in which to find the data to plot
#' @param depth maximum depth of the subclustering to plot
#' @param max_nb_lines Defines the maximum number of lines to plot (default is 100)
#'
#' @return a plotly plot
#' @export
#'
#' @examples
plot_functional_hierarchique <-
  function(clusters,
           var,
           data_func,
           depth,
           max_nb_lines = 100) {
    clusters = substr(clusters, 1, (2 * (depth - 1) + 1))
    time = data_func$time
    data = data_func$data
    unique_clusters = sort(unique(clusters))

    pal = grDevices::colorRampPalette(brewer.pal(11, "Spectral"))(length(unique_clusters))
    p = plot_ly()
    max_line_per_cluster = max(max_nb_lines %/% length(unique_clusters), 2)

    if (!is.null(dim(data))) {
      data = as.list(data.frame(t(data)))
    }
    if (!is.null(dim(time))) {
      time = as.list(data.frame(t(time)))
    }

    for (i in (1:length(unique_clusters))) {
      data_cl = data[which(clusters == unique_clusters[i])]
      time_cl = time[which(clusters == unique_clusters[i])]

      p = p %>% plotly::add_trace(
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

      for (k in 2:min(max_line_per_cluster, length(data_cl))) {
        # print("#####")
        # print(data_cl[k][[1]])
        p = p %>% plotly::add_trace(
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
      plotly::layout(
        title = paste0(
          "Distribution of the variable ",
          var,
          " in the sub-clustering of depth ",
          depth
        ),
        xaxis = list(title = "time"),
        yaxis = list(title = var),
        plot_bgcolor = '#8a8a5c'
      )
    return(p)
  }



#' Plot categorical data by clusters
#'
#' @param clusters vector of the clusters associated with data
#' @param var The name of the categorical data to plot
#' @param output The output in which to find the data to plot
#' @param depth maximum depth of the subclustering to plot
#' @param max_nb_lines Defines the maximum number of lines to plot (default is 100)
#' @param dictionary the dictionary in which are stored the real names of the categorical data levels
#'
#' @return a plotly plot
#' @export
#'
#' @examples
plot_categorical_hierarchique <-
  function(clusters,
           var,
           data,
           depth,
           max_nb_lines = 100,
           dictionary = NULL,
           order_by = NULL) {
    clusters = substr(clusters, 1, (2 * (depth - 1) + 1))
    unique_clusters = sort(unique(clusters))
    pal = grDevices::colorRampPalette(brewer.pal(11, "Spectral"))(length(unique_clusters))
    max_line_per_cluster = max(max_nb_lines %/% length(unique_clusters), 2)

    df = data.frame(table(data.frame(data, clusters)))
    df = df[order(df$clusters),]
    df = split(df, df$clusters)
    df_2 = data.frame(stringsAsFactors = F, do.call(rbind, lapply(df, function(x) {
      c(as.character(x$clusters[1]),
        round(as.numeric(x$Freq) /
                sum(as.numeric(x$Freq)), 3))
    })))
    colnames(df_2) = c("clusters", levels(df[[1]]$data))

    if (!is.null(dictionary)) {
      colnames(df_2) = c("clusters", dictionary[[var]][match(dictionary[[var]][, 2], levels(df[[1]]$data)), 1])
    }

    if (!is.null(order_by)) {
      if (order_by == ("size")) {
        size_clusters = table(clusters)
        df_2 = df_2[order(size_clusters, decreasing = T), ]
        df_2$clusters = factor(df_2$clusters, levels = df_2$clusters)
      }
      if (order_by == ("purity")) {
        # Purity is defined as the proportion of the predominant level
        purity = as.numeric(apply(df_2[, -1], 1, max))
        df_2 = df_2[order(purity, decreasing = T), ]
        df_2$clusters = factor(df_2$clusters, levels = df_2$clusters)
      }
    }

    p = plot_ly(
      y = ~ as.numeric(df_2[, 2]),
      x = df_2$clusters,
      type = 'bar',
      name = colnames(df_2)[2]
    )

    for (i in 3:ncol(df_2)) {
      p = p %>% add_trace(y = as.numeric(df_2[, i]), name = colnames(df_2)[i])
    }
    p = p %>% layout(yaxis = list(title = 'Count'), barmode = 'stack')
    p = p %>%
      plotly::layout(yaxis = list(title = 'Count'))

    return(p)
  }


#' Plot proportions by clusters
#'
#' @param clusters vector of the clusters associated with data
#' @param var The name of the categorical data to plot
#' @param output The output in which to find the data to plot
#' @param depth maximum depth of the subclustering to plot
#' @param max_nb_lines Defines the maximum number of lines to plot (default is 100)
#' @param dictionary the dictionary in which are stored the real names of the categorical data levels
#'
#' @return a plotly plot
#' @export
#'
#' @examples
plot_proportion_hierarchique <-
  function(clusters,
           var,
           data,
           depth,
           order_by = NULL) {
    clusters = substr(clusters, 1, (2 * (depth - 1) + 1))
    unique_clusters = sort(unique(clusters))

    df = data.frame(table(data.frame(data, clusters)))
    df = df[order(df$clusters),]
    df = split(df, df$clusters)
    df_2 = data.frame(stringsAsFactors = F, do.call(rbind, lapply(df, function(x) {
      c(as.character(x$clusters[1]),
        round(as.numeric(x$Freq) /
                sum(as.numeric(x$Freq)), 3))
    })))
    df_2 = cbind(df_2, as.vector(unname(table(clusters))))
    colnames(df_2) = c("clusters", levels(df[[1]]$data), "Proportions")

    if (!is.null(order_by)) {
      if (order_by == ("size")) {
        size_clusters = table(clusters)
        df_2 = df_2[order(size_clusters, decreasing = T), ]
        df_2$clusters = factor(df_2$clusters, levels = df_2$clusters)
      }
      if (order_by == ("purity")) {
        # Purity is defined as the proportion of the predominant level
        purity = as.numeric(apply(df_2[, setdiff(colnames(df_2), c("clusters", "Proportions"))], 1, max))
        df_2 = df_2[order(purity, decreasing = T), ]
        df_2$clusters = factor(df_2$clusters, levels = df_2$clusters)
      }
    }

    p = plot_ly(
      y = ~ as.numeric(df_2$Proportions),
      x = df_2$clusters,
      type = 'bar',
      name = colnames(df_2)[2],
      showlegend = F,
      marker = list(
        color = 'rgb(255, 204, 102)',
        line = list(color = 'rgb(179, 119, 0)',
                    width = 1.5)
      )
    ) %>% layout(yaxis = list(title = 'Size'))

    return(p)
  }


#' Plot quality of prediction by clusters
#'
#' @param clusters vector of the clusters associated with data
#' @param var The name of the categorical data to plot
#' @param output The output in which to find the data to plot
#' @param depth maximum depth of the subclustering to plot
#' @param max_nb_lines Defines the maximum number of lines to plot (default is 100)
#' @param dictionary the dictionary in which are stored the real names of the categorical data levels
#'
#' @return a plotly plot
#' @export
#'
#' @examples
plot_quality_pred_hierarchique <-
  function(clusters,
           var,
           idx_test,
           data_cat,
           data_observed,
           data_completed,
           depth,
           order_by = NULL) {
    # path_results = "~/Alstom/Démo/visualisation/data/data_sample_1_5_3_23/"
    # var="Degradation_type"
    # clusters = aggregate_clusters(paste0(path_results,"data_mixtcomp"))
    # output_mixtcomp = convertJsonRobject(fromJSON(paste0(path_results,"data_mixtcomp/mixtcomp_output.json")))
    # data_func = output_mixtcomp$variable$data$Maneuver_Power
    # data_mixtcomp = read.csv(paste0(path_results,"/data_mixtcomp.csv"),sep=";",stringsAsFactors = F,header=T)
    # idx_test = which(data_mixtcomp[[var]]=="?")
    # data_observed = read.table(paste0(path_results,"data_mixtcomp_complete.csv"),sep=";",header=T,stringsAsFactors = F)[[var]]
    # data_completed = aggregate_completed(paste0(path_results,"data_mixtcomp"),var)

    clusters = substr(clusters, 1, (2 * (depth - 1) + 1))
    unique_clusters = sort(unique(clusters))

    is_predicted_right = (data_observed[idx_test] == data_completed[idx_test])
    quality_by_cluster = unlist(lapply(unique_clusters, function(x) {
      mean(na.rm = TRUE, is_predicted_right[which(clusters == x)])
    }))
    quality_by_cluster[is.na(quality_by_cluster)] = 1
    data.frame(aggregate(
      is_predicted_right,
      by = list(Cluster = clusters[idx_test]),
      FUN = mean
    ))

    df = data.frame(table(data.frame(data_observed, clusters)))
    df = df[order(df$clusters),]
    df = split(df, df$clusters)
    df_2 = data.frame(stringsAsFactors = F, do.call(rbind, lapply(df, function(x) {
      c(as.character(x$clusters[1]),
        round(as.numeric(x$Freq) /
                sum(as.numeric(x$Freq)), 3))
    })))
    df_2 = cbind(df_2, quality_by_cluster)
    colnames(df_2) = c("clusters", levels(df[[1]]$data_observed), "quality")
    df = df_2

    if (!is.null(order_by)) {
      if (order_by == ("size")) {
        size_clusters = table(clusters)
        df = df[order(size_clusters, decreasing = T), ]
        df$clusters = factor(df$clusters, levels = df$clusters)
      }
      if (order_by == ("purity")) {
        # Purity is defined as the proportion of the predominant level
        print(df)

        purity = as.numeric(apply(df[, setdiff(colnames(df), c("clusters", "quality"))], 1, max))

        print(purity)
        df = df[order(purity, decreasing = T), ]
        df$clusters = factor(df$clusters, levels = df$clusters)
      }
      if (order_by == ("quality")) {
        # Purity is defined as the proportion of the predominant level
        quality = df$quality
        df = df[order(quality, decreasing = T), ]
        df$clusters = factor(df$clusters, levels = df$clusters)
      }
    }

    p = plot_ly(
      y = ~ as.numeric(df$quality),
      x = df$clusters,
      type = 'bar',
      name = colnames(df)[2],
      showlegend = F,
      marker = list(
        color = 'rgb(153, 230, 153)',
        line = list(color = 'rgb(25, 103, 25)',
                    width = 1.5)
      )
    ) %>% layout(yaxis = list(title = '% of correct predictions in this cluster'))

    return(p)
  }


#' Plot functional data by clusters
#'
#' @param clusters vector of the clusters associated with data
#' @param var The name of the functional data to plot
#' @param output The output in which to find the data to plot
#' @param depth maximum depth of the subclustering to plot
#' @param max_nb_lines Defines the maximum number of lines to plot (default is 100)
#'
#' @return a plotly plot
#' @export
#'
#' @examples
aggregate_completed_depth <-
  function(dir,
           var,
           depth) {
    print("#############")
    print(depth)
    print(dir)
    output = fromJSON(paste0(dir, "/mixtcomp_output.json"))
    data_completed = output$variable$data[[var]]$completed
    if (depth == 0) {
      return(data_completed)
    } else {
      clusters = output$variable$data$z_class$completed
      for (cluster in 1:output$mixture$nbCluster) {
        subdir = paste0(dir, "/subcluster_", cluster)
        print(subdir)
        if (file.exists(paste0(subdir, "/mixtcomp_output.json"))) {
          next_output = fromJSON(paste0(subdir, "/mixtcomp_output.json"))
          if (next_output$mixture$warnLog == "") {
            data_completed[which(clusters == cluster)] = aggregate_completed_depth(dir = subdir,
                                                                                   var = var,
                                                                                   depth = depth - 1)
          }
        }
      }
    }
    return(data_completed)
  }
