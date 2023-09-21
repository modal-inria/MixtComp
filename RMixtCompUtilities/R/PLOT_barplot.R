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


#' Barplot of the discriminative power of the variables
#'
#' @details The discriminative power of variable j is defined by 1 - C(j)
#' \deqn{C(j)=  -\sum_{k=1}^K \sum_{i=1}^n P(Z_i=k|x_{ij}) ln(P(Z_i=k|x_{ij})) / (n*\log(K))}
#'
#' @param output object returned by \emph{mixtCompLearn} function from \emph{RMixtComp} or \emph{rmcMultiRun} function
#' from \emph{RMixtCompIO}
#' @param class NULL or a number of classes. If NULL, return the discrimative power of variables globally otherwise
#' return the discrimative power of variables in the given class
#' @param ylim vector of length 2 defining the range of y-axis
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... arguments to be passed to plot_ly
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
#'   plotDiscrimVar(resLearn)
#'
#'   plotDiscrimVar(resLearn, class = 1)
#' }
#'
#' @seealso \code{\link{computeDiscrimPowerVar}}
#'
#' @author Matthieu MARBAC
#' @family plot
#' @export
plotDiscrimVar <- function(output, class = NULL, ylim = c(0, 1), pkg = c("ggplot2", "plotly"), ...) {
  pkg <- match.arg(pkg)

  ## Get information
  # names of variables
  namesVbles <- names(output$variable$type)[names(output$variable$type) != "z_class"]
  namesShort <- abbreviate(namesVbles, 6, use.classes = FALSE)
  # discriminative power (1 - Cj), saved at slot pvdiscrimclasses of JSON file
  pvDiscrim <- round(computeDiscrimPowerVar(output, class), 2)
  # Full names and type of the variables
  textMous <- paste(namesVbles, "<br>", as.character(output$variable$type[-1]))

  ## Variables are sorted by decreasing order of their discriminative power
  ## Character must be convert in factor (otherwise variables are plotted with alphabetic order)
  orderVbles <- order(pvDiscrim, decreasing = TRUE)
  pvDiscrim <- pvDiscrim[orderVbles]
  namesVbles <- factor(namesVbles[orderVbles], levels = namesVbles[orderVbles])
  namesShort <- factor(namesShort[orderVbles], levels = namesShort[orderVbles])
  textMous <- factor(textMous[orderVbles], levels = textMous[orderVbles])

  plotTitle <- ifelse(is.null(class),
    "Discriminative level of the variables",
    paste0("Discriminative level of the variables in class ", class)
  )

  ## Barplot
  visuVbles <- switch(pkg,
    "plotly" = barplotly(pvDiscrim, namesShort, main = plotTitle, ylim = ylim, text = textMous, ...),
    "ggplot2" = ggbarplot(pvDiscrim, namesShort, main = plotTitle, ylim = ylim, col.text = "black")
  )
  visuVbles
}

#' Barplot of the discriminative power of the classes
#'
#' @details The discriminative power of class k is defined by 1 - D(k)
#' \deqn{D(k) =  -\sum_{i=1}^n P(Z_i=k|x_i) \log(P(Z_i=k|x_i)) / (n*\exp(-1))}
#'
#' @param output object returned by \emph{mixtCompLearn} function from \emph{RMixtComp} or \emph{rmcMultiRun} function
#' from \emph{RMixtCompIO}
#' @param ylim vector of length 2 defining the range of y-axis
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... arguments to be passed to plot_ly
#'
#' @examples
#'
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
#'   plotDiscrimClass(resLearn)
#' }
#'
#' @seealso \code{\link{computeDiscrimPowerClass}}
#'
#' @author Matthieu MARBAC
#' @family plot
#' @export
plotDiscrimClass <- function(output, ylim = c(0, 1), pkg = c("ggplot2", "plotly"), ...) {
  pkg <- match.arg(pkg)

  ## Get information
  # names of classes
  if (is.null(output$algo$dictionary$z_class)) {
    namesClass <- paste0("Class ", seq_len(output$algo$nClass))
  } else {
    namesClass <- output$algo$dictionary$z_class$old
  }
  # discriminative power (1 - Dk), saved at slot pvdiscrimvbles of JSON file
  pvDiscrim <- round(1 - (-colMeans(log(output$variable$data$z_class$stat**output$variable$data$z_class$stat)) / exp(-1)), 2)

  ## Classes are sorted by decreasing order of their discriminative power
  ## Character must be convert in factor (otherwise alphabetic order is considered)
  orderClass <- order(pvDiscrim, decreasing = TRUE)
  namesClass <- factor(namesClass[orderClass], levels = namesClass[orderClass])
  pvDiscrim <- pvDiscrim[orderClass]
  ## Barplot
  visuClass <- switch(pkg,
    "plotly" = barplotly(pvDiscrim, namesClass, main = "Discriminative level of the classes", ylim = ylim, ...),
    "ggplot2" = ggbarplot(pvDiscrim, namesClass, main = "Discriminative level of the classes", ylim = ylim, col.text = "black")
  )

  visuClass
}


#' Plot the mixture's proportions
#'
#' @param output object returned by \emph{mixtCompLearn} function from \emph{RMixtComp} or \emph{rmcMultiRun} function
#' from \emph{RMixtCompIO}
#' @param pkg "ggplot2" or "plotly". Package used to plot
#' @param ... arguments to be passed to plot_ly
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
#'   plotProportion(resLearn)
#' }
#'
#' @family plot
#' @author Quentin Grimonprez
#' @export
plotProportion <- function(output, pkg = c("ggplot2", "plotly"), ...) {
  pkg <- match.arg(pkg)

  ylimProportion <- min(max(output$variable$param$z_class$stat[, 1]) + 0.1, 1)
  if (is.null(output$algo$dictionary$z_class)) {
    label <- paste0("Class ", seq_len(output$algo$nClass))
  } else {
    label <- output$algo$dictionary$z_class$old
  }


  p <- switch(pkg,
    "ggplot2" = ggbarplot(
      output$variable$param$z_class$stat[, 1], label, main = "Proportion", xlab = "", ylab = "",
      ylim = c(0, ylimProportion), col.text = "black"
    ),
    "plotly" = barplotly(
      output$variable$param$z_class$stat[, 1], label, main = "Proportion", xlab = "",
      ylab = "", ylim = c(0, ylimProportion), text = NULL, ...
    )
  )

  p
}

# @author Quentin Grimonprez
barplotly <- function(value, label, main, xlab = "", ylab = "", ylim = c(0, 1), text = NULL, ...) {
  p <- plot_ly(
    x = label,
    y = value,
    type = "bar",
    text = text
  ) %>%
    layout(
      title = main,
      xaxis = list(title = xlab),
      yaxis = list(title = ylab, range = ylim),
      annotations = list(
        x = label,
        y = value,
        text = round(value, 2),
        xanchor = "center",
        yanchor = "bottom",
        showarrow = FALSE
      )
    )
  p
}

# @author Matthieu Marbac
ggbarplot <- function(value, label, main, xlab = "", ylab = "", ylim = c(0, 1), col.text = "white") {
  df <- data.frame(var = factor(label, levels = label), discrim = value, roundedDiscrim = round(value, 2))
  ggplot(data = df, aes_string(x = "var", y = "discrim")) +
    geom_bar(stat = "identity") +
    geom_text(aes_string(label = "roundedDiscrim"), vjust = -0.5, color = col.text, size = 5) +
    theme_minimal() +
    ylim(ylim[1], ylim[2]) +
    labs(title = main, x = xlab, y = ylab) +
    theme(
      legend.position = "none",
      axis.text.x = element_text(angle = 315, hjust = 0)
    )
}
