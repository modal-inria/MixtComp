#' Plot functional data by clusters
#'
#' @param clusters vector of the clusters associated with data
#' @param var String, The name of the functional data to plot
#' @param data_func list of two elements :time and data / Functional data extracted from Mixtcomp Output
#' @param depth Positive Integer, maximum depth of the subclustering to plot
#' @param max_nb_lines Positive integer, Defines the maximum number of lines to plot (default is 100)
#' @param which_to_highlight String, the name of the cluster to highlight automatically in the plot
#'
#' @family visualisation
#' @return a plotly plot
#'
#' @export
#'
plot_functional_hierarchique <- function(clusters, var, data_func, depth,
                                         max_nb_lines = 100, which_to_highlight = NULL) {
  clusters = substr(clusters, 1, (2 * (depth - 1) + 1))
  time = data_func$time
  data = data_func$data
  unique_clusters = sort(unique(clusters))
  max_line_per_cluster = max(max_nb_lines %/% length(unique_clusters), 2)

  pal = colorRampPalette(brewer.pal(11, "Spectral"))(length(unique_clusters))

  if (!is.null(dim(data))) {
    data = as.list(data.frame(t(data)))
  }
  if (!is.null(dim(time))) {
    time = as.list(data.frame(t(time)))
  }
  if (is.null(which_to_highlight)) {
    is_visible = rep(TRUE, length(unique_clusters))
  } else {
    is_visible = rep("legendonly", length(unique_clusters))
    is_visible[unique_clusters == which_to_highlight] = "TRUE"
  }

  p = plot_ly()

  data_by_cluster = lapply(unique_clusters, function(x) {
    data[which(clusters == x)]
  })
  time_by_cluster = lapply(unique_clusters, function(x) {
    time[which(clusters == x)]
  })

  names(data_by_cluster) = unique_clusters
  for (i in (1:length(unique_clusters))) {
    data_cl = data_by_cluster[[unique_clusters[i]]]
    time_cl = time_by_cluster[[unique_clusters[i]]]

    p = p %>% add_trace(
      y = data_cl[1][[1]],
      x = time_cl[1][[1]],
      type = "scatter",
      mode = "lines",
      legendgroup = unique_clusters[i],
      name = unique_clusters[i],
      showlegend = TRUE,
      line = list(color = pal[i],
                  width = 2),
      visible = is_visible[i]
    )

    for (k in 2:min(max_line_per_cluster, length(data_cl))) {
      p = p %>% add_trace(
        y = data_cl[k][[1]],
        x = time_cl[k][[1]],
        type = "scatter",
        mode = "lines",
        legendgroup = unique_clusters[i],
        name = unique_clusters[i],
        showlegend = FALSE,
        line = list(color = pal[i],
                    width = 2),
        visible = is_visible[i]
      )
    }
  }
  p = p %>%
    layout(
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
#' @param var String, The name of the categorical data to plot
#' @param data the data to be plotted
#' @param depth Positive Integer, maximum depth of the subclustering to plot
#' @param dictionary (optional) List, the dictionary in which are stored the real names of the categorical data levels
#' @param order_by (optional) element among : size or purity, which define the order of the different cluster in the histogram.
#' @param source  (optional) A string specifying the name of the source to be added to the plotly plot (in case the plot is used in a shiny app)
#' @family visualisation
#' @return a plotly plot
#'
#' @export
#'
plot_categorical_hierarchique <- function(clusters, var, data, depth,
                                          dictionary = NULL, order_by = NULL, source = NULL) {
  clusters = substr(clusters, 1, (2 * (depth - 1) + 1))
  unique_clusters = sort(unique(clusters))
  pal = colorRampPalette(brewer.pal(11, "Spectral"))(length(unique_clusters))

  df = data.frame(table(data.frame(data, clusters)))
  df = df[order(df$clusters), ]
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
      df_2 = df_2[order(size_clusters, decreasing = T),]
      df_2$clusters = factor(df_2$clusters, levels = df_2$clusters)
    }
    if (order_by == ("purity")) {
      # Purity is defined as the proportion of the predominant level
      purity = as.numeric(apply(df_2[, -1], 1, max))
      df_2 = df_2[order(purity, decreasing = T),]
      df_2$clusters = factor(df_2$clusters, levels = df_2$clusters)
    }
  }

  p = plot_ly(
    y = ~ as.numeric(df_2[, 2]),
    x = df_2$clusters,
    type = 'bar',
    name = colnames(df_2)[2],
    source = source
  )

  for (i in 3:ncol(df_2)) {
    p = p %>% add_trace(y = as.numeric(df_2[, i]), name = colnames(df_2)[i])
  }
  p = p %>% layout(yaxis = list(title = 'Count'), barmode = 'stack')
  p = p %>%
    layout(yaxis = list(title = 'Count'))

  return(p)
}


#' Plot proportions by clusters
#'
#' @param clusters vector of the clusters associated with data
#' @param var String, The name of the categorical data to plot
#' @param data the data to be plotted
#' @param depth Positive Integer, maximum depth of the subclustering to plot
#' @param order_by (optional) element among : size or purity, which define the order of the different cluster in the histogram.
#' @family visualisation
#' @return a plotly plot
#'
#' @export
#'
plot_proportion_hierarchique <- function(clusters, var, data,
                                         depth, order_by = NULL) {
  clusters = substr(clusters, 1, (2 * (depth - 1) + 1))
  unique_clusters = sort(unique(clusters))

  df = data.frame(table(data.frame(data, clusters)))
  df = df[order(df$clusters), ]
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
      df_2 = df_2[order(size_clusters, decreasing = T),]
      df_2$clusters = factor(df_2$clusters, levels = df_2$clusters)
    }
    if (order_by == ("purity")) {
      # Purity is defined as the proportion of the predominant level
      purity = as.numeric(apply(df_2[, setdiff(colnames(df_2), c("clusters", "Proportions"))], 1, max))
      df_2 = df_2[order(purity, decreasing = T),]
      df_2$clusters = factor(df_2$clusters, levels = df_2$clusters)
    }
  }

  p = plot_ly(
    y = ~ as.numeric(df_2$Proportions),
    x = df_2$clusters,
    type = 'bar',
    name = colnames(df_2)[2],
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
#' @param var String, The name of the categorical data to plot
#' @param depth Positive Integer, maximum depth of the subclustering to plot
#' @param idx_test vector of the index of the test data in the data_observed and data_completed
#' @param data_observed Observed data to be compared with data_completed
#' @param data_completed Completed data
#' @param order_by (optional) element among : size, purity or quality, which define the order of the different cluster in the histogram.
#' @family visualisation
#' @return a plotly plot
#'
#' @export
#'
plot_quality_pred_hierarchique <- function(clusters, var, idx_test, data_observed,
                                           data_completed, depth, order_by = NULL) {
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
  df = df[order(df$clusters), ]
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
      df = df[order(size_clusters, decreasing = T),]
      df$clusters = factor(df$clusters, levels = df$clusters)
    }
    if (order_by == ("purity")) {
      # Purity is defined as the proportion of the predominant level
      print(df)

      purity = as.numeric(apply(df[, setdiff(colnames(df), c("clusters", "quality"))], 1, max))

      print(purity)
      df = df[order(purity, decreasing = T),]
      df$clusters = factor(df$clusters, levels = df$clusters)
    }
    if (order_by == ("quality")) {
      # Purity is defined as the proportion of the predominant level
      quality = df$quality
      df = df[order(quality, decreasing = T),]
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
                  width = 1.5))) %>% layout(yaxis = list(title = '% of correct predictions', xaxis =
                                                           list(title = "cluster")))

  return(p)
}
