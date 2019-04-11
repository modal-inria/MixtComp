# Author : Étienne Goffinet

#' Aggregate the hierarchical clusters estimated
#'
#' @param dir String, a path to the directory in which are the subclusters to be aggregated
#' @param depth Positive Integer or NULL, maximum depth of the subclustering to plot. If NULL, the maximal depth of the clusters is assumed.
#' @family results aggregation
#'
#' @export
#'
aggregate_clusters <- function(dir, depth = NULL) {
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
  output = fromJSON(paste0(dir, "/mixtcomp_output.json"))
  if (!is.null(output$warnLog) || (!is.null(output$mixture$warnLog) && (output$mixture$warnLog != ""))) {
    return("")
  }
  oldMC <- is.null(output$algo)
  nInd <- ifelse(is.null(output$mixture$nbInd), output$algo$nInd, output$mixture$nbInd)
  if (nInd < 10) {
    return("")
  }
  if(oldMC)
    clusters = output$variable$data$z_class$completed
  else
    clusters = output$variable$data$z_class$completed$data

  if(is.null(depth))
    depth = Inf
  continue <- depth != 0
  if(continue)
  {
    existing_subdirs_in_dir = list.dirs(dir, recursive = FALSE)
    if (length(existing_subdirs_in_dir) > 0) {
      for (k in unique(clusters)) {
        path_subdir = paste0(dir, "/subcluster_", k)
        clusters[which(clusters == k)] = paste0(clusters[which(clusters == k)], "-", aggregate_clusters(path_subdir, depth = depth - 1))
      }
    }
  }
  return(clusters)
}

# Aggregate the hierarchical clusters completed data
#
# @param dir String, a path to the directory in which are the subclusters to be aggregated
# @param var String, The variable to be aggregated
#
# @return completed data
#
# @family results aggregation
#
aggregate_completed_max <- function(dir, var) {

  print(dir)
  output_path = paste0(dir, "/mixtcomp_output.json")
  if (!file.exists(output_path)) {
    return(NA)
  }
  output = fromJSON(paste0(dir, "/mixtcomp_output.json"))
  oldMC <- is.null(output$algo)


  if (!is.null(output$warnLog) || (!is.null(output$mixture$warnLog) && (output$mixture$warnLog != ""))) {
    return(NA)
  }


  if(oldMC)
    clusters = output$variable$data$z_class$completed
  else
    clusters = output$variable$data$z_class$completed$data

  if(oldMC)
    data_completed = output$variable$data[[var]]$completed
  else
    data_completed = output$variable$data[[var]]$completed$data

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

#' Aggregate the hierarchical clusters completed data
#'
#' @param dir String, path of the directory in which to finc the nested clustering
#' @param var String, The name of the functional data to plot
#' @param depth Positive Integer or NULL, maximum depth of the subclustering to plot. If NULL, the maximal depth of the clusters is assumed.
#'
#' @return completed data
#'
#' @family results aggregation
#' @export
#'
aggregate_completed <- function(dir, var, depth = NULL) {

  if(is.null(depth))
    return(aggregate_completed_max(dir,var))

  output = fromJSON(paste0(dir, "/mixtcomp_output.json"))

  oldMC <- is.null(output$algo)

  if(oldMC)
    data_completed = output$variable$data[[var]]$completed
  else
    data_completed = output$variable$data[[var]]$completed$data

  if (depth == 0) {
    return(data_completed)
  } else {
    if(oldMC)
      clusters = output$variable$data$z_class$completed
    else
      clusters = output$variable$data$z_class$completed$data

    nbCluster <- ifelse(oldMC, output$mixture$nbCluster, output$algo$nClass)
    for (cluster in 1:nbCluster) {
      subdir = paste0(dir, "/subcluster_", cluster)
      print(subdir)

      if (file.exists(paste0(subdir, "/mixtcomp_output.json"))) {
        next_output = fromJSON(paste0(subdir, "/mixtcomp_output.json"))
        if(is.null(next_output$warnLog) && (is.null(next_output$mixture$warnLog) || (next_output$mixture$warnLog == ""))) {
          data_completed[which(clusters == cluster)] = aggregate_completed(dir = subdir,
                                                                           var = var,
                                                                           depth = depth - 1)
        }
      }
    }
  }
  return(data_completed)
}

#' Aggregate the classification probabilities estimated in the nested clustering
#'
#' @param dir String, path of the directory in which to finc the nested clustering
#' @param depth Positive Integer, maximum depth of the subclustering to plot
#'
#' @return tik
#'
#' @family results aggregation
#'
#' @export
#'
aggregate_classification_probabilities <- function(dir, depth) {
  print(dir)

  output = fromJSON(paste0(dir, "/mixtcomp_output.json"))

  oldMC <- is.null(output$algo)

  if(oldMC)
    probs = output$variable$data$z_class$stat
  else
    probs = output$variable$data$z_class$stat$data

  if (depth == 0) {
    return(probs)
  } else {
    nbCluster <- ifelse(oldMC, output$mixture$nbCluster, output$algo$nClass)

    if(oldMC)
      clusters = output$variable$data$z_class$completed
    else
      clusters = output$variable$data$z_class$completed$data

    probs_completed = c()
    for (cluster in 1:nbCluster) {
      subdir = paste0(dir, "/subcluster_", cluster)
      if (file.exists(paste0(subdir, "/mixtcomp_output.json"))) {
        next_output = fromJSON(paste0(subdir, "/mixtcomp_output.json"))

        if(is.null(next_output$warnLog) && (is.null(next_output$mixture$warnLog) || (next_output$mixture$warnLog == ""))) {
          probs_sub = aggregate_classification_probabilities(subdir, depth = depth - 1)
          probs_all_obs = matrix(0,
                                 ncol = ncol(probs_sub),
                                 nrow = ifelse(oldMC, output$mixture$nbInd, output$algo$nInd))
          probs_all_obs[which(cluster == clusters), ] = probs_sub
          probs_completed = cbind(probs_completed, probs_all_obs)
        } else {
          print("WarnLog not empty")
          probs_completed = cbind(probs_completed, probs[, cluster])
        }
      } else {
        print("file doesn't exists")
        probs_completed = cbind(probs_completed, probs[, cluster])
      }
    }
  }
  return(probs_completed)
}
