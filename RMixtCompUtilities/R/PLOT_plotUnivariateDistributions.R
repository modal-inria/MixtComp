# MixtComp version 4 - july 2019
# Copyright (C) Inria - Université de Lille - CNRS

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>


#' Mean and 95\%-level confidence intervals per class
#'
#'
#' @param output object returned by \emph{mixtCompLearn} function from \emph{RMixtComp} or \emph{rmcMultiRun} function
#' from \emph{RMixtCompIO}
#' @param var name of the variable
#' @param class class to plot
#' @param grl if TRUE plot the CI for the dataset and not only classes
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... other parameters (see \emph{Details})
#'
#' @details For functional data, three other parameters are available:
#' \describe{
#'  \item{add.obs}{if TRUE, observations are added to the plot. Default = FALSE.}
#'  \item{add.CI}{if FALSE, confidence intervals are removed from the plot. Default = TRUE.}
#'  \item{xlim}{xlim of the plot.}
#'  \item{ylim}{ylim of the plot.}
#' }
#'
#'
#' @examples
#' if (requireNamespace("RMixtCompIO", quietly = TRUE)) {
#'   dataLearn <- list(
#'     var1 = as.character(c(rnorm(50, -2, 0.8), rnorm(50, 2, 0.8))),
#'     var2 = as.character(c(rnorm(50, 2), rpois(50, 8)))
#'   )
#'
#'   model <- list(
#'     var1 = list(type = "Gaussian", paramStr = ""),
#'     var2 = list(type = "Poisson", paramStr = "")
#'   )
#'
#'   algo <- list(
#'     nClass = 2,
#'     nInd = 100,
#'     nbBurnInIter = 100,
#'     nbIter = 100,
#'     nbGibbsBurnInIter = 100,
#'     nbGibbsIter = 100,
#'     nInitPerClass = 3,
#'     nSemTry = 20,
#'     confidenceLevel = 0.95,
#'     ratioStableCriterion = 0.95,
#'     nStableCriterion = 10,
#'     mode = "learn"
#'   )
#'
#'   resLearn <-RMixtCompIO::rmcMultiRun(algo, dataLearn, model, nRun = 3)
#'
#'   # plot
#'   plotDataCI(resLearn, "var1")
#' }
#'
#' @author Matthieu MARBAC
#' @family plot
#' @export
plotDataCI <- function(output, var, class = seq_len(output$algo$nClass), grl = FALSE, pkg = c("ggplot2", "plotly"), ...) {
  pkg <- match.arg(pkg)

  if (!(var %in% names(output$variable$type))) {
    stop("This variable does not exist in the mixture model.")
  }

  type <- output$variable$type[[var]]

  if (is.null(output$algo$dictionary$z_class)) {
    labels <- paste0("Class ", seq_len(output$algo$nClass))
  } else {
    labels <- output$algo$dictionary$z_class$old
  }

  switch(type,
    "Gaussian" = plotCINumericData(extractCIGaussianVble(var, output, class, grl), var, class, grl, pkg, labels),
    "Weibull" = plotCINumericData(extractCIWeibullVble(var, output, class, grl), var, class, grl, pkg, labels),
    "Multinomial" = plotCategoricalData(extractCIMultiVble(var, output, class, grl), var, class, grl, pkg, labels),
    "Poisson" = plotCINumericData(extractCIPoissonVble(var, output, class, grl), var, class, grl, pkg, labels),
    "NegativeBinomial" = plotCINumericData(extractCINegBinomialVble(var, output, class, grl), var, class, grl, pkg, labels),
    "Func_CS" = plotFunctionalData(output, var, classToPlot = class, pkg = pkg, ...),
    "Func_SharedAlpha_CS" = plotFunctionalData(output, var, classToPlot = class, pkg = pkg, ...),
    warning(paste0("Not (yet) available for model ", type))
  )
}


# Mean and 95% confidence level per class for a numeric variable (Gaussian or Poisson)
# @author Matthieu Marbac
plotCINumericData <- function(data, var, class, grl, pkg = c("ggplot2", "plotly"), labels, ...) {
  pkg <- match.arg(pkg)

  p <- switch(pkg,
    "ggplot2" = ggplotCINumericData(data, var, class, grl, labels),
    "plotly" = plotlyCINumericData(data, var, class, grl, ...)
  )

  p
}

# @author Matthieu Marbac
plotlyCINumericData <- function(data, var, class, grl, ...) {
  text1 <- paste0(
    "Class.", class, "<br>",
    "Mean: ", round(data$mean[seq_along(class)], 2), "<br>",
    "CI-95%: [", round(data$lower[seq_along(class)], 2),
    ",", round(data$uppers[seq_along(class)], 2), "]"
  )
  if (grl) {
    text1 <- c(text1, paste0(
      "Grl <br>",
      "Mean: ", round(data$mean[length(data$mean)], 2), "<br>",
      "CI-95%: [", round(data$lower[length(data$mean)], 2),
      ",", round(data$uppers[length(data$mean)], 2), "]"
    ))
  }
  p <- plot_ly(
    x = data$mean,
    y = c(seq_along(data$mean)),
    type = "scatter",
    mode = "markers",
    showlegend = FALSE,
    hoverinfo = "text",
    text = text1, ...
  ) %>% layout(
    title = "Mean and 95%-level confidence intervals per class",
    paper_bgcolor = "rgb(255,255,255)", plot_bgcolor = "rgb(229,229,229)",
    xaxis = list(
      title = var,
      titlefont = list(
        family = "Arial, sans-serif",
        size = 18
      ),
      gridcolor = "rgb(255,255,255)",
      showgrid = TRUE,
      showline = FALSE,
      showticklabels = TRUE,
      tickcolor = "rgb(127,127,127)",
      ticks = "outside",
      zeroline = FALSE,
      range = c(min(data$lower), max(data$uppers)) + c(-1, 1) * abs(max(data$uppers) - min(data$lower)) / 10
    ),
    yaxis = list(
      title = "Class",
      gridcolor = "rgb(255,255,255)",
      showgrid = FALSE,
      showline = FALSE,
      showticklabels = FALSE,
      tickcolor = "rgb(127,127,127)", ticks = "",
      zeroline = FALSE
    )
  )
  for (i in seq_along(class)) {
    p <- add_trace(p,
      x = c(data$lower[i], data$uppers[i]), # x0, x1
      y = c(i, i), # y0, y1
      mode = "lines",
      line = list(color = "rgba(0,100,80,0.4)", width = 20),
      showlegend = FALSE,
      hoverinfo = "text",
      text = paste("Class.", i, "<br>",
        "Mean: ", round(data$mean[i], 2), "<br>",
        "CI-95%: [", round(data$lower[i], 2),
        ",", round(data$uppers[i], 2), "]",
        sep = ""
      )
    )
  }
  if (grl) {
    i <- length(data$lower)
    p <- add_trace(p,
      x = c(data$lower[i], data$uppers[i]), # x0, x1
      y = c(i, i), # y0, y1
      mode = "lines",
      line = list(color = "rgba(100, 10,80,0.4)", width = 20),
      showlegend = FALSE,
      hoverinfo = "text",
      text = paste("Class.", i, "<br>",
        "Mean: ", round(data$mean[i], 2), "<br>",
        "CI-95%: [", round(data$lower[i], 2),
        ",", round(data$uppers[i], 2), "]",
        sep = ""
      )
    )
  }

  p
}

# @author Quentin Grimonprez
ggplotCINumericData <- function(data, var, class, grl, labels = class) {
  labelClass <- c()
  if (grl) {
    labelClass <- factor(c(labels[class], "all"), levels = c(labels, "all"))
  } else {
    labelClass <- factor(labels[class], levels = c(labels, "all"))
  }

  df <- data.frame(
    class = labelClass,
    classlo = seq_along(data$mean) - 0.1,
    classup = seq_along(data$mean) + 0.1,
    mean = data$mean,
    lower = data$lower,
    uppers = data$uppers
  )
  p <- ggplot(df, aes_string(x = "mean", y = "class")) +
    geom_point() +
    geom_rect(
      data = df,
      mapping = aes_string(xmin = "lower", xmax = "uppers", ymin = "classlo", ymax = "classup", fill = "class"),
      color = "black",
      alpha = 0.5
    ) +
    labs(title = "Mean and 95%-level confidence intervals per class", x = var, y = "Class") +
    theme(legend.position = "none") +
    scale_fill_discrete(name = "Class", drop = FALSE)

  p
}

# @author Matthieu Marbac
plotCategoricalData <- function(data, var, class, grl, pkg = c("ggplot2", "plotly"), labels = class, ...) {
  pkg <- match.arg(pkg)

  p <- switch(pkg,
    "ggplot2" = ggplotCategoricalData(data, var, class, grl, labels),
    "plotly" = plotlyCategoricalData(data, var, class, grl, ...)
  )

  p
}

# Barplot for categorical data (only the levels included in the 95 confidence level for at least one component are plotted)
# @author Matthieu Marbac
plotlyCategoricalData <- function(data, var, class, grl, ...) {
  formattedW <- lapply(
    seq_along(class),
    function(k) {
      list(
        y = data$probs[k, ],
        type = "bar",
        hoverinfo = "text",
        text = paste0("class ", class[k], "\n", paste(data$levels, data$probs[k, ], sep = ": ", collapse = "\n")),
        showlegend = FALSE,
        marker = list(line = list(color = "black", width = 1.5))
      )
    }
  )
  if (grl) {
    formattedW <- c(
      formattedW,
      list(list(
        y = data$probs[nrow(data$probs), ],
        type = "bar",
        hoverinfo = "text",
        text = "grl",
        showlegend = FALSE,
        marker = list(line = list(color = "red", width = 1.5))
      ))
    )
  }
  # Reduce the list of plotly compliant objs, starting with the plot_ly() value and
  # adding the `add_trace` at the following iterations
  p <- Reduce(function(acc, curr) do.call(add_trace, c(list(p = acc), curr)),
    formattedW,
    init = plot_ly(x = data$levels, ...) %>%
      layout(
        title = "Distribution per class",
        paper_bgcolor = "rgb(255,255,255)", plot_bgcolor = "rgb(229,229,229)",
        xaxis = list(
          title = var,
          gridcolor = "rgb(255,255,255)",
          showgrid = TRUE,
          showline = FALSE,
          showticklabels = TRUE,
          tickcolor = "rgb(127,127,127)",
          ticks = "outside",
          tickvals = data$levels,
          zeroline = FALSE
        ),
        yaxis = list(
          title = "Probability",
          gridcolor = "rgb(255,255,255)",
          showgrid = TRUE,
          showline = FALSE,
          showticklabels = TRUE,
          tickcolor = "rgb(127,127,127)",
          ticks = "outside",
          zeroline = FALSE
        )
      )
  )
  p
}


# @author Quentin Grimonprez
ggplotCategoricalData <- function(data, var, class, grl, labels) {
  labelClass <- c()
  if (grl) {
    labelClass <- factor(c(labels[class], "all"), levels = c(labels, "all"))
  } else {
    labelClass <- factor(labels[class], levels = c(labels, "all"))
  }

  df <- data.frame(
    value = as.numeric(t(data$probs)),
    categ = rep(data$levels, nrow(data$probs)),
    class = rep(labelClass, each = length(data$levels))
  )
  p <- ggplot(data = df, aes_string(x = "categ", y = "value", fill = "class")) +
    geom_bar(stat = "identity", position = position_dodge()) +
    labs(title = "Distribution per class", x = var, y = "Probability") +
    scale_fill_discrete(name = "Class", drop = FALSE)

  p
}


# Mean and 95% confidence level confidence  for functional data
# @author Matthieu Marbac
plotFunctionalData <- function(output, var, add.obs = FALSE, ylim = NULL, xlim = NULL, add.CI = TRUE, classToPlot = NULL,
                               pkg = c("ggplot2", "plotly"), ...) {
  pkg <- match.arg(pkg)

  # Computation of the Confidence Intervals (CI)
  data <- extractCIFunctionnalVble(var, output)

  p <- switch(pkg,
    "ggplot2" = ggplotFunctionalData(data, output, var, add.obs, ylim, xlim, add.CI, classToPlot),
    "plotly" = plotlyFunctionalData(data, output, var, add.obs, ylim, xlim, add.CI, classToPlot, ...)
  )

  p
}

# @author Matthieu Marbac
plotlyFunctionalData <- function(data, output, var, add.obs = FALSE, ylim = NULL, xlim = NULL, add.CI = TRUE,
                                 classToPlot = NULL, ...) {
  G <- output$algo$nClass

  if (is.null(classToPlot)) {
    classToPlot <- seq_len(G)
  }

  # Computation of the bounds for x-axis and y-axis
  if (is.null(xlim)) {
    xlim <- range(sapply(
      seq_along(output$variable$data[[var]]$time),
      function(j) range(output$variable$data[[var]]$time[[j]])
    ))
  }
  if (is.null(ylim)) {
    if (add.obs) {
      ylim <- range(c(min(data$inf), max(data$sup), sapply(
        seq_along(output$variable$data[[var]]$data),
        function(j) range(output$variable$data[[var]]$data[[j]])
      )))
    } else {
      ylim <- c(min(data$inf), max(data$sup))
    }
  }
  formattedW <- NULL
  # observations are added for plot
  if (add.obs) {
    partition <- output$variable$data$z_class$completed

    for (i in seq_along(output$variable$data[[var]]$time)) {
      if (partition[i] %in% classToPlot) {
        formattedW <- c(formattedW, list(
          list(
            x = output$variable$data[[var]]$time[[i]],
            y = output$variable$data[[var]]$data[[i]],
            type = "scatter", mode = "lines", showlegend = FALSE, line = list(color = "rgba(0,100,80,0.4)", width = 1)
          )
        ))
      }
    }
  }
  # mean curves and CI are added
  for (k in classToPlot) {
    if (add.CI) {
      formattedW <- c(formattedW, list(
        list(
          y = data$inf[k, ],
          type = "scatter", mode = "lines",
          line = list(color = "transparent"), showlegend = FALSE,
          name = paste("95% lcl class", k, sep = ".")
        ),
        list(
          y = data$sup[k, ],
          type = "scatter", mode = "lines",
          fill = "tonexty", fillcolor = "rgba(0,100,80,0.4)", line = list(color = "transparent"),
          showlegend = FALSE, name = paste("95% ucl class", k, sep = ".")
        ),
        list(
          y = data$mean[k, ],
          type = "scatter", mode = "lines",
          line = list(color = k, width = 4), showlegend = FALSE,
          name = paste("mean class", k, sep = ".")
        )
      ))
    } else {
      formattedW <- c(formattedW, list(
        list(
          y = data$mean[k, ],
          type = "scatter", mode = "lines",
          line = list(color = k, width = 4), showlegend = FALSE,
          name = paste("mean class", k, sep = ".")
        )
      ))
    }
  }
  # Reduce the list of plotly compliant objs, starting with the plot_ly() value and
  # adding the `add_trace` at the following iterations
  p <- Reduce(function(acc, curr) do.call(add_trace, c(list(p = acc), curr)),
    formattedW,
    init = plot_ly(x = data$time, ...) %>%
      layout(
        title = "Mean curves and 95%-level confidence intervals per class",
        paper_bgcolor = "rgb(255,255,255)", plot_bgcolor = "rgb(229,229,229)",
        xaxis = list(
          title = "Time",
          gridcolor = "rgb(255,255,255)",
          showgrid = TRUE,
          showline = FALSE,
          showticklabels = TRUE,
          tickcolor = "rgb(127,127,127)",
          ticks = "outside",
          range = xlim,
          zeroline = FALSE
        ),
        yaxis = list(
          title = "Value",
          gridcolor = "rgb(255,255,255)",
          showgrid = TRUE,
          showline = FALSE,
          showticklabels = TRUE,
          tickcolor = "rgb(127,127,127)",
          ticks = "outside",
          range = ylim,
          zeroline = FALSE
        )
      )
  )
  p
}
# @author Quentin Grimonprez
ggplotFunctionalData <- function(data, output, var, add.obs = FALSE, ylim = NULL, xlim = NULL, add.CI = TRUE,
                                 classToPlot = NULL) {
  nClass <- nrow(data$mean)
  if (!is.null(classToPlot)) {
    data$mean <- data$mean[classToPlot, , drop = FALSE]
    data$inf <- data$inf[classToPlot, , drop = FALSE]
    data$sup <- data$sup[classToPlot, , drop = FALSE]
  } else {
    classToPlot <- seq_len(nrow(data$mean))
  }

  df <- as.data.frame(t(data$mean))
  df$time <- data$time

  p <- ggplot()

  # add observation
  if (add.obs) {
    for (i in seq_along(output$variable$data[[var]]$time)) {
      df2 <- data.frame(time = output$variable$data[[var]]$time[[i]], value = output$variable$data[[var]]$data[[i]])
      p <- p + geom_line(aes_string(y = "value", x = "time"), data = df2, stat = "identity", size = 0.5, alpha = 0.5)
    }
  }


  # confidence intervals
  if (add.CI) {
    for (i in seq_len(nrow(data$inf))) {
      df2 <- data.frame(inf = data$inf[i, ], sup = data$sup[i, ], time = data$time)
      p <- p + geom_ribbon(data = df2, aes_string(x = "time", ymin = "inf", ymax = "sup"), alpha = 0.3)
    }
  }


  # mean class
  df2 <- data.frame(
    time = rep(data$time, length(classToPlot)), mean = unlist(df[, -ncol(df)]),
    Class = factor(rep(paste("Class", classToPlot), each = length(df$time)), levels = paste("Class", seq_len(nClass)))
  )

  p <- p + geom_line(data = df2, mapping = aes_string(x = "time", y = "mean", group = "Class", colour = "Class"), size = 1.5)

  p <- p +
    labs(title = "Mean curves and 95%-level confidence intervals per class", x = "Time", y = var) +
    scale_color_discrete(name = "Class", drop = FALSE)

  if (!is.null(xlim)) {
    p <- p + layout(axis = list(range = xlim))
  }
  if (!is.null(ylim)) {
    p <- p + layout(yaxis = list(range = ylim))
  }

  p
}
