# MixtComp version 4.0  - july 2019
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


# gaussian ----------------------------------------------------------------

# @author Vincent Kubicki
gaussianGenerator <- function(present, param) {
  x <- gaussianFullGenerator(param)

  xStr <- gaussianHideData(present, x, param)

  return(xStr)
}

# @author Vincent Kubicki
gaussianFullGenerator <- function(param) {
  rnorm(1, param$mean, param$sd)
}

# @author Vincent Kubicki
gaussianHideData <- function(present, x, param) {
  if (!present) {
    missType <- sample(4, size = 1)

    bounds <- round(sort(rnorm(2, param$mean, param$sd)), 5)


    x <- switch(missType,
      "1" = "?",
      "2" = paste0("[", bounds[1], ":", bounds[2], "]"),
      "3" = paste0("[", "-inf", ":", bounds[2], "]"),
      "4" = paste0("[", bounds[1], ":", "+inf", "]")
    )
  }

  return(as.character(x))
}


# poisson -----------------------------------------------------------------

# @author Vincent Kubicki
poissonGenerator <- function(present, param) {
  x <- poissonFullGenerator(param)

  xStr <- poissonHideData(present, x, param)

  return(xStr)
}

# @author Vincent Kubicki
poissonFullGenerator <- function(param) {
  rpois(1, lambda = param)
}

# @author Vincent Kubicki
poissonHideData <- function(present, x, param) {
  if (!present) {
    missType <- sample(3, size = 1)

    bounds <- round(sort(rpois(2, param)), 5)
    if (bounds[1] == bounds[2]) {
      bounds[2] <- bounds[2] + 1
    }


    x <- switch(missType,
      "1" = "?",
      "2" = paste0("[", bounds[1], ":", bounds[2], "]"),
      "3" = paste0("[", bounds[1], ":", "+inf", "]")
    )
  }

  return(as.character(x))
}



# negative binomial -------------------------------------------------------

# @author Vincent Kubicki, Quentin Grimonprez
negativeBinomialGenerator <- function(present, param) {
  x <- negativeBinomialFullGenerator(param)

  xStr <- negativeBinomialHideData(present, x, param)

  return(xStr)
}

# @author Vincent Kubicki, Quentin Grimonprez
negativeBinomialFullGenerator <- function(param) {
  rnbinom(1, size = param$n, prob = param$p)
}

# @author Vincent Kubicki, Quentin Grimonprez
negativeBinomialHideData <- function(present, x, param) {
  if (!present) {
    missType <- sample(3, size = 1)

    bounds <- round(sort(rnbinom(2, size = param$n, prob = param$p)), 5)
    if (bounds[1] == bounds[2]) {
      bounds[2] <- bounds[2] + 1
    }

    x <- switch(missType,
      "1" = "?",
      "2" = paste0("[", bounds[1], ":", "+inf", "]"),
      "3" = paste0("[", bounds[1], ":", bounds[2], "]")
    )
  }

  return(as.character(x))
}


# weibull -----------------------------------------------------------------

# @author Vincent Kubicki
weibullGenerator <- function(present, param) {
  x <- weibullFullGenerator(param)
  xStr <- weibullHideData(present, x, param)
  return(xStr)
}

# @author Vincent Kubicki
weibullFullGenerator <- function(param) {
  rweibull(1, param$shape, param$scale)
}

# @author Vincent Kubicki
weibullHideData <- function(present, x, param) {
  if (!present) {
    missType <- sample(3, size = 1)

    bounds <- round(sort(rweibull(2, param$shape, param$scale)), 5)

    x <- switch(missType,
      "1" = "?",
      "2" = paste0("[", bounds[1], ":", "+inf", "]"),
      "3" = paste0("[", bounds[1], ":", bounds[2], "]")
    )
  }

  return(as.character(x))
}



# multinomial -------------------------------------------------------------

# @author Vincent Kubicki
categoricalGenerator <- function(present, param) {
  x <- categoricalFullGenerator(param)

  xStr <- categoricalHideData(present, x, param)

  return(xStr)
}

# @author Vincent Kubicki
categoricalFullGenerator <- function(param) {
  which(rmultinom(1, 1, param) == 1)
}

# @author Vincent Kubicki
categoricalHideData <- function(present, x, param) {
  if (!present) {
    missType <- sample(2, size = 1) # 1: missing, 2: missingInterval

    if (missType == 1) {
      x <- "?"
    } else {
      nbModalities <- length(param)

      if (nbModalities <= 2) {
        x <- "?"
      } else {
        nbModalitiesToDraw <- max(sample(2:(nbModalities - 1), 1), 2)
        modalities <- sort(sample(nbModalities, nbModalitiesToDraw))

        x <- paste0("{", paste0(modalities, collapse = ","), "}")
      }
    }
  }


  return(as.character(x))
}
