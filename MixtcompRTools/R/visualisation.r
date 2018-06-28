
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

